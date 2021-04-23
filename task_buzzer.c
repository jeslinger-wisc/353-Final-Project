/*
 * task_buzzer.c
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#include "task_buzzer.h"

// Handle for Buzzer Task.
TaskHandle_t Task_Buzzer_Handle;

// Queue of melodies to play on buzzer.
QueueHandle_t Queue_Buzzer;

// Flag to know when to flush queue/stop playing current melody.
static volatile bool FLUSH_BUZZER = false;

/*
 * Initialization function used to initialize hardware resources to use the
 * buzzer on the MKII booster pack.
 */
void initTaskBuzzer(void) {
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
        while(1) {/* Something went wrong*/};
    }
}

/*
 * Queues given melody to be played on the buzzer.
 *
 * melody- melody to queue to be played on buzzer.
 *
 * Returns pdPASS if melody queued, errQUEUE_FULL otherwise.
 */
BaseType_t playMelody(melody_t* melody) {
    return xQueueSendToBack(Queue_Buzzer, melody, portMAX_DELAY);
}

/*
 * Helper function to disable buzzer from making any noise.
 */
void silenceBuzzer(void) {
    // Clear mode control (ie stop timer)
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

/*
 * Helper function to play specified note on buzzer.
 *
 * notePeriod- period of the note to play.
 */
void playBuzzer(uint32_t notePeriod) {
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
        int noteNum;
        for(noteNum = 0; noteNum < curMelody.numNotes; noteNum++) {
            playBuzzer(curMelody.melody[noteNum].notePeriod);
            vTaskDelay(pdMS_TO_TICKS(curMelody.melody[noteNum].durationMs));
            silenceBuzzer();
        }
    }
}
