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
#include "cli.h"

void parseArguments(int argc, char *argv[], Pics img, Input input) {
    printf("You just entered the cli mode!");
    if (argc == 5 || argc == 9) {
        // strncpy(img.path, argv[1], sizeof(img.path));
        // img.path[sizeof(img.path) - 1] = '\0';
        // imgPath = stbi_load(img.path, &width, &height, &channels, 0);
        printf("Resize\n");

        // if (imgPath == NULL) {
        //     // fprintf(stderr, "Error loading image: %s from '%s'!\n", stbi_failure_reason(), img.path);
        //     // free(img.data); // Free the allocated memory before exiting
        //     // exit(EXIT_FAILURE);
        //     printf("Null\n");
        // }

        // printf("Image path: %s", img.path);

    }

}
Action performAction(const Action *action) {
    printf("Hello there!");
    Action reaction;
    return reaction;
}
