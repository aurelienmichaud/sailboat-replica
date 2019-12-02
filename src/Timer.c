// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 

#include "Timer.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"


// variable pointeur de fonction permettant de mémoriser le callback à appeler depuis
// le handler d'IT
void (*Ptr_ItFct_TIM1)(void); 
void (*Ptr_ItFct_TIM2)(void); 
void (*Ptr_ItFct_TIM3)(void); 
void (*Ptr_ItFct_TIM4)(void); 

void (*Ptr_ItFct_SysTick)(void); 

/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé
  * @note   Fonction à lancer avant toute autre. Le timer n'est pas encore lancé (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void Timer_Conf(TIM_TypeDef * Timer, int Arr, int Psc)
{
	LL_TIM_InitTypeDef LL_Tim_Init_Struct;
	
	
	// Validation horloge locale
	if 			(Timer == TIM1) 		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer == TIM2) 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer == TIM3) 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else if (Timer == TIM4) 		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	// chargement structure Arr, Psc, Up Count
	LL_Tim_Init_Struct.Autoreload					= Arr;
	LL_Tim_Init_Struct.Prescaler					= Psc;
	LL_Tim_Init_Struct.ClockDivision			= LL_TIM_CLOCKDIVISION_DIV1;
	LL_Tim_Init_Struct.CounterMode				= LL_TIM_COUNTERMODE_UP;
	LL_Tim_Init_Struct.RepetitionCounter	= 0;
	
	LL_TIM_Init(Timer,&LL_Tim_Init_Struct);
	
	// Blocage Timer
	LL_TIM_DisableCounter(Timer);
}
/* Same but for SysTick */
void Timer_systick_conf(void)
{
	/**
  * This function configures the Cortex-M SysTick source of the time base.
  * HCLKFrequency HCLK frequency in Hz (can be calculated thanks to RCC helper macro)
  * When a RTOS is used, it is recommended to avoid changing the SysTick
  *         configuration by calling this function, for a delay use rather osDelay RTOS service.
  * Ticks Number of ticks
  * None
  */
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
}

/* Timer configuration in PWM input mode */

void Timer_PWM_input_conf(TIM_TypeDef *timer, int channel, int Arr, int Psc)
{
	
	Timer_Conf(timer, Arr, Psc);
	
	if (channel == 1) {
		/* • Select the active input for TIMx_CCR1: write the CC1S bits to 01 in the TIMx_CCMR1
		register (TI1 selected). */
		timer->CCMR1 |= 0x01;
		timer->CCMR1 &= ~(0x1 << 1);
		
		/* • Select the active polarity for TI1FP1 (used both for capture in TIMx_CCR1 and counter
		clear): write the CC1P to ‘0’ (active on rising edge). */
		
		timer->CCER &= ~(0x1 << 1);
		
		/* Select the active input for TIMx_CCR2: write the CC2S bits to 10 in the TIMx_CCMR1
register (TI1 selected). */
		
		timer->CCMR1 &= ~(0x1 << 8);		
		timer->CCMR1 |= 0x1 << 9;
	
		/* Select the active polarity for TI1FP2 (used for capture in TIMx_CCR2): write the CC2P
bit to ‘1’ (active on falling edge). */
		
		timer->CCER |= (0x1 << 5);
		
		/* Select the valid trigger input: write the TS bits to 101 in the TIMx_SMCR register
(TI1FP1 selected). */
		
		timer->SMCR |= 0x1 << 6;
		timer->SMCR &= ~(0x1<<5);
		timer->SMCR |= 0x1 << 4;
		
		/* Configure the slave mode controller in reset mode: write the SMS bits to 100 in the
TIMx_SMCR register. */

		timer->SMCR |= 0x1 << 2;
		timer->SMCR &= ~(0x1<<1);
		timer->SMCR &= ~(0x1);
	
	/* Enable the captures: write the CC1E and CC2E bits to ‘1 in the TIMx_CCER register. */
	
		timer->CCER |= 0x1;
		timer->CCER |= (0x1 << 4);
		
	} else if (channel == 2) {	
		
		timer->CCMR1 |= 0x1 << 8;
		timer->CCMR1 &= ~(0x1 << 9);
		
		timer->CCER &= ~(0x1 << 5);
		
		timer->CCMR1 |= 0x01 << 1;
		timer->CCMR1 &= ~(0x1);
		
		timer->CCER |= (0x1 << 1);

		timer->SMCR |= 0x1 << 6;
		timer->SMCR |= 0x1 << 5;
		timer->SMCR &= ~(0x1<<4);
		
		
		timer->SMCR |= 0x1 << 2;
		timer->SMCR &= ~(0x1<<1);
		timer->SMCR &= ~(0x1);

		
		timer->CCER |= 0x1 << 8;
		timer->CCER |= (0x1 << 12);
	}
	
	Timer_Start(timer);
}

