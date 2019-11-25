#ifndef _RF_INPUT_H_
#define _RF_INPUT_H_

/* Servo Engine can only work with TIM1 or TIM4 */
void rf_input_init(int channel);

int rf_input_getAngle(void);


#endif // _RF_INPUT_H_

