/*
 * task_accel.h
 *
 *  Created on: Apr 23, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_ACCEL_H_
#define TASK_ACCEL_H_

#include "commonHeaders.h"

// Enum to define the possible states of the accelerometer.
typedef enum AccelStates {
    LEFT,
    CENTER,
    RIGHT,
} dir_t;

/*
 * Sets the priority of the Accelerometer Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskAccelPriority(uint32_t pLvl);

/*
 * Getter function to determine the direction the MKII booster pack is
 * being tilted (with regards to the x-axis accelerometer facing the
 * negative y-axis direction).
 *
 * Returns direction MKII booster pack is being tilted
 */
dir_t getDirection(void);

/*
 * Initializes hardware/software resources and creates Accelerometer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskAccel(void);

/*
 * Tears down hardware/software resources and deletes Accelerometer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskAccel(void);

#endif /* TASK_ACCEL_H_ */