void Timer_PWM_set_high_level_counter(TIM_TypeDef *timer, int CCR)
{
	if (timer == TIM4) {
		timer->CCR3 = CCR;
	} else if (timer == TIM2) {
		timer->CCR2 = CCR;
	} else if (timer == TIM1) {
		timer->CCR1 = CCR;
	}
}

void Timer_PWM_output_conf(TIM_TypeDef *timer, int Arr, int Psc)
{
	Timer_Conf(timer, Arr, Psc);
	
	if (timer == TIM4) {
			/*
		The PWM mode can be selected independently on each channel (one PWM per OCx
		output) by writing 110 (PWM mode 1) or ‘111 (PWM mode 2) in the OCxM bits in the
		TIMx_CCMRx register.*/
			/* Channel 3 -> CCMR2 */
			timer->CCMR2 |= 0x6 << 4;
			timer->CCMR2 &= ~(0x1 << 4);
			
		/*	You must enable the corresponding preload register by setting the
		OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register by
		setting the ARPE bit in the TIMx_CR1 register.*/
			
			timer->CCMR2 |= 0x1 << 3;
			
			/*
		As the preload registers are transferred to the shadow registers only when an update event
		occurs, before starting the counter, you have to initialize all the registers by setting the UG
		bit in the TIMx_EGR register. */

			timer->EGR |= 0x1;

		/*
		OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register. It
		can be programmed as active high or active low. OCx output is enabled by the CCxE bit in
		the TIMx_CCER register. Refer to the TIMx_CCERx register description for more details.
		In PWM mode (1 or 2), TIMx_CNT and TIMx_CCRx are always compared to determine
		whether TIMx_CCRx=TIMx_CNT or TIMx_CNT=TIMx_CCRx (depending on the direction of
		the counter). However, to comply with the ETRF (OCREF can be cleared by an external
		event through the ETR signal until the next PWM period), the OCREF signal is asserted
		only:
		• When the result of the comparison changes, or
		• When the output compare mode (OCxM bits in TIMx_CCMRx register) switches from
		the “frozen” configuration (no comparison, OCxM=‘000) to one of the PWM modes
		(OCxM=‘110 or ‘111).
		This forces the PWM by software while the timer is running.*/

			timer->CCER |= 0x1 << 8;
			
			/* Arr = 0xff and timer->CCR3 = 0x7F -> 50% high and 50% low */
			timer->CCR3 = 0;

		
	} else if (timer == TIM2) {
		
			/* Channel 2 -> CCMR1 */
			timer->CCMR1 |= 0x6 << 12;
			timer->CCMR1 &= ~(0x1 << 12);
		
			timer->CCMR1 |= 0x1 << 11;
		
			timer->EGR |= 0x1;
		
			timer->CCER |= 0x1 << 4;
		
			timer->CCR2 = 0;
		
	} else if (timer == TIM1) {
		

			/*The PWM mode can be selected independently on each channel (one PWM per OCx
			output) by writing ‘110’ (PWM mode 1) or ‘111’ (PWM mode 2) in the OCxM bits in the
			TIMx_CCMRx register. You must enable the corresponding preload register by setting the
			OCxPE bit in the TIMx_CCMRx register, */
			/* Channel 2 -> CCMR1 */
			timer->CCMR1 |= 0x6 << 4;
			timer->CCMR1 &= ~(0x1 << 4);
			
			timer->CCMR1 |= 0x1 << 3;
			/*and eventually the auto-reload preload register (in
			upcounting or center-aligned modes) by setting the ARPE bit in the TIMx_CR1 register.*/
			timer->CR1 |= 0x1 << 7;

			/*As the preload registers are transferred to the shadow registers only when an update event
			occurs, before starting the counter, you have to initialize all the registers by setting the UG
			bit in the TIMx_EGR register.*/
			timer->EGR |= 0x1;

			/*OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register.*/
			/* It can be programmed as active high or active low. OCx output is enabled by a combination of the CCxE, CCxNE */
			timer->CCER |= 0x1;
			timer->CCER &= ~(0x1 << 1);
			timer->CCER |= 0x1 << 2;
		
			/*, MOE, OSSI and OSSR bits (TIMx_CCER and TIMx_BDTR registers).
			Refer to the TIMx_CCER register description for more details.*/
			timer->BDTR |= 0x1 << 10; /* OSSI */
			timer->BDTR |= 0x1 << 11; /* OSSR */
			timer->BDTR |= 0x1 << 15; /* MOE  */
			
			timer->CCR1 = 0x7f;
	}
	
	/*
		The timer is able to generate PWM in edge-aligned mode or center-aligned mode
		depending on the CMS bits in the TIMx_CR1 register.
	*/
	// Enable edge-aligned PWM mode
	timer->CR1 &= ~(0x1 << 4);
	timer->CR1 &= ~(0x3 << 5); 
	
}

