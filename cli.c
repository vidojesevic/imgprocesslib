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
#include <getopt.h>
#include "cli.h"
#include "ipl.h"
#include "resize.h"
#include "crop.h"
#include "rotate.h"

unsigned char *imgData;
char *fileSize;
Dime dime;
Flip flip;

void parseArguments(int argc, char *argv[], Pics *img, Input *input, Crop *crop) {
    if (argc == 2) {
        int i = argc - 1;
        if (strcmp(argv[i], "-h") != 0 && strcmp(argv[i], "--help") != 0 && strcmp(argv[i], "-v") != 0 && strcmp(argv[i], "--version") != 0) {
            printUnknownOption(argv[i], &i);
            printHelp();
            return;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printHelp();
            return;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printVersion();
            return;
        }
    } 

    if (argc > 2) {
        img->quality = 80;
        char *path = argv[1];
        strncpy(img->path, path, PATH_SIZE);
        getPath(img);

        allocateImg(img->path, input, &imgData);

        // Calculate image size in KiB
        getSize(img->path, input, &fileSize);

        // Copy the loaded image data to the allocated memory
        img->data = input->data;
        img->width = input->width;
        img->height = input->height;
        img->channel = input->channels;
        img->bitDepth = input->bitDepth;
        strcpy(img->bitInfo, input->bitInfo);
        img->size = fileSize;

        char *cli_option = argv[2];

        if (strcmp(cli_option, "-r") == 0) {
            resizeCLI(img, &dime, argc, argv);
        } else if (strcmp(cli_option, "-c") == 0) {
            cropCLI(img, argc, argv, crop);
        } else if (strcmp(cli_option, "-f") == 0) {
            rotateCLI(img, argc, argv, &flip);
        }

    }
    free(fileSize);
}

void jpgQuality(int argc, char *argv[], char ext[EXT_SIZE], int *quality) {
    if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) {
        for (int i = 0; i < argc; ++i) {
            if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
                *quality = atoi(argv[i + 1]);
            }
        }
    }
}

void rotateCLI(Pics *img, int argc, char *argv[], Flip *flip) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "cw") == 0 || strcmp(argv[i], "acw") == 0) {
            strncpy(flip->direction, argv[i], BIT_NAME_SIZE);
            strcpy(img->name, argv[i+1]);
            findOutExtension(img->name, img->ext);
            jpgQuality(argc, argv, img->ext, &img->quality);
        }
    }
    rotate(img, flip);
}

void cropCLI(Pics *img, int argc, char *argv[], Crop *crop) {
    
    for (int i = 0; i < argc - 4; ++i) {
        if (strcmp(argv[i], "-x") == 0 && strcmp(argv[i+2], "-y") == 0) {
            strcpy(img->name, argv[i+4]);
            findOutExtension(img->name, img->ext);
            jpgQuality(argc, argv, img->ext, &img->quality);
            crop->x = atoi(argv[i+1]);
            crop->y = atoi(argv[i+3]);
        } else if (strcmp(argv[i], "-b") == 0) {
            strcpy(img->name, argv[i+2]);
            findOutExtension(img->name, img->ext);
            jpgQuality(argc, argv, img->ext, &img->quality);
            crop->x = crop->y = atoi(argv[i+1]);
        }
    }

    if (crop->x >= img->width || crop->y >= img->height) {
        perror("Error: Values of X and Y must be less than original width and height!");
        exit(EXIT_FAILURE);
    }
    cropImage(img, crop);
}

