#ifndef LOAD_IMAGE_H
#define LOAD_IMAGE_H

#include <stdlib.h>
#include <SDL/SDL.h>

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
void SDL_FreeSurface(SDL_Surface *surface);
void SDL_FreeSurface(SDL_Surface *surface);
void image(char *chemin);
void otsu(SDL_Surface *image, int width, int height, int histo[]);
float *fornetworktrain(char *chemin);
	
#endif
