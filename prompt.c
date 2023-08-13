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
                resizePrompt(img, dime);
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

void resizePrompt(Pics *img, Dime *dime) {
    int res = 0;
    int backToMainMenu = 0;
    char proc, sv;
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
                clearInputBuffer();
                break;
            case HERO:
                resizeHero(dime);
                clearInputBuffer();
                break;
            case BANNER:
                resizeBanner(dime);
                clearInputBuffer();
                break;
            case BLOG:
                resizeBlog(dime);
                clearInputBuffer();
                break;
            case LOGOREC:
                resizeLogoRec(dime);
                clearInputBuffer();
                break;
            case LOGOSC:
                resizeLogoSc(dime);
                clearInputBuffer();
                break;
            case FAVICON:
                resizeFavicon(dime);
                clearInputBuffer();
                break;
            case SOCIAL:
                resizeSocial(dime);
                clearInputBuffer();
                break;
            case LIGHTBOX:
                resizeLight(dime);
                clearInputBuffer();
                break;
            case THUMBNAIL:
                resizeThumb(dime);
                clearInputBuffer();
                break;
            case CUSTOM:
                clearInputBuffer();
                resizeCustom(dime);
                break;
            case BACK:
                back(&backToMainMenu);
                return;
                break;
        }

        getName(dime);

        if (backToMainMenu != 1 && res != BACK) {
            printf("Proceed with new dimensions? [Y,n] ");
            scanf("%s", &proc);

            // printf("%d", dime->name);
            if (proc != 'Y' && proc != 'y' && proc != 'n') {
                printf("Error %d: Answer with 'Y' or 'n'!\n", errno);
            }
            if (proc == 'Y' || proc == 'y') {
                printResInfo(dime);
                printf("Save image? [Y,n] ");
                scanf("%s", &sv);
                clearInputBuffer();
                if (sv != 'Y' && sv != 'n' && sv != 'y') {
                    printf("Error %d: Answer with 'Y' or 'n'!\n", errno);
                }
                if (sv == 'n') {
                    printf("Image NOT saved!\n");
                }
                if (sv == 'Y' || sv == 'y') {
                    resize(img, dime);
                }
            }
        }
        if (proc == 'n') {
            printInfo(img);
        }
    }

}

void checkName(Pics* img) {
    if (fgets(img->path, PATH_SIZE, stdin) == NULL) {
        perror("Error: Bad user input!\n");
        exit(EXIT_FAILURE);
    }

    size_t length = strlen(img->path);
    if (length > 0 && img->path[length - 1] == '\n') {
        img->path[length - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long, consider increasing the buffer size.\n");
        // clearInputBuffer();
        exit(EXIT_FAILURE);
    }
}

void printInfo(Pics *img){
    printf("Image informations:\n");
    printf("Dimensions: %d x %d | %s | Size: %s\n", img->width, img->height, img->bitInfo, img->size);
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

int back(int *backToMainMenu) {
    printf("Going back!\n");
    *backToMainMenu = 1;
    return *backToMainMenu;
}

void quit() {
    printf("    Goodbye!\n");
    exit(0);
}
