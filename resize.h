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
#ifndef RESIZE_H
#define RESIZE_H

//dependencies
#include "ipl.h"
#include "prompt.h"

// Functions
void resize(Pics *img, Dime *dime);
Dime resizeBack(Dime *dime);
Dime resizeHero(Dime *dime);
Dime resizeBanner(Dime *dime);
Dime resizeBlog(Dime *dime);
Dime resizeLogoRec(Dime *dime);
Dime resizeLogoSc(Dime *dime);
Dime resizeFavicon(Dime *dime);
Dime resizeSocial(Dime *dime);
Dime resizeLight(Dime *dime);
Dime resizeThumb(Dime *dime);
Dime resizeCustom(Dime *dime);
unsigned char* performResize(unsigned char* imageData, int width, int height, int channel, int newWidth, int newHeight);

#endif

