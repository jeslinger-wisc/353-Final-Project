/*
 * task_menu.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_menu.h"

// Handle for Menu Task.
TaskHandle_t Task_Menu_Handle = NULL;

int CHECK;

/*
 * Function for Menu Task.
 * JohnEsl-TODO
 */
void task_menu(void *pvParameters) {
    // set direction as an output
    P2->DIR |= BIT2;

    // Turn off LED
    P2->OUT &= ~BIT2;

    // Endless Task Loop.
    while(1) {
        // Create button task with very short delay between button samples.
        CHECK = 1;
        xTaskCreate
        (   task_button,
            "Task Button",
            configMINIMAL_STACK_SIZE,
            NULL,
            3,
            &Task_Button_Handle
        );

        // Set LED on button press.
        while (!isButtonPressed()) {};
        P2->OUT |= BIT2;
        while (isButtonPressed()) {};

        // Delete Button Task and give time for deletion.
        vTaskDelete(Task_Button_Handle);
        vTaskDelay(pdMS_TO_TICKS(MENU_DEALLOC_DELAY));

        // Create another button task with much longer delay between samples.
        CHECK = 100;
        xTaskCreate
        (   task_button,
            "Task Button",
            configMINIMAL_STACK_SIZE,
            NULL,
            3,
            &Task_Button_Handle
        );

        // Reset LED on press- longer delay shows unique button task instances.
        while (!isButtonPressed()) {};
        P2->OUT &= ~BIT2;
        while (isButtonPressed()) {};

        // Delete Button Task and give time for deletion.
        vTaskDelete(Task_Button_Handle);
        vTaskDelay(pdMS_TO_TICKS(MENU_DEALLOC_DELAY));
    }
}


