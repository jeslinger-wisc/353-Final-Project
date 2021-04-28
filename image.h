/*
 * image.h
 *
 *  Created on: Apr 26, 2021
 *      Author: John Eslinger
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "commonHeaders.h"
#include "lcd.h"

//STORE LCD IMAGE DATA IN THIS STRUCT, TO BE TRANSMITTED OVER QUEUES
typedef struct LCD_Struct {
      uint16_t x;
     uint16_t y;
     uint16_t image_width_pixels;
     uint16_t image_height_pixels;
     const uint8_t *image;
     uint16_t fColor;
     uint16_t bColor;
} LCD_t;

/*
 * Generates and returns new enemy LCD type.
 *
 * Returns LCD type with basic enemy image information
 */
LCD_t newEnemyImage(void);

/*
 * Generates and returns new laser LCD type.
 *
 * Returns LCD type with laser basic information
 */
LCD_t newLaserImage(void);

// Global constant LCD_t for game's title image.
extern const LCD_t titleImage;

// Global constant LCD_t for game's starting instructions image.
extern const LCD_t startImage;

// Global constant LCD_t for game's lose image.
extern const LCD_t loseImage;

// Global constant LCD_t for game's win image.
extern const LCD_t winImage;

#endif /* IMAGE_H_ */
