/*
 * music.c
 *
 *  Created on: Apr 23, 2021
 *      Author: John Eslinger
 */

#include "music.h"

// Sample melody.
note_t sampleNotes[] =
{
 {NOTE_E6, QUARTER_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_C6, QUARTER_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_E6, QUARTER_NOTE},
 {NOTE_F6, EIGTH_NOTE},
 {NOTE_G6, QUARTER_NOTE},
 {NOTE_E6, EIGTH_NOTE},
 {NOTE_D6, EIGTH_NOTE},
 {NOTE_C6, EIGTH_NOTE},
 {NOTE_B5, QUARTER_NOTE},
 {NOTE_A5, EIGTH_NOTE},
 {NOTE_B5, QUARTER_NOTE},
 {NOTE_C6, EIGTH_NOTE},
 {NOTE_G5, QUARTER_NOTE},
};
melody_t sampleMelody = {sampleNotes, 15};


