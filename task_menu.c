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
 * Helper function that plays the main game. Switches application
 * from "control" mode to "game" mode, waits for switchToControlMode()
 * to be called, then finishes switch back to "control" mode
 *
 * Returns TRUE if player won the played game.
 */
static bool playGame(void) {
    // Var to store the result of the game.
    uint32_t playerWon;

    // Create game tasks.
    initTaskEnemy();

    // Lower priority of IO tasks to game priority.
    setTaskAccelPriority(GAME_PRIORITY);
    setTaskButtonPriority(GAME_PRIORITY);
    setTaskBuzzerPriority(GAME_PRIORITY);
    setTaskLCDPriority(GAME_PRIORITY);
    setTaskLightPriority(GAME_PRIORITY);

    // Put Menu task to sleep- restarts here after wakeup.
    IS_AWAKE = false;
    xTaskNotifyWait(0, 0, &playerWon, portMAX_DELAY);
    IS_AWAKE = true;

    // Delete game tasks.
    killTaskEnemy();

    // Pull up IO task priorities to use in control mode.
    setTaskAccelPriority(CONTROL_PRIORITY);
    setTaskButtonPriority(CONTROL_PRIORITY);
    setTaskBuzzerPriority(CONTROL_PRIORITY);
    setTaskLCDPriority(CONTROL_PRIORITY);
    setTaskLightPriority(CONTROL_PRIORITY);

    // Return the game's outcome.
    return (playerWon > 0);
}

/*
 * Menu Task:
 * JohnEsl-TODO
 */
static void task_menu(void *pvParameters) {
    // Display starting screen.
    LCD_t title = titleImage();
    LCD_t begin = beginImage();
    clearScreen();
    LCDget(&title);
    LCDget(&begin);

    // Endless Task Loop.
    while (1) {
        // Wait for starting button press.
        while (!isButtonPressed()) {};

        // Clear screen for gameplay.
        clearScreen();

        // Play game.
        bool playerWon = playGame();

        // Display appropriate message.
        clearScreen();
        LCDget(&title); // Saved from initialization
        if (playerWon) {
            LCD_t won = winImage();
            LCDget(&won);
        }
        else {
            LCD_t lost = loseImage();
            LCDget(&lost);
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
    uint32_t txValue = (playerWon) ? 1 : 0;
    xTaskNotify(Task_Menu_Handle, txValue, eSetValueWithOverwrite);
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
    IS_LIVE = true;
    IS_AWAKE = true;
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
    IS_LIVE = false;
    IS_AWAKE = false;
    return 0;
}
