/*
 * imgprocess - Simple Image Processing Library for web
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

//Dependencies
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

typedef struct Picture {
    char path[128];
    int height;
    int width;
    int channel;
    int size;
    unsigned char* data;
} Pics;

typedef struct NewDimension {
    int resWidth;
    int resHeight;
    char name[32];
} Dime;

#ifndef FUNC_COL_H
#define FUNC_COL_H

#include "fun_col/functions.h"
#include "fun_col/colors.c"

#endif  

typedef enum {
    OPTION_RESIZE = 1,
    OPTION_CROP,
    OPTION_ROTATE,
    OPTION_QUIT
} Option;

typedef enum {
    OPTION_BACKGR = 1,
    OPTION_HERO,
    OPTION_BANNER,
    OPTION_BLOG,
    OPTION_LOGOREC,
    OPTION_LOGOSC,
    OPTION_FAVICON,
    OPTION_SOCIAL,
    OPTION_LIGHTBOX,
    OPTION_THUMBNAIL,
    OPTION_CUSTOM,
    OPTION_BACK
} Res;

int main(int argc, char *argv[]) {

    Pics img;
    Dime dime;
    int option = 0;
    yellow();
    printf("Welcome to simple Image Processing Library for web!\n");
    printf("In few quick steps your image will be ready for fast rendered website!\n");

    printf("Enter path to picture: ");
    if(fgets(img.path, sizeof(img.path), stdin) == NULL) {
        reset();
        perror("Error: Bad user input!\n");
        exit(EXIT_FAILURE);
    }
    if(img.path[strlen(img.path) - 1] != '\n') {
        reset();
        fprintf(stderr, "Input too long, consider increasing the buffer size.\n");
        exit(EXIT_FAILURE);
    }
    img.path[strcspn(img.path, "\n")] = '\0';

    int width, height, channels;
    unsigned char *imgPath = stbi_load(img.path, &width, &height, &channels, 0);

    if (imgPath == NULL) {
        reset();
        fprintf(stderr, "Error: Unable to load image from %s\n", img.path);
        free(img.data); // Free the allocated memory before exiting
        exit(EXIT_FAILURE);
    }

    img.data = (unsigned char*)malloc(width * height * channels);

    if (img.data == NULL) {
        reset();
        fprintf(stderr, "Error: Memory allocation failed!\n");
        stbi_image_free(imgPath);
        exit(EXIT_FAILURE);
    }

    // Copy the loaded image data to the allocated memory
    memcpy(img.data, imgPath, width * height * channels);

    stbi_image_free(imgPath);

    img.width = width;
    img.height = height;
    img.channel = channels;

    // printf("img.data = %s\n", img.data);
    printInfo(&img);

    while(option != OPTION_QUIT) {
        printMenu();
        printf("Option number: ");
        scanf("%d", &option);
        while(option < OPTION_RESIZE || option > OPTION_QUIT) {
            red();
            printf("Error number: %d\nOption must be in range [%d-%d]\n", errno, OPTION_RESIZE, OPTION_QUIT);
            yellow();
            printMenu();
            scanf("%d", &option);
        }
        switch(option) {
            case OPTION_RESIZE:
                resize(&img, &dime);
                break;
            case OPTION_CROP:
                crop();
                break;
            case OPTION_ROTATE:
                rotate();
                break;
            case OPTION_QUIT:
                quit();
                break;
        }
    }

    stbi_image_free(imgPath);
    free(img.data);

    reset();
    return 0;
}

void printInfo(Pics *img){
    printf("Your path to picture is %s\n", img->path);
    printf("Image informations:\n");
    printf("Width: %d px  /  Height: %d px  /  Channel: %d\n", img->width, img->height, img->channel);
}

void printResInfo(Dime *dime){
    printf("New dimensions: Width: %d px  /  Height: %d px\n", dime->resWidth, dime->resHeight);
}

void printMenu() {
    printf("    Choose option:\n");
    printf("\t%d Resize image\n", OPTION_RESIZE);
    printf("\t%d Crop image\n", OPTION_CROP);
    printf("\t%d Rotate image\n", OPTION_ROTATE);
    printf("\t%d Quit\n", OPTION_QUIT);
}

void printResMenu() {
    printf("    Resize options:\n");
    printf("\t%d Background image [1920 x 1080] \\ [16:9]\n", OPTION_BACKGR);
    printf("\t%d Hero image [1280 x 720] \\ [16:9]\n", OPTION_HERO);
    printf("\t%d Website banner [250 x 250] \\ [1:1]\n", OPTION_BANNER);
    printf("\t%d Blog image [1200 x 630] \\ [3:2]\n", OPTION_BLOG);
    printf("\t%d Logo rectangle [250 x 100] \\ [2:3]\n", OPTION_LOGOREC);
    printf("\t%d Logo square [100 x 100] \\ [1:1]\n", OPTION_LOGOSC);
    printf("\t%d Favicon [16 x 16] \\ [1:1]\n", OPTION_FAVICON);
    printf("\t%d Social Media Icon [32 x 32] \\ [1:1]\n", OPTION_SOCIAL);
    printf("\t%d Lightbox image (Full Screen) [1600 x 500] \\ [16:9]\n", OPTION_LIGHTBOX);
    printf("\t%d Thumbnail image [150 x 150t ] \\ [1:1]\n", OPTION_THUMBNAIL);
    printf("\t%d Custom resize image\n", OPTION_CUSTOM);
    printf("\t%d Back to main menu\n", OPTION_BACK);
}

void resize(Pics *img, Dime *dime) {
    int res = 0;
    int backToMainMenu = 0;
    char proc, sv;
    // printf("    Resize image\n");
    while(res != OPTION_BACK) { 
        printResMenu();
        printf("Resize option number: ");
        scanf("%d", &res);
        while(res < OPTION_BACKGR || res > OPTION_BACK) {
            red();
            printf("Error number: %d\nOption must be in range [%d-%d]\n", errno, OPTION_BACKGR, OPTION_BACK);
            yellow();
            printResMenu();
            scanf("%d", &res);
        }
        switch(res) {
            case OPTION_BACKGR:
                resizeBack(dime);
                break;
            case OPTION_HERO:
                resizeHero(dime);
                break;
            case OPTION_BANNER:
                resizeBanner(dime);
                break;
            case OPTION_BLOG:
                resizeBlog(dime);
                break;
            case OPTION_LOGOREC:
                resizeLogoRec(dime);
                break;
            case OPTION_LOGOSC:
                resizeLogoSc(dime);
                break;
            case OPTION_FAVICON:
                resizeFavicon(dime);
                break;
            case OPTION_SOCIAL:
                resizeSocial(dime);
                break;
            case OPTION_LIGHTBOX:
                resizeLight(dime);
                break;
            case OPTION_THUMBNAIL:
                resizeThumb(dime);
                break;
            case OPTION_CUSTOM:
                resizeCustom(dime);
                break;
            case OPTION_BACK:
                back(&backToMainMenu);
                // printResMenu();
                // // exit(0);
                break;
        }

        if(backToMainMenu != 1 && res != OPTION_BACK) {
            printf("Proceed with new dimensions? [Y,n] ");
            scanf("%s", &proc);

            if(proc != 'Y' && proc != 'y' && proc != 'n') { 
                printf("Error %d: Answer with 'Y' or 'n'!\n", errno);
            }
            if(proc == 'Y' || proc == 'y') {
                printResInfo(dime);
                printf("Save image? [Y,n] ");
                scanf("%s", &sv);
                if(sv != 'Y' && sv != 'n' && sv != 'y') {
                    printf("Error %d: Answer with 'Y' or 'n'!\n", errno);
                }
                if(sv == 'n') {
                    printf("Image NOT saved!\n");
                }
                if(sv == 'Y' || sv == 'y') {
                    int newSize = dime->resWidth * dime->resHeight * img->channel;
                    printf("New photo size is %d B\n", newSize);
                    unsigned char* resizedData = performResize(img->data, img->width, img->height, img->channel, dime->resWidth, dime->resHeight);

                    if (resizedData == NULL) {
                        printf("Error resizing image.\n");
                    } else {
                        stbi_image_free(img->data);

                        img->width = dime->resWidth;
                        img->height = dime->resHeight;
                        img->data = resizedData;
                        saveImage(img->data, img->width, img->height, img->channel, dime->name);
                    }
                }
            } else if(proc == 'n') {
                printInfo(img);
            }
        }
    }
}

int back(int *backToMainMenu) {
    printf("Going back!\n");
    *backToMainMenu = 1;
    return *backToMainMenu;
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

void saveImage(unsigned char* imageData, int width, int height, int channel, const char* filename) {
    int result = stbi_write_png(filename, width, height, channel, imageData, width * channel);

    if (result == 0) {
        printf("Error saving image to %s.\n", filename);
    } else {
        printf("Image saved to %s!\n", filename);
    }
}

Dime resizeCustom(Dime *dime) {

    printf("\nEnter WIDTH [pixels]: ");
    scanf("%d", &dime->resWidth);
    printf("Enter HEIGHT [pixels]: ");
    scanf("%d", &dime->resHeight);
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);
    
    return *dime;
}

Dime resizeBack(Dime *dime) {
    printf("Background\n");
    dime->resWidth = 1920;
    dime->resHeight = 1080;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeHero(Dime *dime) {
    printf("Hero image\n");
    dime->resWidth = 1280;
    dime->resHeight = 720;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeBanner(Dime *dime) {
    printf("Website banner\n");
    dime->resWidth = 250;
    dime->resHeight = 250;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeBlog(Dime *dime) {
    printf("Blog image\n");
    dime->resWidth = 1200;
    dime->resHeight = 630;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeLogoRec(Dime *dime) {
    printf("Logo rectangle\n");
    dime->resWidth = 250;
    dime->resHeight = 100;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeLogoSc(Dime *dime) {
    printf("Logo square\n");
    dime->resWidth = 100;
    dime->resHeight = 100;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeFavicon(Dime *dime) {
    printf("Favicon\n");
    dime->resWidth = 16;
    dime->resHeight = 16;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeSocial(Dime *dime) {
    printf("Social media icon\n");
    dime->resWidth = 32;
    dime->resHeight = 32;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeLight(Dime *dime) {
    printf("Lightbox image\n");
    dime->resWidth = 1600;
    dime->resHeight = 500;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

Dime resizeThumb(Dime *dime) {
    printf("Thumbnail image\n");
    dime->resWidth = 150;
    dime->resHeight = 150;
    printf("Enter new name [example.png]: ");
    scanf("%s", dime->name);

    return *dime;
}

void crop() {
    printf("    Crop image\nComming soon...\n");
}

void rotate() {
    printf("    Rotate image\nComming soon...\n");
}

void quit() {
    reset();
    printf("    Goodbye!\n");
    exit(0);
}

