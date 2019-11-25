#include "weather_vane.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

void weather_vane_init()
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);


	
	//GPIO's configuration for both channels and the reset
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_FLOATING);
	
	
	int Arr = (360*4)-1;

	
	//Timer in Encoder mode structure configuration 
	LL_TIM_ENCODER_InitTypeDef My_LL_Tim_Encoder_Init_Struct;
	
	My_LL_Tim_Encoder_Init_Struct.EncoderMode = LL_TIM_ENCODERMODE_X4_TI12;
	My_LL_Tim_Encoder_Init_Struct.IC1Polarity = LL_TIM_IC_POLARITY_RISING;
	My_LL_Tim_Encoder_Init_Struct.IC1ActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	My_LL_Tim_Encoder_Init_Struct.IC1Prescaler = LL_TIM_ICPSC_DIV1; 
	My_LL_Tim_Encoder_Init_Struct.IC1Filter = LL_TIM_IC_FILTER_FDIV1;
	My_LL_Tim_Encoder_Init_Struct.IC2Polarity = LL_TIM_IC_POLARITY_RISING;
	My_LL_Tim_Encoder_Init_Struct.IC2ActiveInput = LL_TIM_ACTIVEINPUT_INDIRECTTI;
	My_LL_Tim_Encoder_Init_Struct.IC2Prescaler = LL_TIM_ICPSC_DIV1;
	My_LL_Tim_Encoder_Init_Struct.IC2Filter = LL_TIM_IC_FILTER_FDIV1;
	
	LL_TIM_ENCODER_Init(TIM3,&My_LL_Tim_Encoder_Init_Struct);
	
	LL_TIM_SetAutoReload(TIM3,Arr);
	
	//Setting the Arr value initialized before

}

void weather_vane_start_counting(){
		LL_TIM_EnableCounter(TIM3);	
	
}


int weather_vane_get_position() {
	
	//We get the counter value for the TIM3
	return LL_TIM_GetCounter(TIM3)/4;
}
