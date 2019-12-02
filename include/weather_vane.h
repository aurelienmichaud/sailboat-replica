#ifndef _WEATHER_VANE_H
#define _WEATHER_VANE_H

#include "stm32f1xx_ll_tim.h"

void weather_vane_start(TIM_TypeDef *__WeatherVaneTimer);

int weather_vane_get_position(void);

#endif // _WEATHER_VANE_H
