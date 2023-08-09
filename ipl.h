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
#define BIT_SIZE 64
#define EXT_SIZE 16

typedef struct Picture {
    char path[PATH_SIZE];
    char ext[EXT_SIZE];
    int height;
    int width;
    int channel;
    int bitDepth;
    char bitInfo[BIT_SIZE];
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
    char name[64];
    char ext[EXT_SIZE];
} Dime;

typedef enum {
    RESIZE = 1,
    CROP,
    ROTATE,
    QUIT
} OptionType;

typedef struct {
    OptionType optionType;
    char imagePath[256];
    char background[256];
} Action;

typedef enum {
    BACKGR = 1,
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
    BACK
} Res;

// System functions
Pics* getPath(Pics *img);
void findOutExtension(char *path, char extension[EXT_SIZE]);
void allocateImg(char *path, Input *input, unsigned char **imgData);
void getSize(char *path, Input *input, int channels, char **fileSize);
char* calcSize(const char* result);
// Crop functions
void getName(Dime *dime);
void getWidth(Dime *dime);
void getHeight(Dime *dime);
void crop();
void rotate();
// void performFreeing(Pics *img);
void saveResizedImage(unsigned char* imageData, int width, int height, int channel, const char* filename);
void quit();
void clearInputBuffer();

#endif
