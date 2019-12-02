#include "weather_vane.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

static TIM_TypeDef *WeatherVaneTimer;

static void weather_vane_init(void);

void weather_vane_start(TIM_TypeDef *__WeatherVaneTimer)
{
	WeatherVaneTimer = __WeatherVaneTimer;
	
	weather_vane_init();
	
	/* NEED TO FIND MANUALLY THE ABSOLUTE 0 POSITION OF THE WEATHER VANE; until then -> infinite loop */
	LL_TIM_SetCounter(WeatherVaneTimer, 0);
	while((LL_GPIO_ReadInputPort(GPIOA) & ((0x1)<<5)) == 0){
		/* NOTHING */
	}
	
	LL_TIM_EnableCounter(WeatherVaneTimer);	
}

#define _WEATHER_VANE_ARR	((360*4)-1)

static void weather_vane_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	
	// Validation horloge locale
	if 			(WeatherVaneTimer == TIM1) 		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (WeatherVaneTimer == TIM2) 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (WeatherVaneTimer == TIM3) 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else if (WeatherVaneTimer == TIM4) 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	// GPIO's configuration for both channels and the reset
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_FLOATING);

	// Timer in Encoder mode structure configuration 
	LL_TIM_ENCODER_InitTypeDef My_LL_Tim_Encoder_Init_Struct;
	
	My_LL_Tim_Encoder_Init_Struct.EncoderMode 		= LL_TIM_ENCODERMODE_X4_TI12;
	My_LL_Tim_Encoder_Init_Struct.IC1Polarity 		= LL_TIM_IC_POLARITY_RISING;
	My_LL_Tim_Encoder_Init_Struct.IC1ActiveInput	= LL_TIM_ACTIVEINPUT_DIRECTTI;
	My_LL_Tim_Encoder_Init_Struct.IC1Prescaler		= LL_TIM_ICPSC_DIV1; 
	My_LL_Tim_Encoder_Init_Struct.IC1Filter 			= LL_TIM_IC_FILTER_FDIV1;
	My_LL_Tim_Encoder_Init_Struct.IC2Polarity 		= LL_TIM_IC_POLARITY_RISING;
	My_LL_Tim_Encoder_Init_Struct.IC2ActiveInput 	= LL_TIM_ACTIVEINPUT_INDIRECTTI;
	My_LL_Tim_Encoder_Init_Struct.IC2Prescaler 		= LL_TIM_ICPSC_DIV1;
	My_LL_Tim_Encoder_Init_Struct.IC2Filter 			= LL_TIM_IC_FILTER_FDIV1;
	
	LL_TIM_ENCODER_Init(WeatherVaneTimer, &My_LL_Tim_Encoder_Init_Struct);
	
	LL_TIM_SetAutoReload(WeatherVaneTimer, _WEATHER_VANE_ARR);
}

int weather_vane_get_position(void)
{
	// Get the counter value for the WeatherVaneTimer
	return LL_TIM_GetCounter(WeatherVaneTimer)/4;
}
