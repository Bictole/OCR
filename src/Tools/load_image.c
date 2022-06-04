#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "segment.h"


void init_sdl()
{
        // Init only the video part.
	// If it fails, die with an error message.
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


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


SDL_Surface* display_image(SDL_Surface *img)
{
	SDL_Surface *screen;

	// Set the window to the same size as the image

	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if(screen == NULL)
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


void SDL_FreeSurface(SDL_Surface *surface);
void otsu(SDL_Surface *image,int width,int height,int histo[]);

static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
	int bpp = surf->format->BytesPerPixel;
	return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}


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

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
	if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());

	SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}


float probar(int histo[],int index,int nbpix);
float probaclasse1(int histo[],int T, int nbpix);
float probaclasse2(int histo[],int T, int nbpix);
float moyenne1(int histo[], int T, int nbpix);
float moyenne2(int histo[], int T, int nbpix);
float variance1(int histo[],int T,int nbpix);
float variance2(int histo[],int T,int nbpix);
float variance_intra(int histo[],int T,int nbpix);
int best_variance(int histo[],int nbpix);
void print_var(int histo[], int nbpix);


void image(char *chemin)
{
        SDL_Surface* image_surface;	
        
        /* Affichage */
	SDL_Surface* screen_surface;
        
    	init_sdl();
	image_surface = load_image(chemin);
        
        /* montre avant nuance de gris*/
        screen_surface = display_image(image_surface);
        wait_for_keypressed();
        
	int width = image_surface->w;
	int height = image_surface->h;	
	Uint8 average;
	int histogramme[256];
	for(int i = 0; i < 256;i++)
	{
		histogramme[i] = 0;
	}
	for (int y = 0;y < height;y++)
	{
		for(int x = 0; x < width; x++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = (r+g+b)/3;
			histogramme[average]++;
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface, x, y, pixel);
		}
	}
        /* pour montrer after nuance gris*/
        screen_surface = display_image(image_surface);
        wait_for_keypressed();
        
	otsu(image_surface, width, height, histogramme);
        
        /* montre apres traitement otsu */
	screen_surface = display_image(image_surface);
	wait_for_keypressed();
		
	segmentation(image_surface);
	
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
}

float *fornetworktrain(char *chemin)
{
        SDL_Surface* image_surface;	
    	init_sdl();
	image_surface = load_image(chemin);
        
	int width = image_surface->w;
	int height = image_surface->h;	
	Uint8 average;
	int histogramme[256];
	for(int i = 0; i < 256;i++)
	{
		histogramme[i] = 0;
	}
	for (int y = 0;y < height;y++)
	{
		for(int x = 0; x < width; x++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = (r+g+b)/3;
			histogramme[average]++;
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface, x, y, pixel);
		}
	}      
	otsu(image_surface, width, height, histogramme);  
		
	return matrice(image_surface,0,width,0,height);
	
	SDL_FreeSurface(image_surface);
}


void otsu(SDL_Surface *image, int width, int height, int histo[])
{
	//print_var(histo,(width*height)); #permet de voir les variance
	float seuil = best_variance(histo,(width*height));
	//printf("%f \n",seuil); #affiche la valeur de seuil choisi
	Uint8 BoW;
	int noir = 0;
	int blanc = 0;
	for(int y = 0;y < height;y++)
	{
		for(int x = 0; x < width; x++)
		{
			Uint32 pixel = get_pixel(image,x,y);
			Uint8 r,g,b;
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
			if(r < seuil)
			{
				BoW = 0;
				blanc++;
			}
			else
			{
				BoW = 255;
				noir++;
			}
			pixel = SDL_MapRGB(image->format,BoW,BoW,BoW);
			put_pixel(image,x,y,pixel);
		}
	}
        if(noir < blanc)
	{
		for(int y = 0;y <height;y++)
		{
			for(int x = 0;x < width; x++)
			{
				Uint32 pixel = get_pixel(image,x,y);
				Uint8 r,g,b;
				SDL_GetRGB(pixel,image->format,&r,&g,&b);
				if(r == 255)
				{
					BoW = 0;
				}
				else
				{
					BoW = 255;
				}
				pixel = SDL_MapRGB(image->format,BoW,BoW,BoW);
				put_pixel(image,x,y,pixel);
			}
		}
	}

}

float probar(int histo[],int index,int nbpix)
{
	float nb = (float)histo[index] / (float)nbpix;
	return nb;
}

float probaclasse1(int histo[],int T, int nbpix)
{
	float prob = 0;
	for(int r = 0;r <= T;r++)
	{
		prob = prob + probar(histo,r,nbpix);
	}
	return prob;

}

float probaclasse2(int histo[], int T, int nbpix)
{
	float prob = 0;
	for(int r = T+1;r <= 255;r++)
	{
		prob = prob + probar(histo,r,nbpix);
	}
	return prob;
}

float moyenne1(int histo[], int T, int nbpix)
{
	float moy = 0;
	for(int r = 0;r<= T;r++)
	{
		moy = moy + (r*probar(histo,r,nbpix));
	}
	return moy/probaclasse1(histo,T,nbpix);
}

float moyenne2(int histo[], int T, int nbpix)
{
	float moy = 0;
	for(int r = T+1;r <= 255;r++)
	{
		moy = moy + (r*probar(histo,r,nbpix));
	}
	return moy/probaclasse2(histo,T,nbpix);
}

float variance1(int histo[],int T,int nbpix)
{
	float vari = 0;
	float moy1 = moyenne1(histo,T,nbpix);
	for(int r =0;r <= T;r++)
	{
		vari = vari + (r - moy1)*(r - moy1)*(probar(histo,r,nbpix));
	}
	return vari/probaclasse1(histo,T,nbpix);
}

float variance2(int histo[],int T,int nbpix)
{
	float vari = 0;
	float moy2 = moyenne2(histo,T,nbpix);
	for(int r = T+1;r <= 255;r++)
	{
		vari = vari + (r - moy2)*(r - moy2)*(probar(histo,r,nbpix)); 
	}
	return vari/probaclasse2(histo,T,nbpix);
}

float variance_intra(int histo[],int T,int nbpix)
{
	return (probaclasse1(histo,T,nbpix)*variance1(histo,T,nbpix))+(probaclasse2(histo,T,nbpix)*variance2(histo,T,nbpix));
}

int best_variance(int histo[],int nbpix)
{
	float intra;
	int T=1;
	while(variance_intra(histo,T,nbpix) != variance_intra(histo,T,nbpix))
	{
		T++;
	}
	float bestvar = variance_intra(histo,T,nbpix);
	int valeur = T;
	for(; T< 255;T++)
	{
		intra = variance_intra(histo,T,nbpix);
		if(intra < bestvar)
		{
			valeur = T;
			bestvar = intra;
		}
	}
	return valeur;
}

void print_var(int histo[],int nbpix)
{
	for(int T = 1;T<255;T++)
	{
		printf("%d : %f \n",T,variance_intra(histo,T,nbpix));
	}
}
