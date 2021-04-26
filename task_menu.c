/*
 * task_menu.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_menu.h"

// Handle for Menu Task.
static TaskHandle_t Task_Menu_Handle = NULL;

// Vars to record the general state of the Menu Task.
static volatile bool IS_LIVE = false;
static volatile bool IS_AWAKE = false;

/*
 * Menu Task:
 * JohnEsl-TODO
 */
static void task_menu(void *pvParameters) {
    // set direction as an output
    P2->DIR |= BIT2;

    // Turn off LED
    P2->OUT &= ~BIT2;

    // Endless Task Loop.
    while (1) {
        if (isDark()) {
            P2->OUT |= BIT2;
        }
        else {
            P2->OUT &= ~BIT2;
        }
    }
}

/*
 * Switches application back to "control" mode (assumably from "game"
 * mode), waking up the Menu Task as needed and passing it the provided
 * result.
 *
 * playerWon- result of assumed game, should be TRUE if player won the game.
 */
void switchToControlMode(bool playerWon) {
    // Ensure Menu Task can/should be switched to.
    if (IS_AWAKE || !(IS_LIVE)) {
        return;
    }

    // Ensure Menu Task is at high enough priority.
    vTaskPrioritySet(Task_Menu_Handle, CONTROL_PRIORITY);

    // Notify/Wakeup Menu Task.
    xTaskNotify(Task_Menu_Handle, playerWon, eSetValueWithOverwrite);
}

/*
 * Initializes hardware/software resources and creates Menu Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskMenu(void) {
    // Allow only one Menu Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Create Menu Task.
    BaseType_t result = xTaskCreate(task_menu,
                                    "Menu Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Menu_Handle
                                    );
    if (result != pdPASS) {
        return -1;
    }

    // Return for successful setup.
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Menu Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskMenu(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Menu Task.
    vTaskDelete(Task_Menu_Handle);

    // Return for successful deconstruction.
    return 0;
}
