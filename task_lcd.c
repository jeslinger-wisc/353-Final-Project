/*
 * task_lcd.c
 *
 *  Created on: Apr 26, 2021
 *      Author: John Eslinger
 */

#include "task_lcd.h"

// Handle for LCD Task.
static TaskHandle_t Task_Lcd_Handle;

// Queue of images to display on the LCD screen.
static QueueHandle_t Queue_Lcd;

// Vars to know the general state of the LCD Task.
static volatile bool IS_LIVE = false;

/*
 * Function for LCD Task.
 * JohnEsl-TODO
 */
void task_lcd(void *pvParameters) {
    // Var to store current image being processed.
    LCD_t curImage;

    // Endless Task Loop.
    while (1) {
        // Get/Wait for image.
        if (xQueueReceive(Queue_Lcd, &curImage, portMAX_DELAY) != pdPASS) {
            continue;
        }

        // Invert color of image for darker environments.
        if (isDark()) {
            curImage.fColor = (curImage.fColor ^ 0xFFFF);
        }

        // Display image.
        lcd_draw_image(curImage.x,
                       curImage.y,
                       curImage.image_width_pixels,
                       curImage.image_height_pixels,
                       curImage.image,
                       curImage.fColor,
                       curImage.bColor
        );
    }
}

/*
 * Sets the priority of the LCD Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskLCDPriority(uint32_t pLvl) {
    vTaskPrioritySet(Task_Lcd_Handle, pLvl);
}

// TODO
BaseType_t LCDget(LCD_t* LCDimage) {
    return xQueueSendToBack(Queue_Lcd, LCDimage, portMAX_DELAY);
}

/*
 * Initializes hardware/software resources and creates LCD Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskLCD(void) {
    // Allow only one LCD Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Setup hardware.
    Crystalfontz128x128_Init();

    // Create Queue for LCD.
    Queue_Lcd = xQueueCreate(LCD_QUEUE_SIZE, sizeof(LCD_t));
    if (Queue_Lcd == NULL) {
        return -1;
    }

    // Create Buzzer Task.
    BaseType_t result = xTaskCreate(task_lcd,
                                    "LCD Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Lcd_Handle
                                    );
    if (result != pdPASS) {
        vQueueDelete(Queue_Lcd);
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    return 0;
}

/*
 * Tears down hardware/software resources and deletes LCD Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskLCD(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Buzzer Task.
    vTaskDelete(Task_Lcd_Handle);

    // Delete queue.
    vQueueDelete(Queue_Lcd);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}
