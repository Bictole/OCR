#ifndef RLSA_H
#define RLSA_H

#include <stdlib.h>
#include <SDL.h>

void sdl_to_black(char *path);

void rlsa_horizontal(SDL_Surface* image_surface, int step);

void rlsa_vertical(SDL_Surface* image_surface, int step);

void img_to_black(SDL_Surface* image_surface, int value);

SDL_Surface* AndRlsa(SDL_Surface *image_surface, SDL_Surface *image_surface2, char *path);

void display_rlsa(char *path);

#endif
