/*
 * task_laser.h
 *
 *  Created on: Apr 27, 2021
 *      Author: AR
 */

#ifndef TASK_LASER_H_
#define TASK_LASER_H_

#include "commonHeaders.h"
#include "task_enemy.h"
#include "task_lcd.h"
#include "task_player.h"

// Struct to define laser sprites.
typedef struct LaserType {
    LCD_t image;
    bool goingUp;
} laser_t;

/*
 * Queues given laser to be displayed/moved on screen.
 *
 * laser- laser to be queued to be displayed on screen
 *
 * Returns TRUE is laser queued, FALSE otherwise.
 */
bool queueLaser(laser_t* laser);

/*
 * Gets the laser information related to the specified index.
 * Note: Returns ptr, allowing writing to the laser's info.
 *
 * index- index of laser to get
 *
 * Returns ptr to laser at the specified index
 */
volatile laser_t* getLaserInfo(int index);

/*
 * Initializes hardware/software resources and creates Laser Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskLaser(void);

/*
 * Tears down hardware/software resources and deletes Laser Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskLaser(void);

#endif /* TASK_LASER_H_ */
