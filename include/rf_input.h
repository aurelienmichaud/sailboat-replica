#ifndef _RF_INPUT_H_
#define _RF_INPUT_H_

#include "stm32f1xx_ll_tim.h"

#define _RF_INPUT_ANGLE_RANGE	10.0

/* RF Input Module can only work with TIM4 */
void rf_input_start(TIM_TypeDef *RFInputTimer, int channel);

/* Returns an angle in [ - _RF_INPUT_ANGLE_RANGE ; _RF_INPUT_ANGLE_RANGE] */
int rf_input_get_angle(void);

#endif // _RF_INPUT_H_

