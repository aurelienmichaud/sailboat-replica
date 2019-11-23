#include "rf_input.h"
#include "Timer.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

static TIM_TypeDef *RFInputTimer;

/* RF Input only works with PB6-TIM4CH1 or PB7-TIM4CH2 */
void rf_input_init(int channel)
{
	/* Servo Engine on PIN8 */
	LL_GPIO_InitTypeDef LLGPIO_struct;

	RFInputTimer = TIM4;
	
	/* Timer configuration in PWM input mode */
	Timer_PWM_input_conf(RFInputTimer, channel, 5624, 255);
	
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


