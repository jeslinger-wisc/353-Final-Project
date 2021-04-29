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
 * LCD Task: Handles all LCD drawing requests. Takes/Waits for an
 * image from a queue, displays it on the screen, and repeats.
 *
 * Note: Images undergo a few edits/checks, namely 1) if darkness is
 * detected, non-black colors will be XORed 2) if the image data
 * ptr is null, the remaining data will be used to draw a rectangle
 * with the background color.
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

        // Invert color of image (unless black) for darker environments.
        if (isDark() && (curImage.fColor != LCD_COLOR_BLACK)) {
            curImage.fColor = (curImage.fColor ^ 0xFFFF);
        }

        // Display image as appropriate.
        if (curImage.image != NULL) { // Drawing image
            lcd_draw_image(curImage.x,
                           curImage.y,
                           curImage.image_width_pixels,
                           curImage.image_height_pixels,
                           curImage.image,
                           curImage.fColor,
                           curImage.bColor
            );
        }
        else { // Drawing background rectangle
            lcd_draw_rectangle(curImage.x,
                           curImage.y,
                           curImage.image_width_pixels,
                           curImage.image_height_pixels,
                           curImage.bColor
            );
        }
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

/*
 * Queue an image to be displayed on the LCD screen.
 *
 * LCDimage- ptr to image to display on the screen
 *
 * Returns TRUE if successful, FALSE otherwise
 */
BaseType_t LCDget(LCD_t* LCDimage) {
    return xQueueSendToBack(Queue_Lcd, LCDimage, portMAX_DELAY);
}

/*
 * "Clears" screen by setting every pixel to black.
 */
void clearScreen(void) {
    LCD_t clearImage = { .x = LCD_HORIZONTAL_MAX / 2,
                          .y = LCD_VERTICAL_MAX / 2,
                          .image_width_pixels = LCD_HORIZONTAL_MAX,
                          .image_height_pixels = LCD_VERTICAL_MAX,
                          .image = NULL, // Signals to draw black rectangle
                          .fColor = LCD_COLOR_BLACK,
                          .bColor = LCD_COLOR_BLACK,
    };
    LCDget(&clearImage);
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
