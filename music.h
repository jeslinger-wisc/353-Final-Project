/*
 * music.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include "commonHeaders.h"

// Note Periods for common notes (assuming 24 MHz clock frequency).
#define NOTE_E5        36419 // 659 Hz
#define NOTE_F5        34384 // 698 Hz
#define NOTE_G5        30612 // 784 Hz
#define NOTE_A5        27273 // 880 Hz
#define NOTE_A5S       26316 // 932 Hz
#define NOTE_B5        24291 // 988 Hz
#define NOTE_C6        22945 // 1046 Hz
#define NOTE_C6S       21641 // 1109 Hz
#define NOTE_D6        20426 // 1175 Hz
#define NOTE_D6S       19277 // 1245 Hz
#define NOTE_E6        18196 // 1319 Hz
#define NOTE_F6        17180 // 1397 Hz
#define NOTE_F6S       16216 // 1480 Hz
#define NOTE_G6        15306 // 1568 Hz
#define NOTE_G6S       14449 // 1661 Hz
#define NOTE_A6        13636 // 1760 Hz
#define NOTE_A6S       12869 // 1865 Hz
#define NOTE_B6        12146 // 1976 Hz
#define NOTE_C7        11467 // 2093 Hz
#define NOTE_C7S       10825 // 2217 Hz
#define NOTE_D7        10217 // 2349 Hz
#define NOTE_D7S       9642  // 2489 Hz
#define NOTE_E7        9101  // 2637 Hz
#define NOTE_F7        8590  // 2794 Hz
#define NOTE_F7S       8108  // 2960 Hz
#define NOTE_G7        7653  // 3136 Hz
#define NOTE_G7S       7225  // 3322 Hz

// Durations for common lengths (assuming 4/4 time).
#define QUARTER_NOTE (60000 / BUZZER_BPM)
#define EIGTH_NOTE ((QUARTER_NOTE) / 2)
#define SIXTEENTH_NOTE ((QUARTER_NOTE) / 4)
#define HALF_NOTE ((QUARTER_NOTE) * 2)
#define WHOLE_NOTE ((QUARTER_NOTE) * 4)

// Structs to define notes and melodies.
typedef struct NoteStruct {
    uint32_t notePeriod;
    uint32_t durationMs;
} note_t;
typedef struct MelodyStruct {
    note_t* melody;
    uint32_t numNotes;
} melody_t;

////////////////////////
// Available Melodies //
////////////////////////

// Start game melody.
extern melody_t startMelody;

// Lose game melody.
extern melody_t loseMelody;

// Win game melody.
extern melody_t winMelody;

// Enemy Shot melody.
extern melody_t enemyShotMelody;

// Enemy Death melody.
extern melody_t enemyDeathMelody;

// Player Shot melody.
extern melody_t playerShotMelody;

#endif /* MUSIC_H_ */
