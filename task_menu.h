/*
 * task_menu.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_MENU_H_
#define TASK_MENU_H_

#include "commonHeaders.h"

// TODO remove as needed
#include "task_button.h"
#include "task_accel.h"
#include "task_buzzer.h"
#include "task_lcd.h"
#include "task_light.h"


/*
 * Switches application back to "control" mode (assumably from "game"
 * mode), waking up the Menu Task as needed and passing it the provided
 * result.
 *
 * playerWon- result of assumed game, should be TRUE if player won the game.
 */
void switchToControlMode(bool playerWon);

/*
 * Initializes hardware/software resources and creates Menu Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskMenu(void);

/*
 * Tears down hardware/software resources and deletes Menu Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskMenu(void);

#endif /* TASK_MENU_H_ */
