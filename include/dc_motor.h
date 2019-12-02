#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include "stm32f1xx_ll_tim.h"

/* DC Motor can only work with TIM2 channel 2 */
void dc_motor_start(TIM_TypeDef *_DCMotorTimer);

void dc_motor_move(int angle);

#endif //_DC_MOTOR_H_
