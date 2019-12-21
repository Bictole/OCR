#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

int work();

SDL_Keycode pause();

void Black_White(SDL_Surface* image,int w, int h, int matrice[w][h]);

Uint32 getpixel(SDL_Surface *surface, int x, int y);

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
 
int main()
{
	SDL_Keycode s[2];
	s[0]= SDLK_SPACE;
	s[1] = SDLK_ESCAPE;
	/*SDL_Keycode srl[4];
	srl[0]=SDLK_SPACE;
	srl[1]=SDLK_RIGHT;
	srl[2]=SDLK_LEFT;
	srl[3] = SDLK_ESCAPE;*/
	SDL_Keycode ot[3];
	ot[0] = SDLK_KP_1;
	ot[1] = SDLK_KP_2;
	ot[2] = SDLK_ESCAPE;
	SDL_Keycode ottf[5];
	ottf[0] = SDLK_KP_1;
	ottf[1] = SDLK_KP_2;
	ottf[2] = SDLK_KP_3;
	ottf[3] = SDLK_KP_4;
	ottf[4] = SDLK_ESCAPE;
	SDL_Keycode e[1];
	e[0] = SDLK_ESCAPE;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);

	SDL_Window* mainWindow = SDL_CreateWindow("OCR",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1000,1000,SDL_WINDOW_SHOWN);
	SDL_Surface* background1 = IMG_Load("Interface/Intro.jpg");
	SDL_Renderer* renderer1=SDL_CreateRenderer(mainWindow,-1,SDL_RENDERER_ACCELERATED);
	SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
	SDL_RenderCopy(renderer1,texture1,NULL,NULL);

	SDL_RenderPresent(renderer1);

	if (pause(s,2)==SDLK_ESCAPE)
	{	
		SDL_DestroyTexture(texture1);
		SDL_FreeSurface(background1);

		SDL_DestroyRenderer(renderer1);

		SDL_DestroyWindow(mainWindow);

		IMG_Quit();

		SDL_Quit();

		return 0;
	}
	

	SDL_DestroyTexture(texture1);
	SDL_FreeSurface(background1);

	background1 = IMG_Load("Interface/format.png");
	texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
	SDL_RenderCopy(renderer1,texture1,NULL,NULL);
	SDL_RenderPresent(renderer1);
	int format = 0;

	switch(pause(ottf,5))
	{
		case(SDLK_KP_1):
			format = 1;
			break;
		case(SDLK_KP_2):
			format = 2;
			break;
		case(SDLK_KP_3):
			format = 3;
			break;
		case(SDLK_KP_4):
			format = 4;
			break;
		case(SDLK_ESCAPE):
			SDL_DestroyTexture(texture1);
			SDL_FreeSurface(background1);

			SDL_DestroyRenderer(renderer1);
	
			SDL_DestroyWindow(mainWindow);

			IMG_Quit();

			SDL_Quit();

			return 0;
			break;
	}

	SDL_DestroyTexture(texture1);
	SDL_FreeSurface(background1);

	int imageok = 1;
	int angle;
	//int rotated;
	//int i;
	int a;
	int b;
	int c;
	int d;
	SDL_Texture* texture3;
	SDL_Rect dest;
	SDL_Surface* image = 0;


	while (imageok)
	{
		imageok = 0;
		background1 = IMG_Load("Interface/instruction.png");
		texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
		SDL_RenderCopy(renderer1,texture1,NULL,NULL);
		SDL_RenderPresent(renderer1);
		if (pause(s,2)==SDLK_ESCAPE)
		{	
			SDL_DestroyTexture(texture1);
			SDL_FreeSurface(background1);

			SDL_DestroyRenderer(renderer1);

			SDL_DestroyWindow(mainWindow);

			IMG_Quit();

			SDL_Quit();

			return 0;
		}
		SDL_DestroyTexture(texture1);
		SDL_FreeSurface(background1);

		switch(format)
		{
			case(1):
				image = IMG_Load("Image/OCR.jpg");
				break;
			case(2):
				image = IMG_Load("Image/OCR.png");
				break;
			case(3):
				image = IMG_Load("Image/OCR.bmp");
				break;
			case(4):
				image = IMG_Load("Image/OCR.tif");
				break;
		}
		if (image)
		{
			background1 = IMG_Load("Interface/verif.png");
			texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
			SDL_RenderCopy(renderer1,texture1,NULL,NULL);
			SDL_RenderPresent(renderer1);

			a = image->h;
			b = image->w;

			if (a > 600 || b > 600)
			{
				if (a > b)
				{
					b = (b*600)/a;
					a = 600;
				}
				else
				{
					a = (a*600)/b;
					b = 600;
				}
			}
			d = 600/2-b/2+200;
			c=600/2-a/2+200;
			texture3 = SDL_CreateTextureFromSurface(renderer1,image);
			dest.x = d;
			dest.y = c;
			dest.w = b;
			dest.h = a;
			//rotated = 0;
			angle = 0;
			//i = 0;
			SDL_RenderCopyEx(renderer1,texture3,NULL,&dest,angle,NULL,SDL_FLIP_NONE );
			SDL_RenderPresent(renderer1);
			switch (pause(ot,3))
			{
				case(SDLK_KP_2):
					imageok = 1;
					break;
				case(SDLK_ESCAPE):
					SDL_DestroyTexture(texture1);
					SDL_FreeSurface(background1);

					SDL_DestroyRenderer(renderer1);
	
					SDL_DestroyWindow(mainWindow);

					IMG_Quit();

					SDL_Quit();

					return 0;
					break;
			}
			
			SDL_DestroyTexture(texture1);
			SDL_FreeSurface(background1);
		}
		else
		{
			background1 = IMG_Load("Interface/fail.png");
			texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
			SDL_RenderCopy(renderer1,texture1,NULL,NULL);
			SDL_RenderPresent(renderer1);
			if (pause(s,2)==SDLK_ESCAPE)
			{	
				SDL_DestroyTexture(texture1);
				SDL_FreeSurface(background1);

				SDL_DestroyRenderer(renderer1);

				SDL_DestroyWindow(mainWindow);

				IMG_Quit();

				SDL_Quit();

				return 0;
			}
			SDL_DestroyTexture(texture1);
			SDL_FreeSurface(background1);
			imageok = 1;
		}
	}

	/*background1 = IMG_Load("Interface/rotation.png");
	texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
	SDL_RenderCopy(renderer1,texture1,NULL,NULL);

	SDL_RenderPresent(renderer1);

	i=0;
	

	while (!rotated)
	{
		SDL_RenderPresent(renderer1);
		if (i>2)
		{
			switch(pause(srl,4))
			{
				case(SDLK_SPACE):
					rotated = 1;
					break;
				case(SDLK_LEFT):
					angle-=1;
					break;
				case(SDLK_RIGHT):
					angle+=1;
					break;
				case(SDLK_ESCAPE):
					SDL_DestroyTexture(texture1);
					SDL_FreeSurface(background1);

					SDL_DestroyRenderer(renderer1);
	
					SDL_DestroyWindow(mainWindow);

					IMG_Quit();

					SDL_Quit();

					return 0;
					break;
			}
		}
		i+=1;
		SDL_RenderCopy(renderer1,texture1,NULL,NULL);
		SDL_RenderPresent(renderer1);
		if (i<3)
		{
			SDL_Delay(5);
		}
		SDL_RenderCopyEx(renderer1,texture3,NULL,&dest,angle,NULL,SDL_FLIP_NONE );
	}

	SDL_DestroyTexture(texture3);*/

	background1 = IMG_Load("Interface/wait.png");
	texture1 = SDL_CreateTextureFromSurface(renderer1,background1);
	SDL_RenderCopy(renderer1,texture1,NULL,NULL);
	SDL_RenderPresent(renderer1);

	work(image);

	SDL_Delay(5000);

	SDL_FreeSurface(image);

	SDL_DestroyTexture(texture1);

	SDL_FreeSurface(background1);

	background1 = IMG_Load("Interface/result.png");

	texture1 = SDL_CreateTextureFromSurface(renderer1,background1);

	SDL_RenderCopy(renderer1,texture1,NULL,NULL);

	SDL_RenderPresent(renderer1);

	pause(e,1);

	SDL_DestroyTexture(texture1);

	SDL_FreeSurface(background1);

	SDL_DestroyRenderer(renderer1);

	SDL_DestroyWindow(mainWindow);

	IMG_Quit();

	SDL_Quit();

	return 0;
}

