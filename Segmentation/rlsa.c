/**
 * \author       victor.simonin
 * \description  All the RLSA algorithm used for the segmentation,
                 also a basic pre-processing function and the tool
                 to debug easily the RLSA.
 */


#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Tools/tools.h"
#include "segmentation.h"


/**
 * \author       victor.simonin
 * \description  Apply the rlsa algorithm horizontally
 * \param    image_surface     The given image
 * \param    step              The step used in the rlsa algorithm
 * \return   Void
 */
//Algorithm of the RLSA -> Horizontal way
void rlsa_horizontal(SDL_Surface* image_surface, int step)
{
  //RLSA horizontal//////////////////////////////////////////
  int x = image_surface->w;
  int y = image_surface->h;

  //Travelling on the sdl
  for(int i = 0; i < y; i++)
    {
      for(int j = 0; j < x; j++)
	{
	  //Getting the pixel and its rgb Values
	  Uint32 pixel = get_pixel(image_surface, j, i);

	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  //Testing if it's black
	  if(r == 0)
	    {
	      int test = 1;
	      int j2 = j + 1;
	      int count = 0;

	      //Testing if they are some white pixels between two black
	      while(test == 1 && count < step && j2 < x)
		{
		  Uint32 pixel2 = get_pixel(image_surface, j2, i);

                  Uint8 r, g, b;
                  SDL_GetRGB(pixel2, image_surface->format, &r, &g, &b);

                  //Stopping if there is a black pixel
                  if(r == 0)
                  {
                      test = 0;
                  }
                  else
                  {
                      count += 1;
                      j2 += 1;
                  }
                }

              //If we have some white pixels between two blacks, change them in black pixels
              if(test == 0)
              {
                  //Travelling from the first to the second black pixel
                  for(int j3 = j + 1; j3 < j2; j3++)
                  {
                      //Getting the pixel, changing its rgb for black and putting him n the sdl
                      Uint32 pixel3 = get_pixel(image_surface, j3, i);

                      Uint8 r, g, b;
                      SDL_GetRGB(pixel3, image_surface->format, &r, &g, &b);

                      r = 0;
                      g = 0;
                      b = 0;
                      Uint32 pixel4 = SDL_MapRGB(image_surface->format, r, g, b);

                      put_pixel(image_surface, j3, i, pixel4);
                  }
              }
              j = j2-1;
            }
        }
    }
}


/**
 * \author       victor.simonin
 * \description  Apply the rlsa vertically
 * \param    image_surface     The given image
 * \param    step              The step used in the rlsa algorithm
 * \return   Void
 */
//Algorithm of the RLSA -> vertical way
void rlsa_vertical(SDL_Surface* image_surface, int step)
{
    int x = image_surface->w;
    int y = image_surface->h;
    //RLSA vertical
    //Travelling on the sdl
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            //Getting the pixel and its rgb Values
            Uint32 pixel = get_pixel(image_surface, i, j);

            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            //Testing if it's black
            if(r == 0)
            {
                int test = 1;
                int j2 = j + 1;
                int count = 0;

                 //Testing if they are some white pixels between two black
                while(test == 1 && count < step && j2 < y)
                {
                    Uint32 pixel2 = get_pixel(image_surface, i, j2);

                    Uint8 r, g, b;
                    SDL_GetRGB(pixel2, image_surface->format, &r, &g, &b);
                    //Stopping if there is a black pixel
                    if(r == 0)
                    {
                        test = 0;
                    }
                    else
                    {
                        count += 1;
                        j2 += 1;
                    }
                }

                //If we have some white pixels between two blacks, change them in black pixels
                if(test == 0)
                {
                    //Travelling from the first to the second black pixel
                    for(int j3 = j + 1; j3 < j2; j3++)
                    {
                        //Getting the pixel, changing its rgb for black and putting him n the sdl
                        Uint32 pixel3 = get_pixel(image_surface, i, j3);

                        Uint8 r, g, b;
                        SDL_GetRGB(pixel3, image_surface->format, &r, &g, &b);

                        r = 0;
                        g = 0;
                        b = 0;

                        Uint32 pixel4 = SDL_MapRGB(image_surface->format, r, g, b);

                        put_pixel(image_surface, i, j3, pixel4);
                    }
                }
                j = j2-1;
            }
        }
    }
}



/**
 * \author       victor.simonin
 * \description  Basic pre-processing function which convert a color image
                 into a black and white one
 * \param    image_surface     The given image
 * \param    value             The value used to convert the pixels in black
                               or white.
 * \return   Void
 */
