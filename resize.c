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
#include "resize.h"
#include "prompt.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

void stbi_image_free(void *data);

void resize(Pics *img, Dime *dime) {
    unsigned char* resizedData = performResize(img->data, img->width, img->height, img->channel, dime->resWidth, dime->resHeight);

    if (resizedData == NULL) {
        printf("Error resizing image.\n");
    } 
    if (resizedData != NULL) {
        // free allocated memory from allocateImg()
        stbi_image_free(img->data);

        img->width = dime->resWidth;
        img->height = dime->resHeight;
        img->data = resizedData;
        saveImage(img->data, img->width, img->height, img->channel, dime->name, dime->ext, img->quality);
    }
}

Dime resizeCustom(Dime *dime) {
    printf("Enter WIDTH [pixels]: ");
    getWidth(dime);

    printf("Enter HEIGHT [pixels]: ");
    getHeight(dime);

    return *dime;
}

Dime resizeBack(Dime *dime) {
    dime->resWidth = 1920;
    dime->resHeight = 1080;

    return *dime;
}

Dime resizeHero(Dime *dime) {
    dime->resWidth = 1280;
    dime->resHeight = 720;

    return *dime;
}

Dime resizeBanner(Dime *dime) {
    dime->resWidth = 250;
    dime->resHeight = 250;

    return *dime;
}

Dime resizeBlog(Dime *dime) {
    dime->resWidth = 1200;
    dime->resHeight = 630;

    return *dime;
}

Dime resizeLogoRec(Dime *dime) {
    dime->resWidth = 250;
    dime->resHeight = 100;

    return *dime;
}

Dime resizeLogoSc(Dime *dime) {
    dime->resWidth = 100;
    dime->resHeight = 100;

    return *dime;
}

Dime resizeFavicon(Dime *dime) {
    dime->resWidth = 16;
    dime->resHeight = 16;

    return *dime;
}

Dime resizeSocial(Dime *dime) {
    dime->resWidth = 32;
    dime->resHeight = 32;

    return *dime;
}

Dime resizeLight(Dime *dime) {
    dime->resWidth = 1600;
    dime->resHeight = 500;

    return *dime;
}

Dime resizeThumb(Dime *dime) {
    dime->resWidth = 150;
    dime->resHeight = 150;

    return *dime;
}

unsigned char* performResize(unsigned char *imageData, int width, int height, int channel, int newWidth, int newHeight) {
    // Allocate memory for the resized image
    unsigned char* resizedData = malloc(newWidth * newHeight * channel);

    // Resize the image using stbir_resize_uint8
    int result = stbir_resize_uint8(imageData, width, height, 0, resizedData, newWidth, newHeight, 0, channel);

    // Check if the resizing was successful
    if (result == 0) {
        free(resizedData);
        return NULL; // Return NULL to indicate failure
    }
    printf("Successfully changed dimensions!\n");

    return resizedData; // Return the resized image data
}
