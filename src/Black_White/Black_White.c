#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void init_sdl2()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image2(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

Uint8* pixel_ref2(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel2(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref2(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void SDL_FreeSurface(SDL_Surface *surface);

void put_pixel2(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref2(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

SDL_Surface * Black_White(char *path, SDL_Surface* image_surface)
{
    init_sdl2();

    image_surface = load_image2(path);

    int w = image_surface->w;

    int h = image_surface->h;

    Uint8 a;

    for (int y=0;y<h;y++)
    {
	    for (int x = 0;x<w;x++)
	    {
		    Uint32 pixel = get_pixel2(image_surface,x,y);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel, image_surface->format, &r, &g ,&b);
		    if (0.3*r+0.59*g+0.11*b>128)
			    a = 255;
		    else
			    a = 0;
		    pixel = SDL_MapRGB(image_surface->format,a,a,a);
		    put_pixel2(image_surface,x,y,pixel);
	    }
    }

    SDL_FreeSurface(image_surface);

    return image_surface;
}
