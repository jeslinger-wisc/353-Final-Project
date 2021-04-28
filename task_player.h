/*
 * task_player.h
 *
 *  Created on: Apr 27, 2021
 *      Author: AR
 */

#ifndef TASK_PLAYER_H_
#define TASK_PLAYER_H_


#include "commonHeaders.h"
#include "task_accel.h"
#include "task_button.h"
#include "task_lcd.h"
#include "task_laser.h"
#include "image.h"


/*
 * Call to detect if the player is hit by the enemy laser.
 * If hit is detected, set the image of the player to NULL and remove player sprite.
 */
void markAsHit(LCD_t* playerImage);


/*
 * Initializes hardware/software resources and creates Player Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */

int initTaskPlayer(void);

/*
 * Tears down hardware/software resources and deletes Player Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */

int killTaskPlayer(void);

#endif /* TASK_PLAYER_H_ */
