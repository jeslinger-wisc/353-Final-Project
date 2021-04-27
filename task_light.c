/*
 * task_light.c
 *
 *  Created on: Apr 26, 2021
 *      Author: John Eslinger
 */

#include "task_light.h"

// Handle for Light Sensor Task.
static TaskHandle_t Task_Light_Handle = NULL;

// Vars to record the general state of the Light Sensor Task.
static volatile bool IS_LIVE = false;
static volatile bool IS_DARK = false;

/*
 * Light Task:
 * JohnEsl-TODO
 */
static void task_light(void *pvParameters) {
    // Endless Task Loop.
    while (1) {
        // Update the darkness state of the Light Sensor Task.
        if (opt3001_read_lux() <= LIGHT_DARK_THRESH) {
            IS_DARK = true;
        }
        else {
            IS_DARK = false;
        }

        // Wait for next sampling.
        vTaskDelay(pdMS_TO_TICKS(LIGHT_PERIOD_DELAY));
    }
}

/*
 * Sets the priority of the Light Sensor Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskLightPriority(uint32_t pLvl) {
    vTaskPrioritySet(Task_Light_Handle, pLvl);
}

/*
 * Getter method that returns the general state of the light sensor.
 *
 * Returns TRUE if light sensor detects darkness, FALSE otherwise.
 */
bool isDark(void) {
    return IS_DARK;
}

/*
 * Initializes hardware/software resources and creates Light Sensor Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskLight(void) {
    // Allow only one Light Sensor Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Set up hardware.
    i2c_init();
    opt3001_init();

    // Create Button Task.
    BaseType_t result = xTaskCreate(task_light,
                                    "Light Sensor Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Light_Handle
                                    );
    if (result != pdPASS) {
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    IS_DARK = false;
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Light Sensor Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskLight(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Button Task.
    vTaskDelete(Task_Light_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}
