/*
 * task_menu.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_MENU_H_
#define TASK_MENU_H_

// Standard includes for ece353 firmware.
#include "msp.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// RTOS header files.
#include <FreeRTOS.h>
#include <task.h>

// Application header files.
#include "config.h"
#include "task_button.h"

// Function handle and declaration for Menu Task.
extern TaskHandle_t Task_Menu_Handle;
void task_menu(void *pvParameters);

extern int CHECK;

#endif /* TASK_MENU_H_ */
