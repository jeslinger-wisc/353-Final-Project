/*
 * task_menu.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_menu.h"

// Handle for Menu Task.
TaskHandle_t Task_Menu_Handle = NULL;

/*
 * Function for Menu Task.
 * JohnEsl-TODO
 */
void task_menu(void *pvParameters) {
    // Create Button Task (temporarily with same priority).
    xTaskCreate
    (   task_button,
        "Task Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        BUTTON_TASK_PRIORITY,
        &Task_Button_Handle
    );

    // Create Buzzer Task (temporarily with same priority).
    xTaskCreate
    (   task_buzzer,
        "Task Buzzer",
        configMINIMAL_STACK_SIZE,
        NULL,
        BUZZER_TASK_PRIORITY,
        &Task_Buzzer_Handle
    );

    // Create Accel Task (temporarily with same priority).
    xTaskCreate
    (   task_accel,
        "Task Accel",
        configMINIMAL_STACK_SIZE,
        NULL,
        ACCEL_TASK_PRIORITY,
        &Task_Accel_Handle
    );

    // Endless Task Loop.
    while(1) {
        // setup blue LED
        P2->DIR |= BIT2;
        P2->OUT &= ~BIT2;

        // Set LED on right tilt.
        while((accelDir() != RIGHT)) {};
        P2->OUT |= BIT2;

        // Reset LED on left tile + back to center.
        while((accelDir() != LEFT)) {};
        while((accelDir() != CENTER)) {};
    }
}


