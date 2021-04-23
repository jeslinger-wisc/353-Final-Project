/*
 * menu_task.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef MENU_TASK_H_
#define MENU_TASK_H_

// Standard includes for ece353 firmware.
#include "msp.h"
#include <stdint.h>
#include <stdio.h>

// RTOS header files.
#include <FreeRTOS.h>
#include <task.h>

// Function handle and declaration for Menu Task.
extern TaskHandle_t Menu_Task_Handle;
void menu_task(void *pvParameters);

#endif /* MENU_TASK_H_ */
