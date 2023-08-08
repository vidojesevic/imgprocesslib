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
#include "resize.h"

void promptMode(Pics *img, Dime *dime, int option) {
    while (option != QUIT) {
        printMenu();
        printf("Option number: ");
        scanf("%d", &option);
        while (option < RESIZE || option > QUIT) {
            printf("Error number: %d\nOption must be in range [%d-%d]\n", errno, RESIZE, QUIT);
            printMenu();
            scanf("%d", &option);
        }
        switch (option) {
            case RESIZE:
                resize(img, dime);
                break;
            case CROP:
                crop();
                break;
            case ROTATE:
                rotate();
                break;
            case QUIT:
                quit();
                break;
        }
    }
}

void printInfo(Pics *img){
    printf("Image informations:\n");
    printf("Width: %d px | Height: %d px | %s | Size: %s\n", img->width, img->height, img->bitInfo, img->size);
}

void printResInfo(Dime *dime){
    printf("New dimensions: Width: %d px | Height: %d px\n", dime->resWidth, dime->resHeight);
}

void printMenu() {
    printf("    Choose option:\n");
    printf("\t%d Resize image\n", RESIZE);
    printf("\t%d Crop image\n", CROP);
    printf("\t%d Rotate image\n", ROTATE);
    printf("\t%d Quit\n", QUIT);
}

void printResMenu() {
    printf("    Resize options:\n");
    printf("\t%d Background image [1920 x 1080] \\ [16:9]\n", BACKGR);
    printf("\t%d Hero image [1280 x 720] \\ [16:9]\n", HERO);
    printf("\t%d Website banner [250 x 250] \\ [1:1]\n", BANNER);
    printf("\t%d Blog image [1200 x 630] \\ [3:2]\n", BLOG);
    printf("\t%d Logo rectangle [250 x 100] \\ [2:3]\n", LOGOREC);
    printf("\t%d Logo square [100 x 100] \\ [1:1]\n", LOGOSC);
    printf("\t%d Favicon [16 x 16] \\ [1:1]\n", FAVICON);
    printf("\t%d Social Media Icon [32 x 32] \\ [1:1]\n", SOCIAL);
    printf("\t%d Lightbox image (Full Screen) [1600 x 500] \\ [16:9]\n", LIGHTBOX);
    printf("\t%d Thumbnail image [150 x 150 ] \\ [1:1]\n", THUMBNAIL);
    printf("\t%d Custom resize image\n", CUSTOM);
    printf("\t%d Back to main menu\n", BACK);
}

void getWidth(Dime *dime) {
    char input[256];
    int validInput = 0;

    while (!validInput) {
        printf("\nEnter WIDTH [pixels]: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Error reading input.");
            exit(EXIT_FAILURE);
        }
        if (sscanf(input, "%d", &dime->resWidth) != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        validInput = 1;
    }
}

void getHeight(Dime *dime) {
    char input[256];
    int validInput = 0;

    while (!validInput) {
        printf("\nEnter HEIGHT [pixels]: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Error reading input.");
            exit(EXIT_FAILURE);
        }
        if (sscanf(input, "%d", &dime->resHeight) != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        validInput = 1;
    }
}

void getName(Dime *dime) {
    char input[256];
    int validInput = 0;

    while (!validInput) {
        printf("Enter new name [example.png]: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Error reading input.");
            exit(EXIT_FAILURE);
        }
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) {
            printf("Invalid input. Please enter a valid string.\n");
            validInput = 1;
        } else {
            strncpy(dime->name, input, sizeof(dime->name) - 1);
            validInput = 1;
        }
    }
}
