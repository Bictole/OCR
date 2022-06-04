/**
 * \author       victor.simonin
 * \description  Contain Functions that mostly are useful to manipulate sdl 1.2
                 95% of these func are taken from Practical 3 : ARRAY AND SDL
 */


#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include <stdio.h>


//Managing the sdl

/**
 * \author       victor.simonin
 * \description  Init the sdl
 * \return   Void
 */
void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


/**
 * \author       victor.simonin
 * \description  Load the sdl image
 * \param    path        The path of the given image in the computer
 * \return   The sdl_surface which is load
 */
SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


/**
 * \author       victor.simonin
 * \description  Display the given sdl_surface on the screen
 * \param    img       The sdl_surface load from the previouos function
 * \return   The sdl_surface which is displayed
 */
SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


/**
 * \author       victor.simonin
 * \description  Stop the process until a key is pressed down
 * \return       void
 */
void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}


/**
 * \author       victor.simonin
 * \description  Free the memory allocated for the surface
 * \param        surface        The sdl_surface which need to be free
 * \return       void
 */
void SDL_FreeSurface(SDL_Surface *surface);



static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

/**
 * \author       victor.simonin
 * \description  Getting the value of a pixel with his coordinates
 * \param        surface        The sdl_surface
 * \param        x, y           The coordinates of the pixel on the image
 * \return       The sdl_surface which is load
 */
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

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

/**
 * \author       victor.simonin
 * \description  Setting the value of a pixel with his coordinates
 * \param        surface        The sdl_surface
 * \param        x, y           The coordinates of the pixel on the image
 * \param        pixel          The pixel
 * \return       void
 */
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

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


/**
 * \author       victor.simonin
 * \description  Redraw the surface
 * \param        image        The sdl_surface which is the given image
 * \param        screen       The sdl_surface which is the screen
 * \return       void
 */
void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}




/**
 * \author       victor.simonin
 * \description  Complete function to display an image on the screen
 * \return       The sdl_surface which is load
 */
void diplay_the_image()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // TODO: Initialize the SDL
    init_sdl();

    image_surface = load_image("image.jpeg");
    screen_surface =  display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    wait_for_keypressed();


    // TODO: Free the image surface.
    SDL_FreeSurface(image_surface);


    // TODO: Free the screen surface.
    SDL_FreeSurface(screen_surface);
}

/**
 * \author       victor.simonin
 * \description  Complete function to convert an image into grayscale
 * \return       void
 */
void grayscale_the_image()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    // TODO: Initialize the SDL
    init_sdl();

    image_surface = load_image("image.jpeg");
    screen_surface = display_image(image_surface);

    // TODO: Wait for a key to be pressed.
    wait_for_keypressed();

    int x = image_surface->w;
    int y = image_surface->h;

    for(int i = 0; i < y; i++)
      {
	for(int j = 0; j < x; j++)
	  {
	    Uint32 pixel = get_pixel(image_surface, j, i);

	    Uint8 r, g, b;
	    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	    double average = r*0.3 + g*0.59 + b*0.11;

	    r = average;
	    g = average;
	    b = average;

	    Uint32 pixel2 = SDL_MapRGB(image_surface->format, r, g, b);

	    put_pixel(image_surface, j, i, pixel2);

	  }
      }

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();


    // TODO: Free the image surface.
    SDL_FreeSurface(image_surface);

    // TODO: Free the screen surface.
    SDL_FreeSurface(screen_surface);
}


//Managing the matrices

/**
 * \author       victor.simonin
 * \description  Complete function to print an array
 * \param        m           The array of int
 * \param        rowx,cols   The width and height of the array
 * \return       void
 */
void print_matrix(int m[], size_t rows, size_t cols)
{
    for(size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            int a = m[i*cols + j];
            printf("%4d", a);
        }
        printf("\n");
    }
}