void resizeCLI(Pics *img, Dime *dime, int argc, char *argv[]) {
    ResOptionMapping resOptions[] = {
        {BACK, "--background"},
        {HERO, "--hero"},
        {BANNER, "--web-banner"},
        {BLOG, "--blog"},
        {LOGOREC, "--logo-rec"},
        {LOGOSC, "--logo-sq"},
        {FAVICON, "--favicon"},
        {SOCIAL, "--social"},
        {LIGHTBOX, "--lightbox"},
        {THUMBNAIL, "--thumb"}
    };

    ResOptionFunction resFunction[] = {
        {"--background", resizeBack},
        {"--hero", resizeHero},
        {"--web-banner", resizeBanner},
        {"--blog", resizeBlog},
        {"--logo-rec", resizeLogoRec},
        {"--logo-sq", resizeLogoSc},
        {"--favicon", resizeFavicon},
        {"--social", resizeSocial},
        {"--lightbox", resizeLight},
        {"--thumb", resizeThumb}
    };

    size_t numResOption = sizeof(resOptions) / sizeof(resOptions[0]);
    img->quality = 80;
    for (int j = 0; j < argc - 1; ++j) {
        if (strcmp(argv[j], "--custom") == 0) {
            if (strcmp(argv[j+1], "-w") == 0 && strcmp(argv[j+3], "-h") == 0) {
                dime->resWidth = atoi(argv[j+2]);
                dime->resHeight = atoi(argv[j+4]);
                strcpy(dime->name, argv[j+5]);
                findOutExtension(dime->name, dime->ext);
                jpgQuality(argc, argv, dime->ext, &img->quality);
                resize(img, dime);
            }
        } else {
            for (int i = 0; i < numResOption; ++i) {
                if (strcmp(argv[j], resOptions[i].resOption) == 0) {
                    resFunction[i].resizeFunction(dime);
                    strcpy(dime->name, argv[j+1]);
                    findOutExtension(dime->name, dime->ext);
                    jpgQuality(argc, argv, dime->ext, &img->quality);
                    resize(img, dime);
                } 
            }
        }
    }
}    

void printHelp() {
    printf("Usage: ");
    printf("ipl [Options] / ipl <Input_path> <Operations> <Operation options> <Output_path>\n");
    printf("  Image Processing Library, simple processing tool for web developers\n");
    printf("\nOptions:\n");
    printf("  NULL\t\t\tIf the program is called without any arguments, it enters the prompt mode.\n");
    printf("  -h, --help\t\tGet help about usage of program\n");
    printf("  -v, --version\t\tGet information about version of program\n");
    printf("\nOperations:\n");
    printf("  -r\t\tResize image file\n");
    printf("  -c\t\tCrop a specific region from an image\n");
    printf("  -f\t\tRotate (flip) image file\n");
    printf("\nResize options:\n");
    printf("  --background\t\tBackground image [1920 x 1080] / [16:9]\n");
    printf("  --hero\t\tHero image [1280 x 720] / [16:9]\n");
    printf("  --web-banner\t\tWebsite banner [250 x 250] / [1:1]\n");
    printf("  --blog\t\tBlog image [1200 x 630] / [3:2]\n");
    printf("  --logo-rec\t\tLogo rectangle [250 x 100] / [2:3]\n");
    printf("  --logo-sq\t\tLogo square [100 x 100] / [1:1]\n");
    printf("  --favicon\t\tFavicon [16 x 16] / [1:1]\n");
    printf("  --social\t\tSocial Media Icon [32 x 32] / [1:1]\n");
    printf("  --lightbox\t\tLightbox image (Full Screen) [1600 x 500] / [16:9]\n");
    printf("  --thumb\t\tThumbnail image [150 x 150 ] / [1:1]\n");
    printf("  --custom\t\tCustom resize image\n");
    printf("Custom resize:\n");
    printf("  -w\t\t\twidth, provide number in px\n");
    printf("  -h\t\t\theight, provide number in px\n");
    printf("Special case:\n");
    printf("  -q\t\t\tIf working with .img/.jpeg output, you can provide quality number [1-100], else quality default is 80\n");
    printf("\nCrop options:\n");
    printf("  -x\t\t\tx-axis, provide number in px\n");
    printf("  -y\t\t\ty-axis, provide number in px\n");
    printf("  -b\t\t\tborder to crop, provide number in px\n");
    printf("\nRotate options:\n");
    printf("  cw\t\t\trotate image by 90 degrees clockwise\n");
    printf("  acw\t\t\trotate image by 90 degrees anti clockwise\n");
    printf("\nUsage examples:\n");
    printf("  ipl image.jpg -r --background output.jpg -q 80\n");
    printf("  ipl image.png -r --custom -w 1200 -h 900 output.png\n");
    printf("  ipl image.png -c -x 100 -y 100 output.png\n");
    printf("  ipl image.png -c -b 100 output.png\n");
    printf("  ipl image.jpg -f cw output.jpg\n");
    printf("\nBug Reporting:\n");
    printf("If you encounter any bugs or issues while using ipl, please report them to \n<vidoje.sevic.275.19@ict.edu.rs>. Your feedback is valuable in improving the tool.\n");
}

void printVersion() {
    printf("ipl (Image Processing Library) v0.23\n");
}

void printUnknownOption(char argv[NUM_ARGC], int *i) {
    printf("Unknown option: %s\n", argv);
    // (*i)++;
}

