/*
 * task_buzzer.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef TASK_BUZZER_H_
#define TASK_BUZZER_H_

#include"commonHeaders.h"
#include "music.h"

/*
 * Sets the priority of the Buzzer Task.
 *
 * pLvl- Priority Level to set the task to.
 */
void setTaskBuzzerPriority(uint32_t pLvl);

/*
 * "Getter" method returning the state of the buzzer regarding any currenly
 * playing melodies.
 *
 * Returns TRUE if playing a melody, FALSE otherwise
 */
bool isPlayingMelody(void);

/*
 * Queues given melody to be played on the buzzer.
 *
 * melody- melody to queue to be played on buzzer.
 *
 * Returns TRUE is melody queued, FASLE otherwise.
 */
bool queueMelody(melody_t* melody);

/*
 * Initializes hardware/software resources and creates Buzzer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int initTaskBuzzer(void);

/*
 * Tears down hardware/software resources and deletes Buzzer Task.
 *
 * Returns 0 for successful setup, -1 otherwise.
 */
int killTaskBuzzer(void);

#endif /* TASK_BUZZER_H_ */
