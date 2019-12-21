#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <SDL.h>

//Managing the sdl :

void init_sdl();

SDL_Surface* load_image(char *path);

SDL_Surface* display_image(SDL_Surface *img);

void wait_for_keypressed();

void SDL_FreeSurface(SDL_Surface *surface);

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void update_surface(SDL_Surface* screen, SDL_Surface* image);

void diplay_the_image();

void grayscale_the_image();


//Managing the matrices
void print_matrix(int m[], size_t rows, size_t cols);

#endif


