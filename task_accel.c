/*
 * task_accel.c
 *
 *  Created on: Apr 23, 2021
 *      Author: John Eslinger
 */

#include "task_accel.h"

// Handle for Accelerometer Task (and sub-task).
static TaskHandle_t Task_Accel_Handle = NULL;
static TaskHandle_t Task_Accel_Bottom_Half_Handle = NULL;

// Vars to record the general state of the Accelerometer Task.
static volatile bool IS_LIVE = false;
static volatile dir_t ACCEL_DIR = CENTER;

/*
 * Accelerometer Task: Starts an ADC conversion of accelerometer's
 * x-axis before sleeping for a configurable amount of milliseconds.s
 */
static void task_accel(void *pvParameters) {
    // Endless Task Loop.
    while (1) {
        // Start ADC conversion.
        ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

        // Delay before starting next conversion.
        vTaskDelay(pdMS_TO_TICKS(ACCEL_PERIOD_DELAY));
    }
}

/*
 * Accelerometer Bottom Half Sub-Task: Takes ADC converted values (passed in
 * via a task notification) and uses them to convey the direction the x-axis
 * of the accelerometer is being tilted.
 */
static void task_accel_bottom_half(void *pvParameters) {
    // Var to get current accelerometer reading.
    uint32_t xaxisValue = 0;

    // Endless Task Loop.
    while (1) {
        // Get/Wait for task notification with x-axis value.
        if (xTaskNotifyWait(0, 0, &xaxisValue, portMAX_DELAY) != pdPASS) {
            continue;
        }

        // Update the recorded state of the accelerometer.
        ACCEL_DIR = CENTER;
        if (xaxisValue < (ACCEL_CENTER_SAMPLE - ACCEL_CENTER_THRESH)) {
            ACCEL_DIR = LEFT;
        }
        else if (xaxisValue > (ACCEL_CENTER_SAMPLE + ACCEL_CENTER_THRESH)) {
            ACCEL_DIR = RIGHT;
        }
    }
}

/*
 * Sets the priority of the Accelerometer Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskAccelPriority(uint32_t pLvl) {
    vTaskPrioritySet(Task_Accel_Handle, pLvl);
    vTaskPrioritySet(Task_Accel_Bottom_Half_Handle, pLvl);
}

/*
 * Getter function to determine the direction the MKII booster pack is
 * being tilted (with regards to the x-axis accelerometer facing the
 * negative y-axis direction).
 *
 * Returns direction MKII booster pack is being tilted
 */
dir_t getDirection(void) {
    return ACCEL_DIR;
}

/*
 * Initializes hardware/software resources and creates Accelerometer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskAccel(void) {
    // Allow only one Accelerometer Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Configure pin to x-axis accelerometer as analog input.
    P6->DIR &= ~BIT1;
    P6->SEL0 |= BIT1; // select = 2'b11
    P6->SEL1 |= BIT1;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHP |     // timer source
                  ADC14_CTL0_SHT0_2;   // 16 samples

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the x-axis accelerometer signal with MEM[0]
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_14;

    // Enable interrupts AFTER a value is written into MEM[0].
    ADC14->IER0 |= ADC14_IER0_IE0;

    // Enable ADC Interrupt with priority 2.
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC on.
    ADC14->CTL0 |= ADC14_CTL0_ON;

    // Create Accelerometer Task.
    BaseType_t result = xTaskCreate(task_accel,
                                    "Accelerometer Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Accel_Handle
                                    );
    if (result != pdPASS) {
        return -1;
    }

    // Create Accelerometer Bottom Half Task.
    BaseType_t result2 = xTaskCreate(task_accel_bottom_half,
                                    "Accelerometer Bottom Half Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Accel_Bottom_Half_Handle
                                    );
    if (result2 != pdPASS) {
        vTaskDelete(Task_Accel_Handle); // All or nothing
        return -1;
    }

    // Return for successful setup.
    IS_LIVE = true;
    ACCEL_DIR = CENTER;
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Accelerometer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskAccel(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Accelerometer Task and sub-task.
    vTaskDelete(Task_Accel_Handle);
    vTaskDelete(Task_Accel_Bottom_Half_Handle);

    // Return for successful deconstruction.
    IS_LIVE = false;
    return 0;
}

/*
 * Interrupt Handler for ADC14. Sends collected data from the accelerometer
 * to the Accelerometer Task's bottom half task.
 */
void ADC14_IRQHandler(void) {
    // Get the converted value of the x-axis accelerometer (clears interrupt).
    uint32_t xaxisValue = ADC14->MEM[0];

    // Send data to bottom half task.
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(Task_Accel_Bottom_Half_Handle,
                       xaxisValue,
                       eSetValueWithOverwrite,
                       &xHigherPriorityTaskWoken);
}