int work(SDL_Surface* image)
{
	int w = image->w;
	int h = image->h;
	int matrice[w][h];
	Black_White(image,w,h,matrice);
	/*for (int j = 0;j<h;j++)
	{	
		for (int i = 0;i<w;i++)
		{
			printf("%i\n",matrice[i][j]);
		}
	}*/
	char c[]="I LOVE EPITA";

	FILE * fPtr;
	fPtr = fopen("Result/OCR_Result.txt","w");

	fputs(c,fPtr);

	fclose(fPtr);

	return 0;
}

SDL_Keycode pause(SDL_Keycode *keylist, int len)
{
	int continuer = 1;
	SDL_Event event;
	SDL_Keycode a;
	event.type = 0;
	while (continuer)
	{
		SDL_WaitEvent(&event);
		if (event.type==SDL_KEYDOWN)
		{
			for (int i = 0;i<len;i++)
			{
				if (keylist[i]==event.key.keysym.sym)
				{
					continuer = 0;
					a=keylist[i];
				}
			}
		}
	}
	return a;
}

void Black_White(SDL_Surface* image,int w, int h, int matrice[w][h])
{
    	int m = 0;

	int n = 0;

	Uint8 r;
	Uint8 g;
	Uint8 b;

	SDL_LockSurface(image);

    	for (int y=0;y<h;y++)
    	{
		for (int x = 0;x<w;x++)
	    	{
			SDL_GetRGB(getpixel(image,x,y),image->format,&r,&g,&b);
			m+=0.3*r+0.59*g+0.11*b;
			n+=1;
	    	}
    	}

	Uint8 cmoyenne = m/n;

    	for (int y=0;y<h;y++)
    	{
		for (int x = 0;x<w;x++)
	    	{
			
				SDL_GetRGB(getpixel(image,x,y),image->format,&r,&g,&b);
				if (0.3*r+0.59*g+0.11*b>cmoyenne)
				{
					matrice[x][y] = 1;
				}
				else
				{
					matrice[x][y] = 0;
				}
			
	    	}
    	}

	SDL_UnlockSurface(image);
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
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
