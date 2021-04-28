/*
 * music.c
 *
 *  Created on: Apr 23, 2021
 *      Author: John Eslinger
 */

#include "music.h"

// Start game melody.
static note_t startNotes[] =
{
 {NOTE_E6, QUARTER_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_C6, QUARTER_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_E6, QUARTER_NOTE},
 {NOTE_F6, EIGTH_NOTE},
 {NOTE_G6, QUARTER_NOTE}
};
melody_t startMelody = {startNotes, 7};

// Lose game melody.
static note_t loseNotes[] =
{
 {NOTE_G6, QUARTER_NOTE},
 {NOTE_D6S, QUARTER_NOTE},
 {NOTE_C6, HALF_NOTE},
 {NOTE_G6, QUARTER_NOTE},
 {NOTE_D6S, QUARTER_NOTE},
 {NOTE_C6, HALF_NOTE}
};
melody_t loseMelody = {loseNotes, 6};

// Win game melody.
static note_t winNotes[] =
{
 {NOTE_A5, EIGTH_NOTE},
 {NOTE_G5, EIGTH_NOTE},
 {NOTE_F5, EIGTH_NOTE},
 {NOTE_E5, QUARTER_NOTE},
 {NOTE_G5, EIGTH_NOTE},
 {NOTE_C6, QUARTER_NOTE},
 {NOTE_E6, EIGTH_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_C6S, EIGTH_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_F6, QUARTER_NOTE},
 {NOTE_B5, EIGTH_NOTE},
 {NOTE_C6, QUARTER_NOTE}
};
melody_t winMelody = {winNotes, 13};

// Enemy Shot melody.
static note_t enemyShotNotes[] =
{
 {NOTE_E6, SIXTEENTH_NOTE}
};
melody_t enemyShotMelody = {enemyShotNotes, 1};

// Enemy Death melody.
static note_t enemyDeathNotes[] =
{
 {NOTE_C6, SIXTEENTH_NOTE}
};
melody_t enemyDeathMelody = {enemyDeathNotes, 1};

// Player Shot melody.
static note_t playerShotNotes[] =
{
 {NOTE_G6, SIXTEENTH_NOTE}
};
melody_t playerShotMelody = {playerShotNotes, 1};
