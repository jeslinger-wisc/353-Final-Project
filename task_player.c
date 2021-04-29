/*
 * task_player.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Arman
 */
#include "task_player.h"

// Handle for Player Task.
static TaskHandle_t Task_Player_Handle;

// Vars to know the general state of the Player Task.
static volatile bool IS_LIVE = false;
static volatile bool IS_HIT = false;

/*
 * Function for Player Task. Controls the movement of the player sprite via
 * the data returned from the accelerometer (player movement is allowed on the x axis)
 * Upon detecting button press, player will shoot a laser from their current position.
 * This task uses markAsHit method to declare the game over.
 *
 */

void task_player(void *pvParameters) {
    // Var to determine if ready to shoot another laser.
    static bool readyToShoot = false;

    // Var with LCD image
    LCD_t playerImage = newEnemyImage();
    playerImage.x = 64;
    playerImage.y = 110;
    const int playerXRadius = (newEnemyImage().image_width_pixels / 2) + 1;

    const int playerYRadius = (newEnemyImage().image_height_pixels / 2) + 1;

    const int playerWidth = newEnemyImage().image_width_pixels;

    // Constant used for edge calculations.

    // Endless Task Loop.
    while (1) {

        //Create a new laser instantiaton for the player
        laser_t playerLaser = { .image = newLaserImage(),
                                .goingUp = false
                                };
        switch(getDirection())
               {
                case  LEFT://MOVE LEFT
                       {
                           if(playerImage.x > playerXRadius){

                             playerImage.x= playerImage.x-1;
                             //Check if the player is hit, priority!
                             markAsHit(&playerImage);
                             //if the player is hit, the game will be over!
                             if (playerImage.image == NULL) {
                                 switchToControlMode(false);
                             }
                             else{//otherwise print the new position of player
                                 LCDget(&playerImage);
                             }
                             break;
                             }
                           else{
                               break;
                           }
                       }
                   case RIGHT: //MOVE RIGHT
                       {
                           if(playerImage.x < (LCD_HORIZONTAL_MAX - playerXRadius)){

                               playerImage.x=playerImage.x+1;

                               //Check if the player is hit, priority!
                               markAsHit(&playerImage);
                               //if the player is hit, the game will be over!
                               if (playerImage.image == NULL) {
                                   switchToControlMode(false);
                               }
                               else{//otherwise print the new position of player
                                   LCDget(&playerImage);
                               }
                              break;
                               }
                           else{
                             break;
                           }
                       }
                   case CENTER:
                        {
                            //Check if the player is hit, priority!
                            markAsHit(&playerImage);
                            //if the player is hit, the game will be over!
                            if (playerImage.image == NULL) {
                                switchToControlMode(false);
                            }
                            else{//otherwise print the new position of player
                                LCDget(&playerImage);
                            }
                        break;
                      }
                   default:
                   {
                       break;
                   }
               }

        // Enable laser upon button un-pressing.
        if (!isButtonPressed()) {
            readyToShoot = true;
        }

        // Shoot laser when ready + signaled.
        if (readyToShoot && isButtonPressed()) {
            //Propel the laser across the screen from the player
            playerLaser.image.x =  playerImage.x;
            playerLaser.image.y = playerImage.y - playerYRadius - 1;
            queueLaser(&playerLaser);
            queueMelody(&playerShotMelody);

            // Disable shooting.
            readyToShoot = false;
        }

       vTaskDelay(pdMS_TO_TICKS(PLAYER_PERIOD_DELAY));
    }
}

/*
 * Call to detect if the player is hit by the enemy laser.
 * If hit is detected, set the image of the player to NULL and remove player sprite.
 */
void markAsHit(LCD_t* playerImage) {

    //LCD_t playerImage = playerImage;

    const int playerXRadius = (newEnemyImage().image_width_pixels / 2) + 1;
    const int playerYRadius = (newEnemyImage().image_height_pixels / 2) + 1;
    const int playerWidth = newEnemyImage().image_width_pixels;

    //Get all of the lasers on the screen (Shot by the enemy)
    // Often used constants for the Enemy Task.
    int laserIndex = 0;
     for (; laserIndex < LASER_COUNT; laserIndex++) {
         // Get laser info.
         volatile laser_t* laser = getLaserInfo(laserIndex);

         // Skip if laser from the index is not active.
         if (laser->image.image == NULL) {
             continue;
         }

         // Skip if laser is sent by the player themselves.
         if ((laser->goingUp) == false) {
             continue;
         }

         // Determine if the laser is hitting the player.
         //4 checks, covering the width of the player image and the height of the player image.
         int laserXRadius = (newLaserImage().image_width_pixels / 2) + 1;
         int laserYRadius = (newLaserImage().image_height_pixels / 2) + 1;

         if ((playerImage-> x - playerXRadius) > (laser->image.x + laserXRadius)) {
             continue;
         }

         else if ((playerImage-> x  + playerXRadius) < (laser->image.x - laserXRadius)) {
             continue;
         }

         else if ((playerImage-> y - playerYRadius) > (laser->image.y + laserYRadius)) {
             continue;
         }

         else if ((playerImage-> y + playerYRadius) < (laser->image.y - laserYRadius)) {
             continue;
         }


         else { // HIT DETECTED! YOU LOSE

             playerImage->fColor = LCD_COLOR_BLACK;
             LCDget(playerImage);
             playerImage->image = NULL;

             // Play death melody.
             queueMelody(&enemyDeathMelody);

             // Kill lasers that are still traversing.
             laser->image.image = NULL;
             LCD_t img = laser->image; // convert to non-volatile for the LCDget function
             img.fColor = LCD_COLOR_BLACK;
             LCDget(&img);
             continue;
         }
     }
}

/*
 * Initializes hardware/software resources and creates Player Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskPlayer(void) {
    // Allow only one Player Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Create Player Task.
    BaseType_t result = xTaskCreate(task_player,
                                    "Player Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    GAME_PRIORITY,
                                    &Task_Player_Handle
                                    );
    if (result != pdPASS) {
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Player Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */

int killTaskPlayer(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Player Task.
    vTaskDelete(Task_Player_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}