/**
	* @brief  Démarre le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_Start(TIM_TypeDef * Timer)
{
		LL_TIM_EnableCounter(Timer);
}

/**
	* @brief  Arrêt le timer considéré
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_Stop(TIM_TypeDef * Timer)
{
	LL_TIM_DisableCounter(Timer);
}


/**
	* @brief  Configure le Timer considéré en interruption sur débordement.
  * @note   A ce stade, les interruptions ne sont pas validés (voir  MyTimer_IT_Enable )
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				void (*IT_function) (void) : nom (adresse) de la fonction à lancer sur interruption
	*         int Prio : priorité associée à l'interruption
  * @retval None
  */
void Timer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio)
{
	// affectation de la fonction
	if 			(Timer == TIM1) Ptr_ItFct_TIM1 = IT_function;
	else if (Timer == TIM2)	Ptr_ItFct_TIM2 = IT_function;
	else if (Timer == TIM3)	Ptr_ItFct_TIM3 = IT_function;
	else if (Timer == TIM4) Ptr_ItFct_TIM4 = IT_function;

	
	// Blocage IT (il faudra la débloquer voir fct suivante)
	LL_TIM_DisableIT_UPDATE(Timer);
	
	// validation du canal NVIC
	IRQn_Type TIM_irq;
	
	if 			(Timer == TIM1) TIM_irq = TIM1_UP_IRQn;
	else if (Timer == TIM2)	TIM_irq = TIM2_IRQn;
	else if (Timer == TIM3)	TIM_irq = TIM3_IRQn;
	else if (Timer == TIM4)	TIM_irq = TIM4_IRQn;
	
	NVIC_SetPriority(TIM_irq, Prio);
	NVIC_EnableIRQ(TIM_irq);
}

void Timer_systick_IT_Conf(void (*IT_function) (void))
{
	Ptr_ItFct_SysTick = IT_function;
	
	NVIC_SetPriority(SysTick_IRQn, 0);
	NVIC_EnableIRQ(SysTick_IRQn);
}


/**
	* @brief  Autorise les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_IT_Enable(TIM_TypeDef * Timer)
{
		LL_TIM_EnableIT_UPDATE(Timer);
}
/* Same but for SysTick */
void Timer_systick_IT_Enable(void)
{
	LL_SYSTICK_EnableIT();
}


/**
	* @brief  Interdit les interruptions
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Timer_IT_Disable(TIM_TypeDef * Timer)
{
	LL_TIM_DisableIT_UPDATE(Timer);
}	


/*
============ LES INTERRUPTIONS =================================

*/

void TIM1_UP_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM1);
	(*Ptr_ItFct_TIM1)();
}	

void TIM2_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM2);
	(*Ptr_ItFct_TIM2)();
}	

void TIM3_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM3);
	(*Ptr_ItFct_TIM3)();
}	

void TIM4_IRQHandler(void)
{
	// rabaisser le flag d'IT
	LL_TIM_ClearFlag_UPDATE(TIM4);
	(*Ptr_ItFct_TIM4)();
}

void SysTick_Handler(void)
{
	// rabaisser le flag d'IT
	
	(*Ptr_ItFct_SysTick)();
}	
