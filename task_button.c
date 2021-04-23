/*
 * task_button.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_button.h"

// Handle for Button Task.
TaskHandle_t Task_Button_Handle = NULL;

// Overall/Debounced state of the S2 button.
static volatile bool IS_PRESSED = false;

/*
 * Initialization function used to initialize hardware resources to use the S2
 * button on the MKII booster pack.
 */
void initTaskButton(void) {
    // Set as input.
    P3->DIR &= ~BIT5;
}

/*
 * Getter function to determine if the S2 button on the MKII booster pack is
 * pressed down or not (based on debounced state).
 *
 * TRUE if pressed down, FALSE otherwise.
 */
bool isButtonPressed(void) {
    return IS_PRESSED;
}

/*
 * Function for Button Task.
 * JohnEsl-TODO
 */
void task_button(void *pvParameters) {
    // Count consecutive pressed samples.
    uint32_t pressedInRow = 0;

    // Endless Task Loop.
    while(1) {
        // Sample button/set count.
        pressedInRow = (!(P3->IN & BIT5)) ? pressedInRow + 1 : 0;

        // Set state of button.
        IS_PRESSED = (pressedInRow >= BUTTON_DEBOUNCE_VALUE);

        // Wait for next sampling.
        vTaskDelay(pdMS_TO_TICKS(CHECK));
    }
}


