#include "rf_input.h"
#include "Timer.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

#define _RF_INPUT_TIMER	TIM4
#define _RF_INPUT_ARR		4999
#define _RF_INPUT_PSC		(72000000 / ((_RF_INPUT_ARR + 1) * 50) - 1)

#define _RF_INPUT_ANGLE_RANGE	10.0

#define _RF_INPUT_MAX_DUTY_CYCLE			(((double)_RF_INPUT_ARR / 10.0))
#define _RF_INPUT_NEUTRAL_DUTY_CYCLE	(((double)_RF_INPUT_ARR * 3.0 / 40.0))

static TIM_TypeDef *RFInputTimer;
static int RFInputChannel;

/* RF Input only works with PB6-TIM4CH1 or PB7-TIM4CH2 */
void rf_input_init(int channel)
{
	/* Servo Engine on PIN8 */
	LL_GPIO_InitTypeDef LLGPIO_struct;

	RFInputTimer = _RF_INPUT_TIMER;
	RFInputChannel = channel;
	
	/* Timer configuration in PWM input mode */
	Timer_PWM_input_conf(RFInputTimer,
											channel,
											_RF_INPUT_ARR,
											_RF_INPUT_PSC);
	
	if (channel == 1) {

		LLGPIO_struct.Pin = LL_GPIO_PIN_6;
		
	} else if (channel == 2) {

		LLGPIO_struct.Pin = LL_GPIO_PIN_7;
	}
	
	/* TIM1 and TIM4 will both use the eighth pin of respectively GPIOA and GPIOB */
	LLGPIO_struct.Mode       		= LL_GPIO_MODE_FLOATING;
	
	/* Enable clock */
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	LL_GPIO_Init(GPIOB, &LLGPIO_struct);
}

int rf_input_getAngle(void)
{
	int val;
	
	if (RFInputChannel == 1)
		val = RFInputTimer->CCR2;
	
	else if(RFInputChannel == 2)
		val = RFInputTimer->CCR1;
		
	return (int)((val - _RF_INPUT_NEUTRAL_DUTY_CYCLE) * _RF_INPUT_ANGLE_RANGE / (_RF_INPUT_MAX_DUTY_CYCLE - _RF_INPUT_NEUTRAL_DUTY_CYCLE));
}

