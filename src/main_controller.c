#include "main_controller.h"

#include "Timer.h"
#include "servo.h"
#include "rf_input.h"
#include "weather_vane.h"
#include "dc_motor.h"

#include "stm32f1xx_ll_gpio.h"

/* Servo motor : TIM1 or TIM4 (the channel is automatically set to 2 for TIM1 and to 3 for TIM4) */
#define SERVO_TIMER TIM1

/* RF Input Module needs to use TIM4, only need to set the channel */
#define RF_INPUT_TIMER					TIM4
#define RF_INPUT_TIMER_CHANNEL 	1

#define WEATHER_VANE_TIMER	TIM3

#define DC_MOTOR_TIMER	TIM2

/* Because all the timer are used, the main controller will used SysTick */
#define MAIN_CONTROLLER_TIMER											SysTick
#define MAIN_CONTROLLER_TIMER_ARR									999
#define MAIN_CONTROLLER_TIMER_PSC 								719
#define MAIN_CONTROLLER_TIMER_INTERRUPT_PRIORITY	3

static void main_controller_start_all(void)
{
	servo_start(SERVO_TIMER);
	
	rf_input_start(RF_INPUT_TIMER, RF_INPUT_TIMER_CHANNEL);
	
	/* CAREFULL : The weather vane needs to set the 0 position
		by physically and manually turning the weather vane 360 degree */
	weather_vane_start(WEATHER_VANE_TIMER);
	
	/* DC Motor can only work with TIM2 channel 2 */
	dc_motor_start(DC_MOTOR_TIMER);
}

/* Interrupt callback */
static void main_controller_task(void);

static void main_controller_init(void)
{
	main_controller_start_all();

	/* If MAIN_CONTROLLER_TIMER is SysTick */
	Timer_systick_conf();
	Timer_systick_IT_Conf(main_controller_task);
	Timer_systick_IT_Enable();
	
	SysTick->LOAD = 720000;
	
	/* If MAIN_CONTROLLER_TIMER is TIM1, TIM2, TIM3 or TIM4 */
	/*
	// Configure timer counter if timer = TIM1, TIM2, TIM3 or TIM4
	Timer_Conf(MAIN_CONTROLLER_TIMER,
							MAIN_CONTROLLER_TIMER_ARR,
							MAIN_CONTROLLER_TIMER_PSC);
		
	// Put main_controller_task() as a callback function for the MAIN_CONTROLLER_TIMER interrupt
	Timer_IT_Conf(MAIN_CONTROLLER_TIMER,
								main_controller_task,
								MAIN_CONTROLLER_TIMER_INTERRUPT_PRIORITY);
		
	// Enable IT
	Timer_IT_Enable(MAIN_CONTROLLER_TIMER);
	Timer_Start(MAIN_CONTROLLER_TIMER);
	*/
	
	
}

void main_controller_start(void)
{
	main_controller_init();
}


/* Interrupt callback */
void main_controller_task(void)
{
	int angle;

	/* Sail's angle setting */
	angle = weather_vane_get_position();
	servo_setAngle(angle);
	
	/* Base board movements */
	angle = rf_input_get_angle();
	dc_motor_move(angle);
}

