/*
 * ipl - Simple Image Processing Library for web
 * Copyright (C) 2023 Vidoje Šević
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/gpl-3.0.txt>
 * */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include "prompt.h"
#include "cli.h"
#include "rotate.h"

#define PI 3.14159265358979323846f

const float angleRadCW = 90.0f * (PI / 180.0f);
const float angleRadACW = -90.0f * (PI / 180.0f);

unsigned char interpolateColor(Pics *img, float x, float y, int channel, char direction[BIT_NAME_SIZE]) {
    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float dx = x - x0;
    float dy = y - y0;

    unsigned char color00 = img->data[(y0 * img->width + x0) * img->channel + channel];
    unsigned char color01 = img->data[(y0 * img->width + x1) * img->channel + channel];
    unsigned char color10 = img->data[(y1 * img->width + x0) * img->channel + channel];
    unsigned char color11 = img->data[(y1 * img->width + x1) * img->channel + channel];

    unsigned char interpolatedColor = (1 - dx) * (1 - dy) * color00 + dx * (1 - dy) * color01 + \
                                      (1 - dx) * dy * color10 + dx * dy * color11;

    return interpolatedColor;
}


void rotationAlgoritm(unsigned char *imgData, Pics *img, Flip *flip, int newWidth, int newHeight) {
    for (int yDest = 0; yDest < newHeight; ++yDest) {
        for (int xDest = 0; xDest < newWidth; ++xDest) {
            float xSrc;
            float ySrc;

            // This is only for clockwise direction
            xSrc = (xDest - newWidth / 2) * flip->cosTheta + (yDest - newHeight / 2) * flip->sinTheta + img->width / 2;
            ySrc = -(xDest - newWidth / 2) * flip->sinTheta + (yDest - newHeight / 2) * flip->cosTheta + img->height / 2;

            if (xSrc >= 0 && xSrc < img->width && ySrc >= 0 && ySrc < img->height) {
                for (int c = 0; c < img->channel; ++c) {
                    imgData[yDest * newWidth * img->channel + xDest * img->channel + c] = interpolateColor(img, xSrc, ySrc, c, flip->direction);
                }
            }
        }
    }
}

void rotate(Pics *img, Flip *flip) {
    int newWidth;
    int newHeight;

    if (strcmp(flip->direction, "cw") == 0) {
        flip->cosTheta = cos(angleRadCW);
        flip->sinTheta = sin(angleRadCW);
        // printf("Rotation direction is clockwise\n");
    } else if (strcmp(flip->direction, "acw") == 0) {
        flip->cosTheta = cos(angleRadACW);
        flip->sinTheta = sin(angleRadACW);
        // printf("Rotation direction is anti-clockwise\n");
    } else {
        perror("Error direction input!\n");
        exit(EXIT_FAILURE);
    }

    newWidth = (int)(fabs(img->width * flip->cosTheta) + fabs(img->height * flip->sinTheta));
    newHeight = (int)(fabs(img->width * flip->sinTheta) + fabs(img->height * flip->cosTheta));

    unsigned char *rotatedImageData = (unsigned char*)malloc(newWidth * newHeight * img->channel);

    if (!rotatedImageData) {
        perror("Error: Memory alocation failed\n");
        free(rotatedImageData);
        exit(EXIT_FAILURE);
    }

    if (strcmp(flip->direction, "cw") == 0) {
        rotationAlgoritm(rotatedImageData, img, flip, newWidth, newHeight);
    } else if (strcmp(flip->direction, "acw") == 0) {
        // Anti-clockwise == 3 * clockwise
        for (int i = 0; i < 3; ++i) {
            rotationAlgoritm(rotatedImageData, img, flip, newWidth, newHeight);
        }
    }

    saveImage(rotatedImageData, newWidth, newHeight, img->channel, img->name, img->ext, img->quality);

}
