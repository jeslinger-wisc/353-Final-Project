/*
 * task_player.c
 *
 *  Created on: Apr 27, 2021
 *      Author: AR
 */

#include "task_laser.h"

// Handle for Laser Task.
static TaskHandle_t Task_Laser_Handle;

// Queue of lasers to display/move.
static QueueHandle_t Queue_Laser;

// Vars to know the general state of the Laser Task.
static volatile bool IS_LIVE = false;
static volatile laser_t lasers[LASER_COUNT];

/*
 * Function for Laser Task.
 *
 */
void task_laser(void *pvParameters) {
    // Initialize array of lasers to be empty.
    int index = 0;
    for (; index < LASER_COUNT; index++) {
        // Set initial details.
        lasers[index].image = newLaserImage();

        // Set as empty.
        lasers[index].image.image = NULL;
    }

    // Endless Task Loop.
    while (1) {
        // Update all current lasers.
        int index = 0;
        for (; index < LASER_COUNT; index++) {
            // Skip if not active.
            if (lasers[index].image.image == NULL) {
                continue;
            }

            // Adjust laser coordinates.
            lasers[index].image.y += (lasers[index].goingUp) ? 1 : -1;

            // Nullify laser if at height limit.
            int laserYRadius = (newLaserImage().image_height_pixels / 2) + 1;
            if ((lasers[index].image.y - laserYRadius) <= 0) {
                LCD_t img = lasers[index].image; // convert to non-volatile.
                img.fColor = LCD_COLOR_BLACK;
                LCDget(&img);
                lasers[index].image.image = NULL;
            }
            else if ((lasers[index].image.y + laserYRadius) >= LCD_VERTICAL_MAX) {
                LCD_t img = lasers[index].image; // convert to non-volatile.
                img.fColor = LCD_COLOR_BLACK;
                LCDget(&img);
                lasers[index].image.image = NULL;
            }

            // Display laser.
            LCD_t img = lasers[index].image; // convert to non-volatile.
            LCDget(&img);
        }

        // Attempt to queue in new laser.
        laser_t newLaser;
        if (xQueueReceive(Queue_Laser, &newLaser, 0) == pdPASS) {
            // Find open spot for laser.
            int index = 0;
            for (; index < LASER_COUNT; index++) {
                // Check if index is empty.
                if (lasers[index].image.image == NULL) {
                    lasers[index] = newLaser; // fill
                    LCDget(&(newLaser.image)); // display
                    break;
                }
            }
        }

        // Delay till next cycle.
        vTaskDelay(pdMS_TO_TICKS(LASER_PERIOD_DELAY));
    }
}

/*
 * Gets the laser information related to the specified index.
 * Note: Returns ptr, allowing writing to the laser's info.
 *
 * index- index of laser to get
 *
 * Returns ptr to laser at the specified index
 */
volatile laser_t* getLaserInfo(int index) {
    return &(lasers[index]);
}

/*
 * Queues given laser to be displayed/moved on screen.
 *
 * laser- laser to be queued to be displayed on screen
 *
 * Returns TRUE is laser queued, FALSE otherwise.
 */
bool queueLaser(laser_t* laser) {
    return (xQueueSendToBack(Queue_Laser, laser, 0) == pdPASS);
}

/*
 * Initializes hardware/software resources and creates Laser Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskLaser(void) {
    // Allow only one Laser Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Create Queue for laser.
    Queue_Laser = xQueueCreate(LASER_COUNT, sizeof(laser_t));
    if (Queue_Laser == NULL) {
        return -1;
    }

    // Create Laser Task.
    BaseType_t result = xTaskCreate(task_laser,
                                    "Laser Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    GAME_PRIORITY,
                                    &Task_Laser_Handle
                                    );

    if (result != pdPASS) {
        vQueueDelete(Queue_Laser);
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    return 0;
}


/*
 * Tears down hardware/software resources and deletes Laser Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskLaser(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete queue.
    vQueueDelete(Queue_Laser);

    // Delete Buzzer Task.
    vTaskDelete(Task_Laser_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}
