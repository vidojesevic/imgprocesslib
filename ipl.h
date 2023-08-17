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
#ifndef IPL_H
#define IPL_H

#define KiloByte 1024
#define PATH_SIZE 128
#define BIT_NAME_SIZE 64
#define SIZE 32
#define EXT_SIZE 16

typedef struct Picture {
    char path[PATH_SIZE];
    char name[BIT_NAME_SIZE];
    char ext[EXT_SIZE];
    int height;
    int width;
    int channel;
    int bitDepth;
    int quality;
    char bitInfo[BIT_NAME_SIZE];
    char *size;
    unsigned char* data;
} Pics;

typedef struct Input {
    int width;
    int height;
    int channels;
    int bitDepth;
    char bitInfo[64];
    unsigned char* data;
} Input;

typedef struct NewDimension {
    int resWidth;
    int resHeight;
    int quality;
    char name[64];
    char ext[EXT_SIZE];
} Dime;

typedef struct Crop { 
    int x;
    int y;
    int border;
} Crop;

typedef struct Flip {
    char direction[BIT_NAME_SIZE];
    float cosTheta;
    float sinTheta;
} Flip;

typedef enum {
    RESIZE = 1,
    CROP,
    ROTATE,
    QUIT
} Options;

typedef enum {
    BACK = 1,
    HERO,
    BANNER,
    BLOG,
    LOGOREC,
    LOGOSC,
    FAVICON,
    SOCIAL,
    LIGHTBOX,
    THUMBNAIL,
    CUSTOM,
    GOBACK
} Res;

typedef struct {
    Res resValue;
    const char *resOption;
} ResOptionMapping;

// function pointer type
typedef Dime (*ResizeFunction)(Dime *dime);

typedef struct {
    const char *resOption;
    Dime (*resizeFunction)(Dime *dime);
} ResOptionFunction;

// System functions
Pics* getPath(Pics *img);
void findOutExtension(char *path, char extension[EXT_SIZE]);
void allocateImg(char *path, Input *input, unsigned char **imgData);
void getSize(char *path, Input *input, char **fileSize);
char* calcSize(const char* result);
// Crop functions
void getName(Dime *dime);
void getWidth(Dime *dime);
void getHeight(Dime *dime);
void rotate();
// void performFreeing(Pics *img);
void saveJPG(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result, int quality);
void savePNG(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result);
void saveBMP(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result);
void saveTGA(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result);
void saveHDR(unsigned char* imageData, int width, int height, int channel, const char* filename, int *result);
void saveImage(unsigned char* imageData, int width, int height, int channel, const char* filename, char *extension, int quality);
void saveCroppedImage(unsigned char* imageData, int width, int height, int channel, const char* filename, char *extension, int quality);
void clearInputBuffer();

#endif
