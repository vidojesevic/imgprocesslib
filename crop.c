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
#include "prompt.h"
#include "cli.h"

unsigned char *stbi_load(const char* filename, int *x, int *y, int *n, int desiredChannels);

void cropImage(Pics *img, Crop *crop) {

    printf("X: %d, Y: %d, border: %d\n", crop->x, crop->y, crop->border);
    int newWidth = img->width - 2 * crop->x;
    int newHeight = img->height - 2 * crop->y;

    if (newWidth <= 0 || newHeight <= 0) {
        perror("Error: invalid crop dimensions!\n");
        exit(EXIT_FAILURE);
    }
    // printf("New width: %d,new height: %d\n", newWidth, newHeight);
    unsigned char *croppedData = (unsigned char*)malloc(newWidth * newHeight * img->channel);

    if (croppedData == NULL) {
        perror("Error: Memory alocation failed!");
        exit(EXIT_FAILURE);
    }

    char outputFilename[PATH_SIZE]; // Define a buffer for the output filename
    snprintf(outputFilename, sizeof(outputFilename), "cropped_%s", img->name);

    for (int y = crop->y; y < crop->y + newHeight; y++) {
        for (int x = crop->x; x < crop->x + newWidth; x++) {
            for (int c = 0; c < img->channel; c++) {
                croppedData[(y - crop->y) * newWidth * img->channel + (x - crop->x) * img->channel + c] =
                    img->data[y * img->width * img->channel + x * img->channel + c];
            }
        }
    }

    printf("Jel ovde problem?");
    saveCroppedImage(croppedData, newWidth, newHeight, img->channel, outputFilename, img->ext, img->quality);

    free(img->data);
    img->data = croppedData;
    img->width = newWidth;
    img->height = newHeight;
}

// for (int y = crop->y; y < img->height; y++) {
//     for (int x = crop->x; x < img->width; x++) {
//         for (int c = 0; c < img->channel; c++) {
//             croppedData[(y - crop->y) * newWidth * img->channel + (x - crop->x) * img->channel + c] =
//                 img->data[y * img->width * img->channel + x * img->channel + c];
//         }
//     }
// }

