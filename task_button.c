/*
 * task_button.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_button.h"

// Values used to de-bounce button.
#define DB_SHIFT (32 - BUTTON_DEBOUNCE_VALUE)
#define DB_MASK (0xFFFFFFFF << DB_SHIFT)

// Handle for Button Task.
static TaskHandle_t Task_Button_Handle = NULL;

// Vars to record the general state of the Button Task.
static volatile bool IS_LIVE = false;
static volatile bool IS_PRESSED = false;

/*
 * Button Task: Debounces the S2 button and updates an system-wide
 * viewable variable that conveys the de-bounced status of the button. Sleeps
 * between debounces according to configurable time variable.
 */
static void task_button(void *pvParameters) {
    // Var to store last 32 samples.
    uint32_t samples = 0;

    // Endless Task Loop.
    while (1) {
        // Shift in button sample.
        samples = (samples << 1) | (!(P3->IN & BIT5));

        // Set state of button.
        IS_PRESSED = (samples << DB_SHIFT) & DB_MASK;

        // Wait for next sampling.
        vTaskDelay(pdMS_TO_TICKS(BUTTON_PERIOD_DELAY));
    }
}

/*
 * Sets the priority of the Button Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskButtonPriority(uint32_t pLvl) {
    vTaskPrioritySet(Task_Button_Handle, pLvl);
}

/*
 * Getter method that returns de-bounced state of the S2 button on the
 * MKII booster pack.
 *
 * Returns TRUE if pressed down, FALSE otherwise.
 */
bool isButtonPressed(void) {
    return IS_PRESSED;
}

/*
 * Initializes hardware/software resources and creates Button Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskButton(void) {
    // Allow only one Button Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Set as input.
    P3->DIR &= ~BIT5;

    // Create Button Task.
    BaseType_t result = xTaskCreate(task_button,
                                    "Button Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Button_Handle
                                    );
    if (result != pdPASS) {
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    IS_PRESSED = false;
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Button Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskButton(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Button Task.
    vTaskDelete(Task_Button_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}
