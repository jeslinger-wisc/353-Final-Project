/*
/*
 *  task_player.h
 *
 *  Created on: Apr 26, 2021
 *  Author: Arman Ramiz
 */

#ifndef TASK_PLAYER_H_
#define TASK_PLAYER_H_

#include <main.h>

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

#include <space_ship.h>
#include <task_button.h>
#include <task_lcd.h>
#include <timer32.h>


extern TaskHandle_t Task_Player_Handle;

/******************************************************************************
 * This function will initialize Queue_LCD  and initialize the LCD
 ******************************************************************************/
void Task_Space_Ship_Init(void);

/******************************************************************************
 * This task manages the movement of the player controlled image. It takes notifications
 * from the accelerometer, updates the position of the player sprite, and send that data
 * to the lcd task.
 ******************************************************************************/
void Task_Player(void *pvParameters);



#endif /* TASK_PLAYER_H_ */