//Function which suppress the color in an image given
void img_to_black(SDL_Surface* image_surface, int value)
{
    int x = image_surface->w;
    int y = image_surface->h;

    //Travelling on the sdl
    for(int i = 0; i < y; i++)
    {
        for(int j = 0; j < x; j++)
        {
            //Getting the pixel value
            Uint32 pixel = get_pixel(image_surface, j, i);

            //Getting the rgb value of the pixel
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            //Computing the average of the rgb
            double average = r*0.3 + g*0.59 + b*0.11;

            //White pixel if the average is under 128 else black
            if(average > value)
            {
                r = 255;
                g = 255;
                b = 255;
            }
            else
            {
                r = 0;
                g = 0;
                b = 0;
            }

            //New pixel with new rgb value
            Uint32 pixel2 = SDL_MapRGB(image_surface->format, r, g, b);

            //Putting the pixel on the Sdl
            put_pixel(image_surface, j, i, pixel2);

        }
    }
}

/**
 * \author       victor.simonin
 * \description  Apply the logical operator AND on two image in order to
                 keep only the black pixels in both.
 * \param    image_surface     The image after a first horizontal rlsa
 * \param    image_surface2    The image after a first vertical rlsa
 * \param    path              The path of the given image in the computer
 * \return   The final surface after the AND
 NOT USED
 */
SDL_Surface* AndRlsa(SDL_Surface *image_surface, SDL_Surface *image_surface2, char *path)
{
  SDL_Surface* image_surface3;
  SDL_Surface* screen_surface3;

  //init the sdl
  init_sdl();

  //loading and displaying the image
  image_surface3 = load_image(path);
  screen_surface3 = display_image(image_surface3);

  wait_for_keypressed();

  int x = image_surface3->w;
  int y = image_surface3->h;

  //Travelling on the sdl
  for(int i = 0; i < y; i++)
    {
      for(int j = 0; j < x; j++)
        {
	  //Getting the pixel value
	  Uint32 pixel = get_pixel(image_surface3, j, i);
	  //Getting the rgb value of the pixel
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface3->format, &r, &g, &b);


	  //Pixel of the first surface
	  Uint32 pixel2 = get_pixel(image_surface, j, i);
	  Uint8 r2, g2, b2;
	  SDL_GetRGB(pixel2, image_surface->format, &r2, &g2, &b2);

	  //Pixel of the second surface
	  Uint32 pixel3 = get_pixel(image_surface2, j, i);
	  Uint8 r3, g3, b3;
	  SDL_GetRGB(pixel3, image_surface2->format, &r3, &g3, &b3);


	  //AND of the two first surface's pixels
	  if(r2 == 0 && r3 == 0)
            {
	      r = 0;
	      g = 0;
	      b = 0;
            }
	  else
            {
	      r = 255;
	      g = 255;
	      b = 255;
            }

	  //New pixel with new rgb value
	  Uint32 pixel4 = SDL_MapRGB(image_surface3->format, r, g, b);

	  //Putting the pixel on the Sdl
	  put_pixel(image_surface3, j, i, pixel4);

        }
    }



  update_surface(screen_surface3, image_surface3);
  wait_for_keypressed();

  return image_surface3;
}



/**
 * \author       victor.simonin
 * \description  Display the rlsa and generally the segmentation algorithm
 * \param    path              The path of the given image in the computer
 * \return   Void
 */
//Final function for the first presentation
//ONLY FOR DEBUGGING NOW
void display_rlsa(char *path)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    //init the sdl
    init_sdl();

    //loading and displaying the image
    image_surface = load_image(path);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();

//Horizontal RLSA

    img_to_black(image_surface, 235);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    rlsa_horizontal(image_surface, 15);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

//Vertical RLSA

    SDL_Surface* image_surface2;
    SDL_Surface* screen_surface2;

    //init the sdl
    init_sdl();

    //loading and displaying the image
    image_surface2 = load_image(path);
    screen_surface2 = display_image(image_surface2);

    wait_for_keypressed();

    img_to_black(image_surface2, 235);
    update_surface(screen_surface2, image_surface2);
    wait_for_keypressed();

    rlsa_vertical(image_surface2, 40);
    update_surface(screen_surface2, image_surface2);
    wait_for_keypressed();


