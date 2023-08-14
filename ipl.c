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
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>
#include "ipl.h"
#include "cli.h"
#include "prompt.h"
#include "resize.h"
#include "crop.h"
#include "rotate.h"

//Dependencies
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main(int argc, char *argv[]) {
    Input input;
    Pics img;
    Dime dime;
    unsigned char *imgData;
    char *fileSize = NULL;
    Crop crop;

    if (argc > 1) {
        // If user call ./ipl with more than 1 argv
        parseArguments(argc, argv, &img, &input, &crop);
    } else {
        // prompt mode
        int option = 0;
        printf("Welcome to simple Image Processing Library for web!\n");
        printf("In few quick steps your image will be ready for fast rendered website!\n");

        printf("Enter path to picture: ");

        // Checking if path is correct
        checkName(&img);
        getPath(&img);

        allocateImg(img.path, &input, &imgData);

        // Calculate image size in KiB
        getSize(img.path, &input, &fileSize);

        // Copy the loaded image data to the allocated memory
        img.data = input.data;
        img.width = input.width;
        img.height = input.height;
        img.channel = input.channels;
        img.bitDepth = input.bitDepth;
        strcpy(img.bitInfo, input.bitInfo);
        img.size = fileSize;

        printInfo(&img);
        
        promptMode(&img, &dime, option, &crop);

        // stbi_image_free(imgData);
        free(fileSize);
        free(imgData);
        free(input.data);
        free(img.data);
        free(img.size);
    }


    return 0;
}

Pics* getPath(Pics *img) {

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
    const char *supportedExtensions[] = {"jpg", "jpeg", "png", "bmp", "tga", "hdr"};
    int isValidExtension = 0;

    char *ext = strrchr(path, '.');
    if (ext) {
        ext++;
        for (int i = 0; i < sizeof(supportedExtensions) / sizeof(supportedExtensions[0]); ++i) {
            if (strcmp(ext, supportedExtensions[i]) == 0) {
                strncpy(extension, supportedExtensions[i], EXT_SIZE);
                break;
            }
        }
    }
    if (!ext) {
        isValidExtension = 1;
    }

    if (isValidExtension != 0) {
        fprintf(stderr, "Unsupported file extension. Supported formats: .jpg, .jpeg, .png, .bmp, .tga, .hdr!\n");
        exit(EXIT_FAILURE);
    }
}

void allocateImg(char *path, Input *input, unsigned char **imgData) {
    *imgData = stbi_load(path, &input->width, &input->height, &input->channels, 0);

    if (*imgData == NULL) {
        fprintf(stderr, "Error loading image: %s from '%s'!\n", stbi_failure_reason(), path);
        free(imgData); // Free the allocated memory before exiting
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

    free(*imgData);
}

void getSize(char *path, Input *input, char **fileSize) {
    // Detect bit per color
    int bpp = stbi_info(path, &input->width, &input->height, &input->bitDepth);

    if (input->channels == 1) {
        input->bitDepth = 1;
    } else if (input->channels == 3) {
        input->bitDepth = 8;
    } else if (input->channels == 4) {
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

    // if (*fileSize != NULL) {
    //     free(*fileSize);
    // }

    *fileSize = calcSize(path);
}

char* calcSize(const char* filename) {
    long fileSize;
    char sizeChar[SIZE];
    char *size = NULL;
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

    size = strdup(sizeChar);
    if (size == NULL) {
        perror("Error alocation failed");
        exit(EXIT_FAILURE);
        free(size);
    }

    return size;
}

void getWidth(Dime *dime) {
    char input[BIT_NAME_SIZE];
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
    char input[BIT_NAME_SIZE];
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
    char input[BIT_NAME_SIZE];
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
    findOutExtension(dime->name, dime->ext);
    // printf("Dime name is %s, ext is %s\n", dime->name, dime->ext);
}

void saveResizedImage(unsigned char* imageData, int width, int height, int channel, const char* filename, char *extension, int quality) {

    int result = 0;
    if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0) {
        saveJPG(imageData, width, height, channel, filename, &result, quality);
    } else if (strcmp(extension, "png") == 0) {
        savePNG(imageData, width, height, channel, filename, &result);
    } else if (strcmp(extension, "bmp") == 0) {
        saveBMP(imageData, width, height, channel, filename, &result);
    } else if (strcmp(extension, "tga") == 0) {
        saveTGA(imageData, width, height, channel, filename, &result);
    } else if (strcmp(extension, "hdr") == 0) {
        saveHDR(imageData, width, height, channel, filename, &result);
    } else {
        printf("Unsupported file extension.\n");
        free(imageData);
        return;
    }

    char *sizeChar;

    if (result == 0) {
        printf("Error saving image to %s.\n", filename);
    } else {
        sizeChar = calcSize(filename);
        printf("Image saved to %s! Size: %s!\n", filename, sizeChar);
        free(sizeChar);
    }
    free(imageData);
}

void saveCroppedImage(unsigned char* imageData, int width, int height, int channel, const char* filename, char *extension, int quality) {

    stbi_write_png(filename, width, height, channel, imageData, width * channel);

    printf("Cropped image saved as %s\n", filename);
    free(imageData);
}

void saveJPG(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result, int quality) {
    // printf("Enter jpg quality [1-100]: ");
    // scanf("%d", &quality);
    if (quality > 0 && quality <= 100)
        *result = stbi_write_jpg(filename, width, height, channel, imageData, quality);
}

void savePNG(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result) {
    *result = stbi_write_png(filename, width, height, channel, imageData, width * channel);
}

void saveBMP(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result) {
    *result = stbi_write_bmp(filename, width, height, channel, imageData);
}

void saveTGA(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result) {
    *result = stbi_write_tga(filename, width, height, channel, imageData);
}

void saveHDR(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result) {
    float *floatImageData = (float *)malloc(width * height * channel * sizeof(float));

    // Convert unsigned char image data to float
    for (int i = 0; i < width * height * channel; ++i) {
        floatImageData[i] = (float)imageData[i] / 255.0f;  // Assuming imageData is in range 0-255
    }

    *result = stbi_write_hdr(filename, width, height, channel, floatImageData);

    free(floatImageData);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

