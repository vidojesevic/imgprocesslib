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


typedef struct Picture {
    char path[128];
    int height;
    int width;
    int channel;
    int bitDepth;
    char bitInfo[64];
    char *size;
    unsigned char* data;
} Pics;

typedef struct NewDimension {
    int resWidth;
    int resHeight;
    char name[64];
} Dime;

typedef enum {
    RESIZE = 1,
    CROP,
    ROTATE,
    QUIT
} OptionType;

typedef struct {
    OptionType optionType;
    char imegePath[256];
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

// Universal functions
char* calcSize(const char* result);
// Resize functions
// Crop functions
void crop();
void rotate();
void performFreeing(Pics *img);
void saveImage(unsigned char* imageData, int width, int height, int channel, const char* filename);
void quit();
void clearInputBuffer();

#endif
