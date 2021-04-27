/*
 * image.h
 *
 *  Created on: Apr 26, 2021
 *      Author: jesli
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

// Sample image.
LCD_t sampleImage(void);

// Enemy image.
LCD_t enemyImage(void);

#endif /* IMAGE_H_ */
