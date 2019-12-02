#ifndef _TIMER_H
#define _TIMER_H

/* Driver for Timer 1 to 4 and SysTick of STM32F103RB */

#include "stm32f103xb.h" 

/**
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis�
  * @note   Fonction � lancer avant toute autre. Le timer n'est pas encore lanc� (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur � placer dans ARR
	*					int Psc   : valeur � placer dans PSC
  * @retval None
  */
void Timer_Conf(TIM_TypeDef * Timer,int Arr, int Psc);
/* Same but for SysTick */
void Timer_systick_conf(void);

/* Timer configuration in PWM input/output mode */
void Timer_PWM_input_conf(TIM_TypeDef *timer, int channel, int Arr, int Psc);
void Timer_PWM_output_conf(TIM_TypeDef *timer, int Arr, int Psc);
/* The the upper ratio with CRR compared to ARR in PWM */
void Timer_PWM_set_high_level_counter(TIM_TypeDef *timer, int CCR);

/**
	* @brief  D�marre le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_Start(TIM_TypeDef * Timer);

/**
	* @brief  Arr�t le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_Stop(TIM_TypeDef * Timer);

/**
	* @brief  Configure le Timer consid�r� en interruption sur d�bordement.
  * @note   A ce stade, les interruptions ne sont pas valid�s (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction � lancer sur interruption
	*         int Prio : priorit� associ�e � l'interruption
  * @retval None
  */
void Timer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio);
/* Same but for SysTick; priority set to 0 */
void Timer_systick_IT_Conf(void (*IT_function) (void));

/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_IT_Enable(TIM_TypeDef * Timer);
/* Same but for SysTick */
void Timer_systick_IT_Enable(void);

/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_IT_Disable(TIM_TypeDef * Timer);

#endif //_TIMER_H
