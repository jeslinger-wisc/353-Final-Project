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
        3,
        &Task_Button_Handle
    );

    // Create Buzzer Task (temporarily with same priority).
    xTaskCreate
    (   task_buzzer,
        "Task Buzzer",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Buzzer_Handle
    );

    // Endless Task Loop.
    while(1) {
        // Play song anytime button is hit.
        while(!isButtonPressed()) {};
        playMelody(&sampleMelody);
        while(isButtonPressed()) {};
    }
}