//AND on the two first surface

    SDL_Surface* image_surface3;
    SDL_Surface* screen_surface3;

    //init the sdl
    init_sdl();

    //loading and displaying the image
    image_surface3 = load_image(path);
    screen_surface3 = display_image(image_surface3);

    wait_for_keypressed();

    int x = image_surface3->w;
    int y = image_surface3->h;

    //Travelling on the sdl
    for(int i = 0; i < y; i++)
    {
        for(int j = 0; j < x; j++)
        {
            //Getting the pixel value
            Uint32 pixel = get_pixel(image_surface3, j, i);
            //Getting the rgb value of the pixel
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface3->format, &r, &g, &b);


            //Pixel of the first surface
            Uint32 pixel2 = get_pixel(image_surface, j, i);
            Uint8 r2, g2, b2;
            SDL_GetRGB(pixel2, image_surface->format, &r2, &g2, &b2);

            //Pixel of the second surface
            Uint32 pixel3 = get_pixel(image_surface2, j, i);
            Uint8 r3, g3, b3;
            SDL_GetRGB(pixel3, image_surface2->format, &r3, &g3, &b3);


            //AND of the two first surface's pixels
            if(r2 == 0 && r3 == 0)
            {
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                r = 255;
                g = 255;
                b = 255;
            }

            //New pixel with new rgb value
            Uint32 pixel4 = SDL_MapRGB(image_surface3->format, r, g, b);

            //Putting the pixel on the Sdl
            put_pixel(image_surface3, j, i, pixel4);

        }
    }



    update_surface(screen_surface3, image_surface3);
    wait_for_keypressed();


    //Smoothing the result with some other rlsa
    rlsa_horizontal(image_surface3, 15);
    rlsa_vertical(image_surface3, 40);

    update_surface(screen_surface3, image_surface3);
    wait_for_keypressed();

    //Writing the red line for the para
    int para[50] = {0};
    Sdl_Horizontal_Segmentation(image_surface3, para);

    for(int i = 0; i < 50; i++)
      {
	printf("%i\n", para[i]);
      }



    update_surface(screen_surface3, image_surface3);
    wait_for_keypressed();


    //FOR LINES

    SDL_Surface* image_surface4;
    SDL_Surface* screen_surface4;
    //init the sdl
    init_sdl();
    //loading and displaying the image
    image_surface4 = load_image(path);
    screen_surface4 = display_image(image_surface4);
    img_to_black(image_surface4, 235);

    wait_for_keypressed();

    rlsa_horizontal(image_surface4, 15);

    update_surface(screen_surface4, image_surface4);
    wait_for_keypressed();

    int lines[200] = {0};
    Sdl_Horizontal_Segmentation(image_surface4, lines);
    update_surface(screen_surface4, image_surface4);
    wait_for_keypressed();

    printf("\n");

    for(int i = 0; i < 200; i++)
      {
	printf("%i\n", lines[i]);
      }

    //FOR words NOW

    SDL_Surface* image_surface6;
    SDL_Surface* screen_surface6;
    //init the sdl
    init_sdl();
    //loading and displaying the image
    image_surface6 = load_image(path);
    screen_surface6 = display_image(image_surface6);
    img_to_black(image_surface6, 235);

    wait_for_keypressed();

    rlsa_horizontal(image_surface6, 5);

    update_surface(screen_surface6, image_surface6);
    wait_for_keypressed();

    int words[4000] = {0};

    int i = 0;
    int j = 0;
    while(lines[i] != 0)
      {
	Sdl_Vertical_Segmentation(image_surface6, words, lines[i], lines[i+1], &j);
	i += 2;
      }


    update_surface(screen_surface6, image_surface6);
    wait_for_keypressed();

    for(int i = 0; i < j; i++)
      {
	printf("%i\n", words[i]);
      }

    printf("\n**********************\n");
    int k = 0;
    x = image_surface6->w;
    while(lines[k] != 0)
      {
	Red_line(image_surface6, x, lines[k] );
	k += 1;
      }

    update_surface(screen_surface6, image_surface6);


    wait_for_keypressed();


    //Search the x,y of the char in the lines

    SDL_Surface* image_surface5;
    SDL_Surface* screen_surface5;
    //init the sdl
    init_sdl();
    //loading and displaying the image
    image_surface5 = load_image(path);
    screen_surface5 = display_image(image_surface5);
    img_to_black(image_surface5, 128);

    wait_for_keypressed();

    int chars[8000] = {0};

    i = 0;
    j = 0;
    while(lines[i] != 0)
      {
	Sdl_Vertical_Segmentation(image_surface5, chars, lines[i], lines[i+1], &j);
	i += 2;
      }

    update_surface(screen_surface5, image_surface5);

    wait_for_keypressed();

    for(int i = 0; i < j; i++)
      {
	printf("%i\n", chars[i]);
      }

    k = 0;
    x = image_surface5->w;
    while(lines[k] != 0)
      {
	Red_line(image_surface5, x, lines[k] );
	k += 1;
      }
    update_surface(screen_surface5, image_surface5);

    ///////////////////////////////////////////////////////////////////

    wait_for_keypressed();

    //Free the image surface.
    SDL_FreeSurface(image_surface);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface);

    //Free the image surface.
    SDL_FreeSurface(image_surface2);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface2);

    //Free the image surface.
    SDL_FreeSurface(image_surface3);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface3);

    //Free the image surface.
    SDL_FreeSurface(image_surface4);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface4);

    //Free the image surface.
    SDL_FreeSurface(image_surface5);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface5);

    //Free the image surface.
    SDL_FreeSurface(image_surface6);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface6);
}

