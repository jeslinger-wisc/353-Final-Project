/*
 * menu_task.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "menu_task.h"

// Handle for Menu Task.
TaskHandle_t Menu_Task_Handle;

/*
 * Function for Menu Task.
 * JohnEsl-TODO
 */
void menu_task(void *pvParameters) {
    // set direction as an output
    P2->DIR |= BIT2;

    // Turn off LED
    P2->OUT &= ~BIT2;

    int i;

    // Endless Task Loop.
    while(1) {
        // turn on the LED
        P2->OUT |= BIT2;

        // Delay
        for(i=0; i < 1000000; i++){};

        // turn off the LED
        P2->OUT &= ~BIT2;

        // Delay
        for(i=0; i < 1000000; i++){};
    }
}


