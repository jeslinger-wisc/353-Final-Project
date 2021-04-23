/*
 * task_buzzer.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_BUZZER_H_
#define TASK_BUZZER_H_

// Standard includes for ece353 firmware.
#include "msp.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// RTOS header files.
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

// Application header files.
#include "config.h"
#include "music.h"

/*
 * Initialization function used to initialize hardware resources to use the
 * buzzer on the MKII booster pack.
 */
void initTaskBuzzer(void);

/*
 * Queues given melody to be played on the buzzer.
 *
 * melody- melody to queue to be played on buzzer.
 *
 * Returns pdPASS if melody queued, errQUEUE_FULL otherwise.
 */
BaseType_t playMelody(melody_t* melody);

// Function handle and declaration for Buzzer Task.
extern TaskHandle_t Task_Buzzer_Handle;
void task_buzzer(void *pvParameters);

#endif /* TASK_BUZZER_H_ */
