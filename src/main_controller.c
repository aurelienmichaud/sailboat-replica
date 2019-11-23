#include "main_controller.h"

#include "Timer.h"
#include "servo.h"
#include "rf_input.h"

#include <stdio.h>

#define SERVO_TIMER 						TIM1

#define RF_INPUT_TIMER_CHANNEL 	1

#define MAIN_CONTROLLER_TIMER		TIM3
#define MAIN_CONTROLLER_TIMER_ARR	999
#define MAIN_CONTROLLER_TIMER_PSC 719
#define MAIN_CONTROLLER_TIMER_INTERRUPT_PRIORITY	3

static void main_controller_init_all(void)
{
	servo_init(SERVO_TIMER);
	
	rf_input_init(RF_INPUT_TIMER_CHANNEL);
}

/* Interrupt callback */
static void main_controller_task(void);

static void main_controller_init(void)
{
	main_controller_init_all();
	
	// Configure timer counter
	Timer_Conf(MAIN_CONTROLLER_TIMER,
							MAIN_CONTROLLER_TIMER_ARR,
							MAIN_CONTROLLER_TIMER_PSC);
	
	// Put main_controller_task() as a callback function for the MAIN_CONTROLLER_TIMER interrupt
	Timer_IT_Conf(MAIN_CONTROLLER_TIMER,
								main_controller_task,
								MAIN_CONTROLLER_TIMER_INTERRUPT_PRIORITY);
	
	// Enable IT
	Timer_IT_Enable(MAIN_CONTROLLER_TIMER);
}

void main_controller_start(void)
{
	main_controller_init();
	
	//servo_setAngle(150);
	
	Timer_Start(MAIN_CONTROLLER_TIMER);
}

/* Interrupt callback */
void main_controller_task(void)
{
	/* TESTS */
	/*
	static int a;
	a += 5;
	servo_setAngle(a % 360);
	*/
}

