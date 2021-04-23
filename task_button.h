/*
 * task_button.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_BUTTON_H_
#define TASK_BUTTON_H_

// Standard includes for ece353 firmware.
#include "msp.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// RTOS header files.
#include <FreeRTOS.h>
#include <task.h>

// Application header files
#include "config.h"

#include "task_menu.h"

/*
 * Initialization function used to initialize hardware resources to use the S2
 * button on the MKII booster pack.
 */
void initTaskButton(void);

/*
 * Getter function to determine if the S2 button on the MKII booster pack is
 * pressed down or not (based on debounced state).
 *
 * TRUE if pressed down, FALSE otherwise.
 */
bool isButtonPressed(void);

// Function handle and declaration for Button Task.
extern TaskHandle_t Task_Button_Handle;
void task_button(void *pvParameters);



#endif /* TASK_BUTTON_H_ */
