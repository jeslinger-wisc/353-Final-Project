/*
 * task_buzzer.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_buzzer.h"

// Handle for Buzzer Task.
static TaskHandle_t Task_Buzzer_Handle;

// Queue of melodies to play on buzzer.
static QueueHandle_t Queue_Buzzer;

// Vars to know the general state of the Buzzer task.
static volatile bool IS_LIVE = false;
static volatile bool PLAYING_MELODY = false;
static volatile uint32_t NOTE_INDEX = 0;

/*
 * Helper function to disable buzzer from making any noise.
 */
static void silenceBuzzer(void) {
    // Clear mode control (ie stop timer)
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

/*
 * Helper function to play specified note on buzzer.
 *
 * notePeriod- period of the note to play.
 */
static void playBuzzer(uint32_t notePeriod) {
    // Disable buzzer for configuration
    silenceBuzzer();

    // Set period
    TIMER_A0->CCR[0] = notePeriod - 1;

    // Set duty cycle to 50%
    TIMER_A0->CCR[4] = (notePeriod / 2) - 1;

    // Clear count and enable buzzer (set to "Up" mode)
    TIMER_A0->CTL |= TIMER_A_CTL_CLR | TIMER_A_CTL_MC__UP;
}

/*
 * Function for Buzzer Task.
 * JohnEsl-TODO
 */
void task_buzzer(void *pvParameters) {
    // Var to store current melody being processed.
    melody_t curMelody;

    // Endless Task Loop.
    while (1) {
        // Get/Wait for melody.
        if (xQueueReceive(Queue_Buzzer, &curMelody, portMAX_DELAY) != pdPASS) {
            continue;
        }

        // Play each note in melody
        PLAYING_MELODY = true;
        for(NOTE_INDEX = 0; NOTE_INDEX < curMelody.numNotes; NOTE_INDEX++) {
            playBuzzer(curMelody.melody[NOTE_INDEX].notePeriod);
            vTaskDelay(pdMS_TO_TICKS(curMelody.melody[NOTE_INDEX].durationMs));
            silenceBuzzer();
        }
        PLAYING_MELODY = false;
    }
}

/*
 * "Getter" method returning the state of the buzzer regarding any currenly
 * playing melodies.
 *
 * Returns TRUE if playing a melody, FALSE otherwise
 */
bool isPlayingMelody(void) {
    return PLAYING_MELODY;
}

/*
 * Queues given melody to be played on the buzzer.
 *
 * melody- melody to queue to be played on buzzer.
 *
 * Returns TRUE is melody queued, FASLE otherwise.
 */
bool queueMelody(melody_t* melody) {
    return (xQueueSendToBack(Queue_Buzzer, melody, portMAX_DELAY) == pdPASS);
}

/*
 * Initializes hardware/software resources and creates Buzzer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskBuzzer(void) {
    // Allow only one Button Task instance at a time.
    if (IS_LIVE) {
        return -1;
    }

    // Set as output
    P2->DIR |= BIT7;

    // Set TimerA(0.4) to control Buzzer
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    // Turn off TimerA
    TIMER_A0->CTL = 0;

    // Set master clock as time source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;

    // Set to "Set/Reset" Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // Create Queue for buzzer.
    Queue_Buzzer = xQueueCreate(BUZZER_QUEUE_SIZE, sizeof(melody_t));
    if (Queue_Buzzer == NULL) {
        return -1;
    }

    // Create Buzzer Task.
    BaseType_t result = xTaskCreate(task_buzzer,
                                    "Buzzer Task",
                                    configMINIMAL_STACK_SIZE,
                                    NULL,
                                    CONTROL_PRIORITY,
                                    &Task_Buzzer_Handle
                                    );
    if (result != pdPASS) {
        vQueueDelete(Queue_Buzzer);
        return -1;
    }

    // Return for successful setup.
    return 0;
}

/*
 * Tears down hardware/software resources and deletes Buzzer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskBuzzer(void) {
    // Only kill task if able to.
    if (!(IS_LIVE)) {
        return -1;
    }

    // Delete Buzzer Task.
    vTaskDelete(Task_Buzzer_Handle);

    // Delete queue.
    vQueueDelete(Queue_Buzzer);

    // Return for successful deconstruction.
    return 0;
}
