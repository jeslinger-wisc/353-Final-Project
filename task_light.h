/*
 * task_light.h
 *
 *  Created on: Apr 26, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_LIGHT_H_
#define TASK_LIGHT_H_

#include "commonHeaders.h"
#include "i2c.h"
#include "opt3001.h"

/*
 * Sets the priority of the Light Sensor Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskLightPriority(uint32_t pLvl);

/*
 * Getter method that returns the general state of the light sensor.
 *
 * Returns TRUE if light sensor detects darkness, FALSE otherwise.
 */
bool isDark(void);

/*
 * Initializes hardware/software resources and creates Light Sensor Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskLight(void);

/*
 * Tears down hardware/software resources and deletes Light Sensor Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskLight(void);

#endif /* TASK_LIGHT_H_ */
