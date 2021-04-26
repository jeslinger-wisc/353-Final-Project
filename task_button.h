/*
 * task_button.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_BUTTON_H_
#define TASK_BUTTON_H_

#include "commonHeaders.h"

/*
 * Getter method that returns de-bounced state of the S2 button on the
 * MKII booster pack.
 *
 * Returns TRUE if pressed down, FALSE otherwise.
 */
bool isButtonPressed(void);

/*
 * Initializes hardware/software resources and creates Button Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskButton(void);

/*
 * Tears down hardware/software resources and deletes Button Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskButton(void);

#endif /* TASK_BUTTON_H_ */
