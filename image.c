/*
 * image.c
 *
 *  Created on: Apr 26, 2021
 *      Author: jesli
 */

#include "image.h"

// Sample Image.
const uint8_t space_shipBitmaps[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //
    0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, //                       ########
    0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 0x00, //                     ############
    0x00, 0x00, 0x38, 0xFF, 0x80, 0x00, 0x00, //                   ###   #########
    0x00, 0x00, 0x60, 0xFF, 0xC0, 0x00, 0x00, //                  ##     ##########
    0x00, 0x00, 0x41, 0xFF, 0xE0, 0x00, 0x00, //                  #     ############
    0x00, 0x00, 0xC1, 0xFF, 0xE0, 0x00, 0x00, //                 ##     ############
    0x00, 0x00, 0x83, 0xFF, 0xF0, 0x00, 0x00, //                 #     ##############
    0x00, 0x01, 0x83, 0xFF, 0xF0, 0x00, 0x00, //                ##     ##############
    0x00, 0x05, 0x83, 0xFF, 0xF2, 0x00, 0x00, //              # ##     ##############  #
    0x00, 0x3D, 0x83, 0xFF, 0xF7, 0x80, 0x00, //           #### ##     ############## ####
    0x00, 0xFC, 0x83, 0xFF, 0xF7, 0xE0, 0x00, //         ######  #     ############## ######
    0x01, 0xFC, 0xC3, 0xFF, 0xE7, 0xF8, 0x00, //        #######  ##    #############  ########
    0x07, 0xFE, 0x63, 0xFF, 0xCF, 0xFE, 0x00, //      ##########  ##   ############  ###########
    0x0F, 0xFF, 0x3F, 0xFF, 0x8F, 0xFF, 0x00, //     ############  ###############   ############
    0x1F, 0xFF, 0x8F, 0xFE, 0x1F, 0xFF, 0x80, //    ##############   ###########    ##############
    0x3C, 0x7F, 0xC0, 0x00, 0x7F, 0xC7, 0xC0, //   ####   #########               #########   #####
    0x7C, 0x3F, 0xF0, 0x03, 0xFF, 0x87, 0xE0, //  #####    ##########          ###########    ######
    0x3E, 0x3F, 0xFF, 0xFF, 0xFF, 0x87, 0xC0, //   #####   ###############################    #####
    0x1F, 0xF8, 0x7F, 0xFF, 0x87, 0xFF, 0x80, //    ##########    ################    ############
    0x0F, 0xFC, 0x3E, 0x1F, 0x87, 0xFF, 0x00, //     ##########    #####    ######    ###########
    0x07, 0xFE, 0x7E, 0x1F, 0x8F, 0xFE, 0x00, //      ##########  ######    ######   ###########
    0x01, 0xFF, 0xFF, 0x1F, 0xFF, 0xFC, 0x00, //        #################   ###################
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, //         #####################################
    0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, //           ##################################
    0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, //            ###############################
    0x00, 0x07, 0xFF, 0xFF, 0xFF, 0x00, 0x00, //              ###########################
    0x00, 0x07, 0xFF, 0xFF, 0xFF, 0x00, 0x00, //              ###########################
    0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0x80, 0x00, //            ##############################
    0x00, 0x1F, 0x81, 0xFE, 0x1F, 0xC0, 0x00, //            ######      ########    #######
    0x00, 0x1F, 0x00, 0x00, 0x07, 0x80, 0x00, //            #####                     ####
    0x00, 0x0E, 0x00, 0x00, 0x03, 0x80, 0x00, //             ###                       ###
    0x00, 0x0C, 0x00, 0x00, 0x03, 0x80, 0x00, //             ##                        ###
    0x00, 0x0C, 0x00, 0x00, 0x01, 0x80, 0x00, //             ##                         ##
    0x00, 0x0C, 0x00, 0x00, 0x01, 0x80, 0x00, //             ##                         ##
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //
};
LCD_t sampleImage = { .x = LCD_HORIZONTAL_MAX / 2,
                      .y = LCD_VERTICAL_MAX / 2,
                      .image_width_pixels = 52,
                      .image_height_pixels = 36,
                      .image = space_shipBitmaps,
                      .fColor = LCD_COLOR_RED,
                      .bColor = LCD_COLOR_BLACK
};
