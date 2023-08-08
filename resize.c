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

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

void resize(Pics *img, Dime *dime) {
    int res = 0;
    int backToMainMenu = 0;
    char proc, sv;
    // printf("    Resize image\n");
    while (res != BACK) { 
        printResMenu();
        printf("Resize option number: ");
        scanf("%d", &res);
        while (res < BACKGR || res > BACK) {
            printf("Error number: %d\nOption must be in range [%d-%d]\n", errno, BACKGR, BACK);
            printResMenu();
            scanf("%d", &res);
        }
        switch (res) {
            case BACKGR:
                resizeBack(dime);
                break;
            case HERO:
                resizeHero(dime);
                break;
            case BANNER:
                resizeBanner(dime);
                break;
            case BLOG:
                resizeBlog(dime);
                break;
            case LOGOREC:
                resizeLogoRec(dime);
                break;
            case LOGOSC:
                resizeLogoSc(dime);
                break;
            case FAVICON:
                resizeFavicon(dime);
                break;
            case SOCIAL:
                resizeSocial(dime);
                break;
            case LIGHTBOX:
                resizeLight(dime);
                break;
            case THUMBNAIL:
                resizeThumb(dime);
                break;
            case CUSTOM:
                resizeCustom(dime);
                break;
            case BACK:
                back(&backToMainMenu);
                // printResMenu();
                // // exit(0);
                break;
        }

        if (backToMainMenu != 1 && res != BACK) {
            printf("Proceed with new dimensions? [Y,n] ");
            scanf("%s", &proc);

            if (proc != 'Y' && proc != 'y' && proc != 'n') { 
                printf("Error %d: Answer with 'Y' or 'n'!\n", errno);
            }
            if (proc == 'Y' || proc == 'y') {
                printResInfo(dime);
                printf("Save image? [Y,n] ");
                scanf("%s", &sv);
                if (sv != 'Y' && sv != 'n' && sv != 'y') {
                    printf("Error %d: Answer with 'Y' or 'n'!\n", errno);
                }
                if (sv == 'n') {
                    printf("Image NOT saved!\n");
                }
                if (sv == 'Y' || sv == 'y') {
                    unsigned char* resizedData = performResize(img->data, img->width, img->height, img->channel, dime->resWidth, dime->resHeight);

                    if (resizedData == NULL) {
                        printf("Error resizing image.\n");
                    } else {
                        performFreeing(img);

                        img->width = dime->resWidth;
                        img->height = dime->resHeight;
                        img->data = resizedData;
                        saveImage(img->data, img->width, img->height, img->channel, dime->name);
                    }
                }
            } else if (proc == 'n') {
                printInfo(img);
            }
        }
    }
}

Dime resizeCustom(Dime *dime) {
    printf("Custom dimensions");
    clearInputBuffer();

    getWidth(dime);
    getHeight(dime);
    getName(dime);

    return *dime;
}

Dime resizeBack(Dime *dime) {
    printf("Background image\n");
    dime->resWidth = 1920;
    dime->resHeight = 1080;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeHero(Dime *dime) {
    printf("Background image\n");
    dime->resWidth = 1280;
    dime->resHeight = 720;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeBanner(Dime *dime) {
    printf("Website banner\n");
    dime->resWidth = 250;
    dime->resHeight = 250;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeBlog(Dime *dime) {
    printf("Blog image\n");
    dime->resWidth = 1200;
    dime->resHeight = 630;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeLogoRec(Dime *dime) {
    printf("Logo rectangle\n");
    dime->resWidth = 250;
    dime->resHeight = 100;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeLogoSc(Dime *dime) {
    printf("Logo square\n");
    dime->resWidth = 100;
    dime->resHeight = 100;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeFavicon(Dime *dime) {
    printf("Favicon\n");
    dime->resWidth = 16;
    dime->resHeight = 16;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeSocial(Dime *dime) {
    printf("Social media icon\n");
    dime->resWidth = 32;
    dime->resHeight = 32;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeLight(Dime *dime) {
    printf("Lightbox image\n");
    dime->resWidth = 1600;
    dime->resHeight = 500;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

Dime resizeThumb(Dime *dime) {
    printf("Thumbnail image\n");
    dime->resWidth = 150;
    dime->resHeight = 150;

    clearInputBuffer();
    getName(dime);

    return *dime;
}

unsigned char* performResize(unsigned char* imageData, int width, int height, int channel, int newWidth, int newHeight) {
    // Allocate memory for the resized image
    unsigned char* resizedData = malloc(newWidth * newHeight * channel);

    // Resize the image using stbir_resize_uint8
    int result = stbir_resize_uint8(imageData, width, height, 0, resizedData, newWidth, newHeight, 0, channel);

    // Check if the resizing was successful
    if (result == 0) {
        free(resizedData);
        return NULL; // Return NULL to indicate failure
    }
    printf("Successfully changed dimensions! ");

    return resizedData; // Return the resized image data
}
