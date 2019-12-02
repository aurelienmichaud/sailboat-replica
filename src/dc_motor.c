#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

#include "dc_motor.h"
#include "rf_input.h"	/* for _RF_INPUT_ANGLE_RANGE */
#include "Timer.h"

#define _DC_MOTOR_TIMER	TIM2 	/* Channel 2 */
#define _DC_MOTOR_ARR	2400		/* High PWM frequency won't work since the DC Motor cannot keep up, so favour a high value for the ARR */
#define _DC_MOTOR_PSC	0

static TIM_TypeDef *DCMotorTimer;

static void dc_motor_init(void);

void dc_motor_start(TIM_TypeDef *_DCMotorTimer)
{
	DCMotorTimer = _DCMotorTimer;
	dc_motor_init();
	Timer_Start(_DC_MOTOR_TIMER);
}

/* DC Motor can only work with TIM2 channel 2 */
static void dc_motor_init(void)
{
	/* DC Motor on GPIOA1 */
	LL_GPIO_InitTypeDef LLGPIO_struct, LLGPIO_struct2;
	
	/* Timer configuration in PWM mode */
	Timer_PWM_output_conf(DCMotorTimer, _DC_MOTOR_ARR, _DC_MOTOR_PSC);
	
	/* PWM Timer's GPIO */
	LLGPIO_struct.Pin 					= LL_GPIO_PIN_1;
	LLGPIO_struct.Mode       		= LL_GPIO_MODE_ALTERNATE;
	LLGPIO_struct.Speed     		= LL_GPIO_SPEED_FREQ_HIGH;
	LLGPIO_struct.OutputType		= LL_GPIO_OUTPUT_PUSHPULL;
	LLGPIO_struct.Pull					= LL_GPIO_PULL_DOWN;
	
	/* GPIO for DC Motor direction handling */
	LLGPIO_struct2.Pin 					= LL_GPIO_PIN_2;
	LLGPIO_struct2.Mode       	= LL_GPIO_MODE_OUTPUT;
	LLGPIO_struct2.Speed     		= LL_GPIO_SPEED_FREQ_HIGH;
	LLGPIO_struct2.OutputType		= LL_GPIO_OUTPUT_PUSHPULL;
	LLGPIO_struct2.Pull					= LL_GPIO_PULL_DOWN;
	
	/* Enable clock */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		
	LL_GPIO_Init(GPIOA, &LLGPIO_struct);
	LL_GPIO_Init(GPIOA, &LLGPIO_struct2);
	
	Timer_PWM_set_high_level_counter(_DC_MOTOR_TIMER, 0);
}

#define _DC_MOTOR_GET_CRR(__ANGLE) \
	((int)(((double)(__ANGLE) / (double) (_RF_INPUT_ANGLE_RANGE)) * (double) (_DC_MOTOR_ARR)))

#define POSITIVE	0
#define NEGATIVE 	1

void dc_motor_move(int angle)
{
	static int direction = POSITIVE;
	int crr;
	
	if (angle >= 0) {
		if (direction == NEGATIVE) {
			GPIOA->ODR |= 0x1 << 2;
			direction = POSITIVE;
		}
		
		crr = _DC_MOTOR_GET_CRR(angle);
	}
	/* angle < 0 */
	else {
		if (direction == POSITIVE) {
			GPIOA->ODR &= ~(0x1 << 2);
			direction = NEGATIVE;
		}
		
		crr = _DC_MOTOR_GET_CRR(-angle);
	}
	
	Timer_PWM_set_high_level_counter(DCMotorTimer, crr);
}
