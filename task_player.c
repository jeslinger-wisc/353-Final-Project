/*
 * task_player.c
 *
 *  Created on: Apr 27, 2021
 *      Author: jesli
 */

#include "task_player.h"

// Handle for Player Task.
static TaskHandle_t Task_Player_Handle;

// Vars to know the general state of the Player Task.
static volatile bool IS_LIVE = false;
static volatile bool IS_HIT = false;

/*
 * Function for Player Task.
 * JohnEsl-TODO
 */
void task_player(void *pvParameters) {
    // Var with LCD image
    LCD_t playerImage = newEnemyImage();
    playerImage.x = 64;
    playerImage.y = 110;

    while(isButtonPressed()) {};
    // Endless Task Loop.
    while (1) {
        if (isButtonPressed()) {
            while(isButtonPressed()){};
            switchToControlMode(true);
        }
        /*
        switch(getDirection())
               {
                case  LEFT://MOVE LEFT
                       {

                           if(playerImage.x > 25){
                            //update the new position of the player sprite (decrement x position)
                               playerImage.x= playerImage.x-1;
                             //curLCDimage.x = x;
                             //curLCDimage.y = y;
                             //status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
                             LCDget(&playerImage);
                             break;
                             }
                           else{
                               break;
                           }
                       }
                   case RIGHT: //MOVE RIGHT
                       {
                           if(playerImage.x < 105){
                               //update the new position of the player sprite (increment x position)
                               playerImage.x=playerImage.x+1;
                               //curLCDimage.x = x;
                               //curLCDimage.y = y;
                               //status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
                               LCDget(&playerImage);
                              break;
                           }
                           else{
                             break;
                           }
                       }
                   case CENTER:
                        {
                        //status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
                        break;
                      }
                   default:
                   {
                       break;
                   }
               }
       */
       vTaskDelay(pdMS_TO_TICKS(PLAYER_PERIOD_DELAY));
    }
}

/*
 * Call when hit by laser
 */
void markAsHit(void) {
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


