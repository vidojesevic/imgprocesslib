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

// UNIX PORN

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "ipl.h"
#include "cli.h"
#include "prompt.h"
#include "resize.h"

//Dependencies
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main(int argc, char *argv[]) {
    unsigned char *imgPath;
    Pics img;
    Dime dime;

    if(argc > 1) {
        printf("You just enter cli mode\n");
        const Action action;
        parseArguments(argc, argv);
        performAction(&action);
    } else {
        int option = 0;
        printf("Welcome to simple Image Processing Library for web!\n");
        printf("In few quick steps your image will be ready for fast rendered website!\n");

        printf("Enter path to picture: ");
        if (fgets(img.path, sizeof(img.path), stdin) == NULL) {
            perror("Error: Bad user input!\n");
            exit(EXIT_FAILURE);
        }
        if (img.path[strlen(img.path) - 1] != '\n') {
            fprintf(stderr, "Input too long, consider increasing the buffer size.\n");
            exit(EXIT_FAILURE);
        }
        img.path[strcspn(img.path, "\n")] = '\0';

        int width, height, channels, bitDepth;
        char *fileSize;
        // printf("DEBUGING FILESIZE: %ld\n", fileSize);
        char bitInfo[64];
        imgPath = stbi_load(img.path, &width, &height, &channels, 0);

        if (imgPath == NULL) {
            fprintf(stderr, "Error loading image: %s from '%s'!\n", stbi_failure_reason(), img.path);
            free(img.data); // Free the allocated memory before exiting
            exit(EXIT_FAILURE);
        }

        img.data = (unsigned char*)malloc(width * height * channels);

        if (img.data == NULL) {
            fprintf(stderr, "Error: Memory allocation failed!\n");
            stbi_image_free(imgPath);
            exit(EXIT_FAILURE);
        }

        // Calculate image size in KiB

        // Detect bit per color
        if (channels == 1) {
            bitDepth = 1;
        } else if (channels == 3) {
            bitDepth = 8;
        } else if (channels == 4) {
            bitDepth = 32;
        } else {
            // For other channel counts, we'll check the actual bit depth using stbi_info
            int bpp = stbi_info(img.path, &width, &height, &bitDepth);
            if (bpp == 8) {
                bitDepth = 8;
            }
            else if (bpp == 16) {
                bitDepth = 16;
            }
            else if (bpp == 24) {
                bitDepth = 24;
            }
            else {
                bitDepth = 0;
            }
        }

        // Copy the loaded image data to the allocated memory
        memcpy(img.data, imgPath, width * height * channels);

        stbi_image_free(imgPath);

        if (bitDepth == 1) {
            strcpy(bitInfo, "1-bit/color (1 channel).");
        } else if (bitDepth == 8) {
            strcpy(bitInfo, "8-bit/color (RGB or grayscale, 3 channels)");
        } else if (bitDepth == 16) {
            strcpy(bitInfo, "16-bit/color (RGB, 3 channels)");
        } else if (bitDepth == 24) {
            strcpy(bitInfo, "24-bit/color (RGB, 3 channels)");
        } else if (bitDepth == 32) {
            strcpy(bitInfo, "32-bit/color (RGBA, 4 channels)");
        } else {
            strcpy(bitInfo, "Unable to determine the bit depth of the image!");
        }

        fileSize = calcSize(img.path);

        img.width = width;
        img.height = height;
        img.channel = channels;
        img.bitDepth = bitDepth;
        strcpy(img.bitInfo, bitInfo);
        img.size = fileSize;

        // printf("img.data = %s\n", img.data);
        printInfo(&img);

        promptMode(&img, &dime, option);

    }

    stbi_image_free(imgPath);
    free(img.data);

    return 0;
}

int back(int *backToMainMenu) {
    printf("Going back!\n");
    *backToMainMenu = 1;
    return *backToMainMenu;
}

void performFreeing(Pics *img) {
    stbi_image_free(img->data);
}

void saveImage(unsigned char* imageData, int width, int height, int channel, const char* filename) {
    int result = stbi_write_png(filename, width, height, channel, imageData, width * channel);
    char *sizeChar;

    if (result == 0) {
        printf("Error saving image to %s.\n", filename);
    } else {
        sizeChar = calcSize(filename);
        printf("Image saved to %s! Size: %s!\n", filename, sizeChar);
    }
}

char* calcSize(const char* filename) {
    long fileSize;
    char sizeChar[32];
    struct stat st;
    
    if (stat(filename, &st) == 0) {
        fileSize = st.st_size;
    } else {
        fileSize = 0;
    }

    if (fileSize > 1024) {
        fileSize = fileSize / 1024;
        snprintf(sizeChar, sizeof(sizeChar), "%ld KiB", fileSize);
    }
    else {
        snprintf(sizeChar, sizeof(sizeChar), "%ld B", fileSize);
    }

    return strdup(sizeChar);
}

void crop() {
    printf("    Crop image\nComming soon...\n");
}

void rotate() {
    printf("    Rotate image\nComming soon...\n");
}

void quit() {
    printf("    Goodbye!\n");
    exit(0);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

