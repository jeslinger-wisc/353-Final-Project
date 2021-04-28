/*
 * task_lcd.h
 *
 *  Created on: Apr 26, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_LCD_H_
#define TASK_LCD_H_

#include"commonHeaders.h"
#include "image.h"
#include "task_light.h"

/*
 * Sets the priority of the LCD Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskLCDPriority(uint32_t pLvl);

// TODO
BaseType_t LCDget(LCD_t* LCDimage);

/*
 * "Clears" screen by setting every pixel to black.
 */
void clearScreen(void);

/*
 * Initializes hardware/software resources and creates LCD Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskLCD(void);

/*
 * Tears down hardware/software resources and deletes LCD Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskLCD(void);

#endif /* TASK_LCD_H_ */
