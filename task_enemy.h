/*
 * task_enemy.h
 *
 *  Created on: Apr 27, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_ENEMY_H_
#define TASK_ENEMY_H_

#include <stdlib.h>

#include "commonHeaders.h"
#include "task_buzzer.h"
#include "task_lcd.h"
#include "task_menu.h"

/*
 * Initializes hardware/software resources and creates Enemy Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskEnemy(void);

/*
 * Tears down hardware/software resources and deletes Enemy Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskEnemy(void);

#endif /* TASK_ENEMY_H_ */
