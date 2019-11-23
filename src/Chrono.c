// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "Timer.h"
#include "stm32f1xx_ll_gpio.h"

// variable privée de type Time qui mémorise la durée mesurée
static Time Chrono_Time; // rem : static rend la visibilité de la variable Chrono_Time limitée à ce fichier 

// variable privée qui mémorise pour le module le timer utilisé par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par défaut au cas où l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// déclaration callback appelé toute les 10ms
void Chrono_Task_10ms(void);

/**
	* @brief  Configure le chronomètre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	// Réglage Timer pour un débordement à 10ms
	Timer_Conf(Chrono_Timer,999, 719);
	
	// Réglage interruption du Timer avec callback : Chrono_Task_10ms()
	Timer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	Timer_IT_Enable(Chrono_Timer);
	
	Chrono_conf_io();
	
}


/**
	* @brief  Démarre le chronomètre. 
  * @note   si la durée dépasse 59mn 59sec 99 Hund, elle est remise à zéro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	Timer_Start(Chrono_Timer);
}


/**
	* @brief  Arrête le chronomètre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	Timer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronomètre à 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arrêt Chrono
	Timer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time privée gérée dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}

void Chrono_conf_io(void)
{
	/* Enable clock */
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	LL_GPIO_InitTypeDef LLGPIO_PIN6_struct,
											LLGPIO_PIN8_struct, 
											LLGPIO_PIN10_struct;

	/* Stop Button */
	LLGPIO_PIN6_struct.Pin 					= LL_GPIO_PIN_6;
  LLGPIO_PIN6_struct.Mode       	= LL_GPIO_MODE_INPUT;
  LLGPIO_PIN6_struct.Speed     		= LL_GPIO_SPEED_FREQ_LOW;
	LLGPIO_PIN6_struct.OutputType		= 0;
	LLGPIO_PIN6_struct.Pull					= LL_GPIO_PULL_DOWN;
	
	/* Start Button */
	LLGPIO_PIN8_struct.Pin 					= LL_GPIO_PIN_8;
  LLGPIO_PIN8_struct.Mode       	= LL_GPIO_MODE_FLOATING;
  LLGPIO_PIN8_struct.Speed      	= LL_GPIO_SPEED_FREQ_HIGH;
	LLGPIO_PIN8_struct.OutputType		= 0;
	LLGPIO_PIN8_struct.Pull					= 0;
	
	/* LED */
	LLGPIO_PIN10_struct.Pin 				= LL_GPIO_PIN_10;
	LLGPIO_PIN10_struct.Mode      	= LL_GPIO_MODE_OUTPUT;
	LLGPIO_PIN10_struct.Speed     	= LL_GPIO_SPEED_FREQ_HIGH;
  LLGPIO_PIN10_struct.OutputType	= LL_GPIO_OUTPUT_PUSHPULL;
	LLGPIO_PIN10_struct.Pull				= 0;
	
	LL_GPIO_Init(GPIOC, &LLGPIO_PIN6_struct);
	LL_GPIO_Init(GPIOC, &LLGPIO_PIN8_struct);
	LL_GPIO_Init(GPIOC, &LLGPIO_PIN10_struct);
	
	/* Clear the LED bit */
	LL_GPIO_WriteReg(GPIOC, ODR, GPIOC->ODR & (~(0x1 << 10)));
}

void Chrono_Background()
{	
	static char state;
	if (READ_BIT(LL_GPIO_ReadReg(GPIOC, IDR), (0x1 << 8))) {
		if (!state) {
			Chrono_Start();
			state = 1-state;
		}
	}
	
	else if (READ_BIT(LL_GPIO_ReadReg(GPIOC, IDR), (0x1 << 6))) {
		if (state) {
			Chrono_Stop();
			state = 1 - state;
		}
	}
}


/**
	* @brief  incrémente la variable privée Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	

	
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
		if (Chrono_Time.Sec % 2){
			LL_GPIO_WriteReg(GPIOC, ODR, GPIOC->ODR | (0x1 << 10));
		} else {
			LL_GPIO_WriteReg(GPIOC, ODR, GPIOC->ODR & (~(0x1 << 10)));
		}
		
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	
}


