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

// Menu Task Configs
#define MENU_KILL_DELAY         50

// Button Task Configs
#define BUTTON_DEBOUNCE_VALUE   10 // # of consecutive pressed samples for "pressed" state
#define BUTTON_PERIOD_DELAY     5  // Time (ms) between button sampling

// Buzzer Task Configs
#define BUZZER_BPM              180 // Beats per minute to play melodies at
#define BUZZER_QUEUE_SIZE       10  // Size of queue for pending melodies

// Accelerometer Task Configs
#define ACCEL_PERIOD_DELAY      5        // Time (ms) between ADC conversion starts
#define ACCEL_CENTER_SAMPLE     2060     // Average x-axis value when held flat (experimentally found)
#define ACCEL_CENTER_THRESH    (830 / 3) // Radius of "center" region (830 being experimentally found limit)

// Light Sensor Task Configs
#define LIGHT_PERIOD_DELAY      5   // Time (ms) between checking the light sensor
#define LIGHT_DARK_THRESH       100   // Largest lux value that registers as "dark"

// LCD Task Configs
#define LCD_QUEUE_SIZE          15 // Size of queue for pending images

// Player Task Configs
#define PLAYER_PERIOD_DELAY     32

// Enemy Task Configs
#define ENEMY_COUNT             7   // Number of enemies displayed per game
#define ENEMY_PERIOD_DELAY      1   // Time (ms) between updating enemies
#define ENEMY_Y_ENDGAME         66 // Y value enemies must reach in order to force the game to end
#define ENEMY_SHOOT_CHANCE      500 // Fractional chance a given enemy will shoot its laser (i.e. 1/x)

// Laser Task Configs
#define LASER_COUNT             10
#define LASER_PERIOD_DELAY      5

#endif /* CONFIG_H_ */

// Priority level check
#if CONTROL_PRIORITY <= GAME_PRIORITY
//#error "CONTROL PRIORITY must be greater than GAME_PRIORITY"
#endif
