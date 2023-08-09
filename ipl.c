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
#include <ctype.h>
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
    unsigned char *imgData;
    Input input;
    Pics img;
    Dime dime;
    char *fileSize;
    // int bitDepth;

    if (argc > 1) {
        printf("You just enter cli mode\n");
        // const Action action;
        // strncpy(action.imagePath, "../img/ban.png");
        if (argc != 5 && argc != 9) {
            fprintf(stderr, "Usage: %s <input_path> <operation> ...\n", argv[0]);
            printf("Try 'ipl --help' or 'ipl --usage' for more information.");
            return 1; // Non-zero exit code indicating an error
        }
        parseArguments(argc, argv, img, input);
        // performAction(&action);
    } else {
        int option = 0;
        printf("Welcome to simple Image Processing Library for web!\n");
        printf("In few quick steps your image will be ready for fast rendered website!\n");

        printf("Enter path to picture: ");

        // Checking if path is correct
        getPath(&img);

        // printf("DEBUGING FILESIZE: %ld\n", fileSize);
        // char bitInfo[64];

        allocateImg(img.path, &input, &imgData);
        // Calculate image size in KiB

        getSize(img.path, &input, input.channels, &fileSize);

        // Copy the loaded image data to the allocated memory
        img.data = input.data;
        img.width = input.width;
        img.height = input.height;
        img.channel = input.channels;
        img.bitDepth = input.bitDepth;
        strcpy(img.bitInfo, input.bitInfo);
        img.size = fileSize;

        // printf("img.data = %s\n", img.data);
        printInfo(&img);
        
        size_t m = strlen(img.ext);
        if (m == 0) {
            printf("NISTA");
        }

        promptMode(&img, &dime, option);

    }

    stbi_image_free(imgData);
    free(input.data);

    return 0;
}

Pics* getPath(Pics *img) {
    if (fgets(img->path, PATH_SIZE, stdin) == NULL) {
        perror("Error: Bad user input!\n");
        exit(EXIT_FAILURE);
    }
    size_t length = strlen(img->path);
    if (length > 0 && img->path[length - 1] == '\n') {
        img->path[length - 1] = '\0';
    } else {
        fprintf(stderr, "Input too long, consider increasing the buffer size.\n");
        clearInputBuffer();
        exit(EXIT_FAILURE);
    }

    if (strlen(img->path) == 0) {
        fprintf(stderr, "No input provided. Please enter a valid image path.\n");
        exit(EXIT_FAILURE);
    }
    
    findOutExtension(img->path, img->ext);

    // Get extension

    return img;
}

void findOutExtension(char *path, char extension[EXT_SIZE]){
    extension[0] = '\0';
    const char *supportedExtensions[] = {"jpg", "jpeg", "png"};
    int isValidExtension = 0;

    char *ext = strrchr(path, '.');
    if (ext) {
        ext++;
        for (int i = 0; i < sizeof(supportedExtensions) / sizeof(supportedExtensions[0]); ++i) {
            if (strcmp(ext, supportedExtensions[i]) == 0) {
                strncpy(extension, supportedExtensions[i], EXT_SIZE);
                printf("Extension %s\n", extension);
                break;
            }
        }
    }
    if (!ext) {
        isValidExtension = 1;
    }

    if (isValidExtension != 0) {
        fprintf(stderr, "Unsupported file extension. Supported formats: .jpg, .jpeg, .png\n");
        exit(EXIT_FAILURE);
    }
}

void allocateImg(char *path, Input *input, unsigned char **imgData) {
    *imgData = stbi_load(path, &input->width, &input->height, &input->channels, 0);

    if (*imgData == NULL) {
        fprintf(stderr, "Error loading image: %s from '%s'!\n", stbi_failure_reason(), path);
        free(input->data); // Free the allocated memory before exiting
        exit(EXIT_FAILURE);
    } else {
        input->data = (unsigned char*)malloc(input->width * input->height * input->channels);
    }

    if (input->data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed!\n");
        stbi_image_free(*imgData);
        exit(EXIT_FAILURE);
    }

    memcpy(input->data, *imgData, input->width * input->height * input->channels);

    stbi_image_free(*imgData);
}

void getSize(char *path, Input *input, int channels, char **fileSize) {
    // Detect bit per color
    int bpp = stbi_info(path, &input->width, &input->height, &input->bitDepth);

    if (channels == 1) {
        input->bitDepth = 1;
    } else if (channels == 3) {
        input->bitDepth = 8;
    } else if (channels == 4) {
        input->bitDepth = 32;
    } else {
        // For other channel counts, we'll check the actual bit depth using stbi_info
        if (bpp == 8) {
            input->bitDepth = 8;
        }
        else if (bpp == 16) {
            input->bitDepth = 16;
        }
        else if (bpp == 24) {
            input->bitDepth = 24;
        }
        else {
            input->bitDepth = 0;
        }
    }

    if (input->bitDepth == 1) {
    }
    if (input->bitDepth == 8) {
        strcpy(input->bitInfo, "8-bit/color (RGB or grayscale, 3 channels)");
    }
    if (input->bitDepth == 16) {
        strcpy(input->bitInfo, "16-bit/color (RGB, 3 channels)");
    }
    if (input->bitDepth == 24) {
        strcpy(input->bitInfo, "24-bit/color (RGB, 3 channels)");
    }
    if (input->bitDepth == 32) {
        strcpy(input->bitInfo, "32-bit/color (RGBA, 4 channels)");
    }

    *fileSize = calcSize(path);
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

    if (fileSize > KiloByte) {
        fileSize = fileSize / KiloByte;
        snprintf(sizeChar, sizeof(sizeChar), "%ld KiB", fileSize);
    }
    else {
        snprintf(sizeChar, sizeof(sizeChar), "%ld B", fileSize);
    }

    return strdup(sizeChar);
}

void getWidth(Dime *dime) {
    char input[256];
    int validInput = 0;

    while (!validInput) {
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

void saveResizedImage(unsigned char* imageData, int width, int height, int channel, const char* filename) {
    int result = stbi_write_png(filename, width, height, channel, imageData, width * channel);
    char *sizeChar;

    if (result == 0) {
        printf("Error saving image to %s.\n", filename);
    } else {
        sizeChar = calcSize(filename);
        printf("Image saved to %s! Size: %s!\n", filename, sizeChar);
    }
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

