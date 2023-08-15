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
#ifndef ROTATE_H
#define ROTATE_H

//dependencies
#include "ipl.h"
#include "prompt.h"

// Functions
unsigned char interpolateColor(Pics *img, float x, float y, int channel, char direction[BIT_NAME_SIZE]);
void rotationAlgoritm(unsigned char *imgData, Pics *img, Flip *flip, int newWidth, int newHeight);
void rotate(Pics *img, Flip *flip);

#endif


