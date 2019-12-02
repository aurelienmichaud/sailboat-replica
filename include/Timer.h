#ifndef _TIMER_H
#define _TIMER_H

/* Driver for Timer 1 to 4 and SysTick of STM32F103RB */

#include "stm32f103xb.h" 

/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé
  * @note   Fonction à lancer avant toute autre. Le timer n'est pas encore lancé (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
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
	* @brief  Démarre le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_Start(TIM_TypeDef * Timer);

/**
	* @brief  Arrêt le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_Stop(TIM_TypeDef * Timer);

/**
	* @brief  Configure le Timer considéré en interruption sur débordement.
  * @note   A ce stade, les interruptions ne sont pas validés (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction à lancer sur interruption
	*         int Prio : priorité associée à l'interruption
  * @retval None
  */
void Timer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio);
/* Same but for SysTick; priority set to 0 */
void Timer_systick_IT_Conf(void (*IT_function) (void));

/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_IT_Enable(TIM_TypeDef * Timer);
/* Same but for SysTick */
void Timer_systick_IT_Enable(void);

/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_IT_Disable(TIM_TypeDef * Timer);

#endif //_TIMER_H
