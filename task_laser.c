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
static volatile laser_t LASERS[LASER_COUNT];

/*
 * Laser Task: Handles laser creation, movement, and (partial) tear down.
 * Within a cycle, each laser is moved and torn down if it has reached an
 * edge. A new laser is also created each cycle if applicable. A configurable
 * amount of time is waited in between cycles.
 */
void task_laser(void *pvParameters) {
    // Constant used within the Laser Task.
    const int laserYRadius = (newLaserImage().image_height_pixels / 2) + 1;

    // Initialize laser array to be empty.
    int index = 0;
    for (; index < LASER_COUNT; index ++) {
        // Setup initial details.
        LASERS[index].image = newLaserImage();

        // Set as empty.
        LASERS[index].image.image = NULL;
    }

    // Endless Task Loop.
    while (1) {
        // Update current lasers.
        int index = 0;
        for (; index < LASER_COUNT; index++) {
            // Skip if not an active laser.
            if (LASERS[index].image.image == NULL) {
                continue;
            }

            // Nullify laser if at border.
            int laserTop = LASERS[index].image.y + laserYRadius;
            int laserBottom = LASERS[index].image.y - laserYRadius;
            if ((laserBottom <= 0) || (laserTop >= LCD_HORIZONTAL_MAX)) {
                LCD_t laserImage = LASERS[index].image; // convert to non-volatile
                laserImage.fColor = LCD_COLOR_BLACK;
                LCDget(&laserImage);
                LASERS[index].image.image = NULL; // Nullify array slot's var

                continue; // Don't process inactive laser
            }

            // Move/Display laser.
            LASERS[index].image.y += (LASERS[index].goingUp) ? 1 : -1;
            LCD_t laserImg = LASERS[index].image; // convert to non-volatile
            LCDget(&laserImg);
        }

        // Attempt to create a new laser.
        laser_t newLaser;
        if (xQueueReceive(Queue_Laser, &newLaser, 0) == pdPASS) {
            // Find open spot for laser (assuming one exists).
            int index = 0;
            for (; index < LASER_COUNT; index++) {
                // Fill in open spot if available.
                if (LASERS[index].image.image == NULL) {
                    LASERS[index] = newLaser; // fill
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
    return &(LASERS[index]);
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
