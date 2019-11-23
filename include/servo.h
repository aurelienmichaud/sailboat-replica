#ifndef _SERVO_H_
#define _SERVO_H_

#include "stm32f1xx_ll_tim.h"

/* Servo Engine can only work with TIM1 or TIM4 */
void servo_init(TIM_TypeDef *timer);
void servo_setAngle(int pos);


#endif // _SERVO_H_

