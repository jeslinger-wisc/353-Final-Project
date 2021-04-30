/*
 * config.h
 *
 *  Created on: Apr 22, 2021
 *      Author: John Eslinger
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Group Priority Levels
#define CONTROL_PRIORITY        3       // Priority of game management tasks
#define GAME_PRIORITY           2       // Priority of game play tasks

// Menu Task Configs
#define MENU_KILL_DELAY         50      // Time (ms) idle task gets to garbage collect for between games

// Button Task Configs
#define BUTTON_DEBOUNCE_VALUE   10      // Consecutive presses needed to confirm "pressed" state
#define BUTTON_PERIOD_DELAY     5       // Time (ms) between button press samples

// Buzzer Task Configs
#define BUZZER_BPM              180     // Beats per minute to play melodies at
#define BUZZER_QUEUE_SIZE       25      // Size of queue for pending melodies

// Accelerometer Task Configs
#define ACCEL_PERIOD_DELAY      10      // Time (ms) between ADC conversion startups
#define ACCEL_CENTER_SAMPLE     2060    // Average x-axis value when held flat (experimentally found)
#define ACCEL_CENTER_THRESH     208     // Radius of "center" region of x-axis (+- 830 is max for sides)

// Light Sensor Task Configs
#define LIGHT_PERIOD_DELAY      10      // Time (ms) between light sensor samplings
#define LIGHT_DARK_THRESH       10      // Largest value (in lux) that classifies as "Dark"

// LCD Task Configs
#define LCD_QUEUE_SIZE          25      // Size of queue for pending images to draw

// Player Task Configs
#define PLAYER_PERIOD_DELAY     10      // Time (ms) between player action updates

// Enemy Task Configs
#define ENEMY_COUNT             21      // Number of enemies displayed per game
#define ENEMY_PER_ROW           7       // Number of enemies to display per row
#define ENEMY_PERIOD_DELAY      30      // Time (ms) between enemy action updates (updated in one large group)
#define ENEMY_Y_ENDGAME         100     // Y value enemies must reach in order to force the game to end
#define ENEMY_SHOOT_CHANCE      200     // 1/x chance an enemy will shoot its laser (while being updated)

// Laser Task Configs
#define LASER_COUNT             20      // Number of laser that canbe simultaneously displayed
#define LASER_PERIOD_DELAY      10      // Time (ms) between laser action updates (updated in one large group)

#endif /* CONFIG_H_ */
