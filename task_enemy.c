/*
 * task_enemy.c
 *
 *  Created on: Apr 27, 2021
 *      Author: John Eslinger
 */

#include "task_enemy.h"

// Handle for Enemy Task.
static TaskHandle_t Task_Enemy_Handle = NULL;

// Vars to record the general state of the Enemy Task.
static volatile bool IS_LIVE = false;

/*
 * Enemy Task:
 * JohnEsl-TODO
 */
static void task_enemy(void *pvParameters) {
    // Constant used for edge calculations.
    const int enemyRadius = (newEnemyImage().image_width_pixels / 2) + 1;
    const int enemyYRadius = (newEnemyImage().image_height_pixels / 2) + 1;

    // Vars to record positional and directional data.
    bool movingLeft = false;
    uint16_t curY = ((newEnemyImage().image_height_pixels) / 2) + 1;

    // Array of enemy sprite info.
    LCD_t enemies[ENEMY_COUNT];

    // Initialize each enemy and display them.
    int startX = ((newEnemyImage().image_width_pixels) / 2) + 1;
    int index = 0;
    for (; index < ENEMY_COUNT; index++) {
        // Set enemy details.
        enemies[index] = newEnemyImage();

        // Adjust enemy location.
        enemies[index].x = enemyRadius + (index * (newEnemyImage().image_width_pixels + 1));
        enemies[index].y = curY;

        // Display enemy.
        LCDget(&enemies[index]);
    }

    // Endless Task Loop.
    while(1) {
        // Update each enemy.
        int index = 0;
        for (; index < ENEMY_COUNT; index++) {
            // Adjust to new position (dead enemies too- for movements).
            enemies[index].x += (movingLeft) ? -1 : 1;
            enemies[index].y = curY;

            // Skip if already dead.
            if (enemies[index].image == NULL) {
                continue;
            }

            // Mark as dead if hit detected.
            /*
            int lIndex = 0;
            for (; lIndex < LASER_COUNT; lIndex++) {
                // Get laser info.
                LCD_t laser = getLaserInfo(lIndex);

                // Skip if not an active laser.
                if (laser.image == NULL) {
                    continue;
                }

                // Determine if laser is hitting enemy.
                LCD_t enemy = enemies[index];
                int laserRadius = (laserImage().image_width_pixels / 2);
                if ((enemy.x - enemyRadius) > (laser.x + laserRadius)) {
                    continue;
                }
                else if ((enemy.x + enemyRadius) < (laser.x - laserRadius)) {
                    continue;
                }
                else if ((enemy.y - enemyRadius) > (laser.y + laserRadius)) {
                    continue;
                }
                else if ((enemy.y + enemyRadius) < (laser.y - laserRadius)) {
                    continue;
                }
                else { // HIT!
                    // "Delete" enemy and mark dead.
                    enemy.fColor = LCD_COLOR_BLACK;
                    LCDget(&enemy);
                    enemy.image = NULL;
                }
            }
            */

            // Display enemy (as possible).
            if (enemies[index].image != NULL) {
                LCDget(&enemies[index]);
            }

            // Attempt to shoot laser.
            if( (rand() % ENEMY_SHOOT_CHANCE) == 0) {
                laser_t newLaser = { .image = newLaserImage(),
                                     .goingUp = true
                                   };
                newLaser.image.x = enemies[index].x;
                newLaser.image.y = enemies[index].y + enemyYRadius + 5;
                queueLaser(&newLaser);
                // queueLaser(enemies[index].x, enemies[index].y, false);
            }
        }

        // Update direction/vertical position as needed.
        if (!movingLeft && (enemies[ENEMY_COUNT - 1].x >= (LCD_HORIZONTAL_MAX - enemyRadius))) {
            movingLeft = !movingLeft;
            curY++;
        }
        else if (movingLeft && (enemies[0].x <= enemyRadius)) {
            movingLeft = !movingLeft;
            curY++;
        }

        // End game as needed.
        if (curY >= ENEMY_Y_ENDGAME) {
            switchToControlMode(false);
        }

        // Delay till next cycle.
        vTaskDelay(pdMS_TO_TICKS(ENEMY_PERIOD_DELAY));
    }
}

/*
 * Initializes hardware/software resources and creates Enemy Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskEnemy(void) {
    // Allow only one Enemy Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Create Enemy Task.
    BaseType_t result = xTaskCreate(task_enemy,
                                    "Enemy Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    GAME_PRIORITY,
                                    &Task_Enemy_Handle
                                    );
    if (result != pdPASS) {
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Enemy Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskEnemy(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Enemy Task.
    vTaskDelete(Task_Enemy_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}
