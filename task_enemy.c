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
    // Often used constants for the Enemy Task.
    const int enemyXRadius = (newEnemyImage().image_width_pixels / 2) + 1;
    const int enemyYRadius = (newEnemyImage().image_height_pixels / 2) + 1;
    const int enemyWidth = newEnemyImage().image_width_pixels;

    // Vars to record position/movement of enemies.
    int leftLimit = 0;
    int rightLimit = (ENEMY_COUNT * (enemyWidth + 1)) + 1;
    int curY = enemyYRadius;
    bool goingLeft = false;

    // Var to record number of enemies alive.
    int enemiesAlive = ENEMY_COUNT;

    // Array of enemy sprite information.
    LCD_t enemies[ENEMY_COUNT];

    // Initialize each enemy and display them.
    int index = 0;
    for (; index < ENEMY_COUNT; index++) {
        // Set enemy details.
        enemies[index] = newEnemyImage();

        //Adjust enemy location.
        enemies[index].x = enemyXRadius + (index * (enemyWidth + 1));
        enemies[index].y = curY;

        // Display enemy.
        LCDget(&(enemies[index]));
    }

    // Endless Task Loop.
    while (1) {
        // Update each enemy.
        int index = 0;
        for(; index < ENEMY_COUNT; index++) {
            // Skip if dead.
            if (enemies[index].image == NULL) {
                continue;
            }

            // Kill if hit detected.
            int laserIndex = 0;
            for (; laserIndex < LASER_COUNT; laserIndex++) {
                // Get laser info.
                volatile laser_t* laser = getLaserInfo(laserIndex);

                // Skip if not active.
                if (laser->image.image == NULL) {
                    continue;
                }

                // Skip if laser from enemy ship.
                if (laser->goingUp) {
                    continue;
                }

                // Determine if laser is hitting enemy.
                LCD_t enemy = enemies[index];
                int laserXRadius = (newLaserImage().image_width_pixels / 2) + 1;
                int laserYRadius = (newLaserImage().image_height_pixels / 2) + 1;
                if ((enemy.x - enemyXRadius) > (laser->image.x + laserXRadius)) {
                    continue;
                }
                else if ((enemy.x + enemyXRadius) < (laser->image.x - laserXRadius)) {
                    continue;
                }
                else if ((enemy.y - enemyYRadius) > (laser->image.y + laserYRadius)) {
                    continue;
                }
                else if ((enemy.y + enemyYRadius) < (laser->image.y - laserYRadius)) {
                    continue;
                }
                else { // HIT!
                    // Kill enemy.
                    enemies[index].fColor = LCD_COLOR_BLACK;
                    LCDget(&(enemies[index]));
                    enemies[index].image = NULL;

                    // Play death melody.
                    queueMelody(&enemyDeathMelody);

                    /* Get rid of lasers on impact- leave out for now.
                    // Kill laser.
                    LCD_t img = laser->image; // convert to non-volatile.
                    img.fColor = LCD_COLOR_BLACK;
                    LCDget(&img);
                    laser->image.image = NULL;
                    */

                    // Check if any enemies are left.
                    enemiesAlive--;
                    if (enemiesAlive <= 0) {
                        switchToControlMode(true);
                    }

                    continue; // Don't process dead enemy.
                }
            }

            // Move enemy.
            enemies[index].x += (goingLeft) ? -1 : 1;
            enemies[index].y = curY;
            LCDget(&(enemies[index]));

            // Attempt to shoot laser.
            if( (rand() % ENEMY_SHOOT_CHANCE) == 0) {
                laser_t newLaser = { .image = newLaserImage(),
                                     .goingUp = true
                                   };
                newLaser.image.x = enemies[index].x;
                newLaser.image.y = enemies[index].y + enemyYRadius + 1;
                queueLaser(&newLaser);
                queueMelody(&enemyShotMelody);
            }
        }

        // Update end trackers.
        leftLimit += (goingLeft) ? -1 : 1;
        rightLimit += (goingLeft) ? -1 : 1;

        // Change movement direction/Y-level as needed.
        if ((leftLimit <= 0) || (rightLimit >= LCD_HORIZONTAL_MAX)) {
            goingLeft = !goingLeft;
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
