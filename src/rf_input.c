#include "rf_input.h"
#include "Timer.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

#define _RF_INPUT_ARR		4999
#define _RF_INPUT_PSC		(72000000 / ((_RF_INPUT_ARR + 1) * 50) - 1)

#ifndef _RF_INPUT_ANGLE_RANGE
# define _RF_INPUT_ANGLE_RANGE	10.0
#endif

#define _RF_INPUT_MAX_DUTY_CYCLE			(((double)_RF_INPUT_ARR / 10.0))
#define _RF_INPUT_NEUTRAL_DUTY_CYCLE	(((double)_RF_INPUT_ARR * 3.0 / 40.0))

static TIM_TypeDef *RFInputTimer;
static int RFInputChannel;

static void rf_input_init(void);

void rf_input_start(TIM_TypeDef *__RFInputTimer, int channel)
{
	RFInputTimer 		= __RFInputTimer;
	RFInputChannel 	= channel;
	
	rf_input_init();
}

/* RF Input only works with PB6-TIM4CH1 or PB7-TIM4CH2 */
static void rf_input_init(void)
{
	/* Servo Engine on PIN8 */
	LL_GPIO_InitTypeDef LLGPIO_struct;

	/* Timer configuration in PWM input mode */
	Timer_PWM_input_conf(RFInputTimer,
											RFInputChannel,
											_RF_INPUT_ARR,
											_RF_INPUT_PSC);
	
	/* GPIOB6 or GPIOB7 depending on the channel */
	if 			(RFInputChannel == 1) 	{ LLGPIO_struct.Pin = LL_GPIO_PIN_6; }
	else if (RFInputChannel == 2) 	{ LLGPIO_struct.Pin = LL_GPIO_PIN_7; }
	
	LLGPIO_struct.Mode = LL_GPIO_MODE_FLOATING;
	LLGPIO_struct.Pull = LL_GPIO_PULL_DOWN;
	
	/* Start clock for GPIOB */
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	LL_GPIO_Init(GPIOB, &LLGPIO_struct);
}

/* Returns an angle in [ - _RF_INPUT_ANGLE_RANGE ; _RF_INPUT_ANGLE_RANGE] */
int rf_input_get_angle(void)
{
	int val;
	
	if (RFInputChannel == 1)
		val = RFInputTimer->CCR2;
	
	else if(RFInputChannel == 2)
		val = RFInputTimer->CCR1;
		
	return (int)((val - _RF_INPUT_NEUTRAL_DUTY_CYCLE) * _RF_INPUT_ANGLE_RANGE / (_RF_INPUT_MAX_DUTY_CYCLE - _RF_INPUT_NEUTRAL_DUTY_CYCLE));
}

