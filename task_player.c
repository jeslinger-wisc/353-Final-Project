/*
 *  task_player.c
 *
 *  Created on: Apr 26, 2021
 *  Author: Arman Ramiz
 */

#include <task_player.h>

#define SHIP_QUEUE_LEN  250

TaskHandle_t Task_Player_Handle;

/******************************************************************************
 * This function will initialize Queue_LCD  and initialize the LCD
 ******************************************************************************/
void Task_Space_Ship_Init(void)
{

    Queue_LCD = xQueueCreate(SHIP_QUEUE_LEN,sizeof(LCD_t));

    // ADD CODE to initialize the LCD
    Crystalfontz128x128_Init();

}


/******************************************************************************
 * This task manages the movement of the player controlled image. It takes notifications
 * from the accelerometer, updates the position of the player sprite, and send that data
 * to the lcd task.
 ******************************************************************************/
void Task_Player(void *pvParameters)
{
    uint8_t x = 64;
    uint8_t y = 110;
    LCD_t curLCDimage;

     BaseType_t status;

    // Endless Task Loop.
     //task_lcd_draw_image(x, height, space_lazerWidth, space_lazerHeight, Bitmaps_Lazer, BLUE_5, BG_COLOR);
    // status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
    while(1) {

        //ece353_T32_2_Interrupt_Ms(10);
        //turn blue for some reason

        P2->DIR |= BIT2;
        P2->OUT &= ~BIT2;
        if(isButtonPressed()){
            P2->OUT |= BIT2;
        }
       int height=90;

      /* if(LCD_UPDATE){
        if(isButtonPressed()){
         while(height >= 0){
                // status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);

                  height = height -1;
                 // task_lcd_draw_image(x, height, space_lazerWidth, space_lazerHeight, Bitmaps_Lazer, BLUE_5, BG_COLOR);
                 LCD_UPDATE= false;
             }
           }
        }*/

       //INITIALIZE THE PLAYER SPRITE
       curLCDimage.image_width_pixels =space_shipWidthPixels;

       curLCDimage.image_height_pixels =space_shipHeightPixels;

       curLCDimage.image = space_shipBitmaps;

       curLCDimage.fColor = FG_COLOR;

       curLCDimage.bColor = BG_COLOR;

       curLCDimage.x = x;
       curLCDimage.y = y;

       //status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
        switch(accelDir())
               {
                case  LEFT://MOVE LEFT
                       {

                           if(x > 25){
                            //update the new position of the player sprite (decrement x position)
                             x= x-1;
                             curLCDimage.x = x;
                             curLCDimage.y = y;
                             status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
                             break;
                             }
                           else{
                               break;
                           }
                       }
                   case RIGHT: //MOVE RIGHT
                       {
                           if(x < 105){
                               //update the new position of the player sprite (increment x position)
                               x=x+1;
                               curLCDimage.x = x;
                               curLCDimage.y = y;
                               status = xQueueSend(Queue_LCD, &curLCDimage, portMAX_DELAY);
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
       vTaskDelay(pdMS_TO_TICKS(16));

    }
}


