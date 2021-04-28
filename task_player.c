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
 *
 */

void task_player(void *pvParameters) {
    // Var with LCD image
    LCD_t playerImage = sampleImage();
    playerImage.x = 64;
    playerImage.y = 110;

    // Constant used for edge calculations.


    // Endless Task Loop.
    while (1) {
        switch(getDirection())
               {
                case  LEFT://MOVE LEFT
                       {
                           if(playerImage.x > 25){
                             playerImage.x= playerImage.x-1;

                             //LCDget(&playerImage);//print the new user position

                             markAsHit(&playerImage);
                                 //print the destroyed space ship image
                                 //maybe add lives?

                             if (playerImage.image != NULL) {
                                 LCDget(&playerImage);
                             }

                             if(isButtonPressed()){
                                 queueLaser(playerImage.x, playerImage.y, true);
                             }
                             break;
                             }
                           else{
                               break;
                           }
                       }
                   case RIGHT: //MOVE RIGHT
                       {
                           if(playerImage.x < 105){
                               playerImage.x=playerImage.x+1;

                              // LCDget(&playerImage);//print the new user position

                               markAsHit(&playerImage);
                                   //print the destroyed space ship image
                                   //maybe add lives?

                               if (playerImage.image != NULL) {
                                   LCDget(&playerImage);
                               }

                               if(isButtonPressed()){
                                   queueLaser(playerImage.x, playerImage.y, true);

                               }
                              break;
                               }
                           else{
                             break;
                           }
                       }
                   case CENTER:
                        {

                        break;
                      }
                   default:
                   {
                       break;
                   }
               }
       vTaskDelay(pdMS_TO_TICKS(PLAYER_PERIOD_DELAY));
    }
}

//laser_t getLaserInfo(int index) // Gets you location/size details regarding laser sprite with said index- if the image field is NULL, then the sprite is not in use.

//void queueLaser(int x, int y, isPlayerLaser) // Queues laser to be created/shot across screen based on starting location and direction.

/*
 * Call when hit by laser
 */

void markAsHit(LCD_t* playerImage) {

    int lIndex = 0;
    int LASER_COUNT =1;
    //Get all of the lasers on the screen (Shot by the enemy)

    const int playerRadius = (playerImage.image_width_pixels / 2) + 1;

    for (; lIndex < LASER_COUNT; lIndex++) {
        // Get laser info.
        LCD_t laser = getLaserInfo(lIndex);
        // Skip if not an active laser.
        if (laser.image == NULL) {
              continue;
        }

        // Determine if laser is hitting player.
        //LCD_t enemy = enemies[index];
        int laserRadius = (laser.image_width_pixels / 2);

        //HIT DETECTION. Detec if the player image touches the laser image from the enemy

        if ((playerImage.x - playerRadius) > (laser.x + laserRadius)) {
               continue;
         }

        else if ((playerImage.x + playerRadius) < (laser.x - laserRadius)) {
                continue;
        }

        else if ((playerImage.y - playerRadius) > (laser.y + laserRadius)) {
                 continue;
        }

        else if ((playerImage.y + playerRadius) < (laser.y - laserRadius)) {
                continue;
        }

        else { // HIT!
        // "Delete" enemy and mark dead.
            playerImage.fColor = LCD_COLOR_BLACK;
            //LCDget(&playerImage);
            playerImage.image = NULL;
        }
    }
    return;
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

    // Delete Buzzer Task.
    vTaskDelete(Task_Player_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}


