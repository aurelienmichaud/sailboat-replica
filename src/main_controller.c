#include "main_controller.h"

#include "Timer.h"
#include "servo.h"
#include "rf_input.h"
#include "weather_vane.h"

#include "stm32f1xx_ll_gpio.h"

#define SERVO_TIMER 						TIM1

#define RF_INPUT_TIMER_CHANNEL 	1

#define MAIN_CONTROLLER_TIMER		SysTick
#define MAIN_CONTROLLER_TIMER_ARR	999
#define MAIN_CONTROLLER_TIMER_PSC 719
#define MAIN_CONTROLLER_TIMER_INTERRUPT_PRIORITY	3

static void main_controller_init_all(void)
{
	servo_init(SERVO_TIMER);
	
	rf_input_init(RF_INPUT_TIMER_CHANNEL);
	
	/* Weahter Vane */
	weather_vane_init();
	
	LL_TIM_SetCounter(TIM3, 0);
	while((LL_GPIO_ReadInputPort(GPIOA) & ((0x1)<<5)) == 0){
		/* NOTHING */
	}
	
	weather_vane_start_counting();
	
	/* END OF WEATHER VANE */
}

/* Interrupt callback */
static void main_controller_task(void);

static void main_controller_init(void)
{
	main_controller_init_all();
	
	// Configure timer counter if timer = TIM1, TIM2, TIM3 or TIM4
	/* Timer_Conf(MAIN_CONTROLLER_TIMER,
							MAIN_CONTROLLER_TIMER_ARR,
							MAIN_CONTROLLER_TIMER_PSC);*/
	
	// Put main_controller_task() as a callback function for the MAIN_CONTROLLER_TIMER interrupt
	/*Timer_IT_Conf(MAIN_CONTROLLER_TIMER,
								main_controller_task,
								MAIN_CONTROLLER_TIMER_INTERRUPT_PRIORITY);*/
	
	// Enable IT
	/* Timer_IT_Enable(MAIN_CONTROLLER_TIMER); */
	
	Timer_systick_conf();
	Timer_systick_IT_Conf(main_controller_task);
	Timer_systick_IT_Enable();
	
	SysTick->LOAD = 720000;
}

void main_controller_start(void)
{
	main_controller_init();
	
	servo_setAngle(150);
	
	/*Timer_Start(MAIN_CONTROLLER_TIMER);*/
}
/* Interrupt callback */
void main_controller_task(void)
{
	int angle = weather_vane_get_position();
	
	servo_setAngle(angle);
	
}

