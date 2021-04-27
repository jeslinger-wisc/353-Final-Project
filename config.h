/*
 * config.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Group Priority Levels
#define CONTROL_PRIORITY        3 // Priority of game management tasks
#define GAME_PRIORITY           2 // Priority of gameplay tasks

// Button Task Configs
#define BUTTON_DEBOUNCE_VALUE   10 // # of consecutive pressed samples for "pressed" state
#define BUTTON_PERIOD_DELAY     5  // Time (ms) between button sampling

// Buzzer Task Configs
#define BUZZER_BPM              120 // Beats per minute to play melodies at
#define BUZZER_QUEUE_SIZE       10  // Size of queue for pending melodies

// Accelerometer Task Configs
#define ACCEL_PERIOD_DELAY      5        // Time (ms) between ADC conversion starts
#define ACCEL_CENTER_SAMPLE     2060     // Average x-axis value when held flat (experimentally found)
#define ACCEL_CENTER_THRESH    (830 / 3) // Radius of "center" region (830 being experimentally found limit)

// Light Sensor Task Configs
#define LIGHT_PERIOD_DELAY      5   // Time (ms) between checking the light sensor
#define LIGHT_DARK_THRESH       100 // Largest lux value that registers as "dark"

// LCD Task Configs
#define LCD_QUEUE_SIZE        15 // Size of queue for pending images

#endif /* CONFIG_H_ */

// Priority level check
#if CONTROL_PRIORITY <= GAME_PRIORITY
#error "CONTROL PRIORITY must be greater than GAME_PRIORITY"
#endif
