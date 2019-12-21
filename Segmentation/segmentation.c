/**
 * \author       victor.simonin
 * \description  Contain the final algorithm of segmentation,
                 the function which are finding the coordinates of char, words,
                 lines and paragraph and some debugging.
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Tools/tools.h"
#include "../Segmentation/segmentation.h"
#include "rlsa.h"

/**
 * \author       victor.simonin
 * \description  Build the array of the y lines coordinates in our given image
 * \param    array             The binarized array of the given image
 * \param    final_array       The array of coordinates
 * \param    height            The array of : array
 * \param    width             The width of : array
 * \return   Nothing but build the array
 NOT USED
 */
//This fonction will return the array of the Y coordinate of the line -> FIXME
void Line_Segmentation(int array[], int final_array[],  size_t width, size_t height)
{
  // int final_array[70];    //array of the y coordinates of the line
  int count_final = 0;    //int to place the coordinates in the final array
  int in_line = 0;        //bool to know if we are on a char or on
                          //a space between lines

  size_t i = 0;
  size_t j = 0;

  //searching in the binary array where the line are in Y coordinate
  while(i < height)
    {
      j = 0;
      while(j < width)
	{
	  if(array[i*width + j] == 1 && in_line == 0)             //condition if we meet a black pixel -> a line
	    {
	      final_array[count_final] = i-1;
	      count_final += 1;
	      in_line = 1;
	      i += 1;
	      j = 0;
	    }
	  else if(array[i * width + j] == 1 && in_line == 1)        //if we already are on a line and meet a black pixel
	    {
	      j = 0;
	      i += 1;
	    }
	  else if(array[i * width + j] == 0 && (i*width+j) == (((i + 1) * width) - 1) && in_line == 1)   //if we quit a line without finding a black pixel 
	    {
	      final_array[count_final] = i;
	      count_final += 1;
	      in_line= 0;
	      j = 0;
	      i += 1;
	    }
	  else
	    {
	      j += 1;
	    }
	}
      i += 1;
    }

  // return final_array;
}

/**
 * \author       victor.simonin
 * \description  Build the array of the x char coordinates in a line
 * \param    array             The binarized array of the given image
 * \param    final_array       The array of the coordinates
 * \param    ymin, ymax        The y coordinates of the line
 * \param    width             The width of : array
 * \return   Nothing but build the array
 NOT USED
 */
//This fonction will return the array of the x coordinate of the char in a line -> FIXME
void Character_Segmentation(int array[], int final_array[],  size_t ymin, size_t ymax, size_t width)
{
  // int final_array[300];      //array of the x coordinate of the char
  int count_final = 0;       //int to place the coordinate in th array
  int in_char = 0;           //bool to know if we already are on a char

  size_t i = ymin;
  size_t j = 0;

  //searching in the binary array where the char are in x coordinate
  while(j < width)
    {
      i = ymin;
      while(i < ymax)
	{
	  if(array[i * width + j] == 1 && in_char == 0)           //if we meet a black pixel -> a char
	    {
	      final_array[count_final] = j-1;
	      count_final += 1;
	      in_char = 1;
	      j += 1;
	      i = ymin;
	    }
	  else if(array[i * width + j] == 1 && in_char == 1)    //if we already are on a char and meet a black pixel
	    {
	      i = ymin;
	      i += 1;
	    }
	  else if(array[i * width + j] == 0 && i == ymax -1 && in_char == 1)    //the end of a char
	    {
	      final_array[count_final] = j;
	      count_final += 1;
	      in_char = 0;
	      i = ymin;
	      j += 1;
	    }
	  else
	    {
	      i += 1;
	    }
	}
      j += 1;
    }

  // return final_array;
}


/**
 * \author       victor.simonin
 * \description  Build the array of the char with his coordinates
 * \param    array             The binarized array of the given image
 * \param    final_array       The array of the char
 * \param    xmin, xmax        The x coordinates of the char
 * \param    ymin, ymax        The y coordinates of the char
 * \return   Nothing but build the array
 NOT USED
 */
//FIXME
//This function will return the array which represent the char
void Build_Char_Matrix(int array[], int final_array[],  size_t xmin, size_t xmax, size_t ymin, size_t ymax)
{
  size_t height = ymax - ymin;
  size_t width = xmax - xmin;

  size_t a = ymin;
  size_t b = xmin;

  //int final_array[height][width];

  //The algoritm watch in the coordinate that we found before with line and char segmentation
  for(size_t i = 0; i < height; i++)
    {
      for(size_t j = 0; j < width; j++)
	{
	  final_array[i * width + j] = array[a * width + b];
	  b += 1;
	}
      a += 1;
      b -= width;
    }

  // return final_array;
}


/**
 * \author       victor.simonin
 * \description  Printing a horizontal red Line for the debugging
                 It helps to know if our cooordinates are good
 * \param    image_surface     The image of the given text
 * \param    x                 The x coordinate where the line should be
 * \param    y                 The y coordinate supposed for the red line
 * \return   Nothing, just print the line
 */
//Printing an horizontal Red Line for the paragraph/line/char
void Red_line(SDL_Surface* image_surface, int x, int y)
{
  for(int i = 0; i < x; i++)
    {
      //Getting the pixel and its rgb Values
      Uint32 pixel = get_pixel(image_surface, i, y);

      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

      r = 255;
      g = 0;
      b = 0;

      Uint32 pixel2 = SDL_MapRGB(image_surface->format, r, g, b);

      put_pixel(image_surface, i, y, pixel2);
    }
}

/**
 * \author       victor.simonin
 * \description  Printing a vertical red Line for the debugging
                 It helps to know if our cooordinates are good
 * \param    image_surface     The image of the given text
 * \param    x                 The x coordinate where the line should be
 * \param    ymin, ymax        The y coordinates supposed for the red line
 * \return   Nothing, just print the line
 */
//Printing a Vertical Red Line for the paragraph/line/ => here char
void Red_line2(SDL_Surface* image_surface, int x, int ymin, int ymax)
{
  for(int i = ymin; i < ymax; i++)
    {
      //Getting the pixel and its rgb Values
      //Turn it red and put it on the image
      Uint32 pixel = get_pixel(image_surface, x, i);

      Uint8 r, g, b;
      SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

      r = 255;
      g = 0;
      b = 0;

      Uint32 pixel2 = SDL_MapRGB(image_surface->format, r, g, b);

      put_pixel(image_surface, x, i, pixel2);
    }
}

/**
 * \author       victor.simonin
 * \description  Filling an array with the y coordinate of the lines/paragraph
                 This function is called on all of our image
 * \param    image_surface     The image of the given text
 * \param    final_tab         The array of coordinate to fill
 * \return   Nothing but fill the arrays of coordinates
 */
//This function should put the coordinates of our para/lines in an array
void Sdl_Horizontal_Segmentation(SDL_Surface* image_surface, int *final_tab)
{
  //Taking the Width and the Height
  int x = image_surface->w;
  int y = image_surface->h;

  //Bool to know if we are on a para/line/char
  int in_line = 0;
  //count for the array of coordinates
  int count_final = 0;

  //Travelling on the SDL
  for(int i = 0; i < y; i++)
    {
      for(int j = 0; j < x; j++)
	{
	  //Getting the pixel and its rgb Values
	  Uint32 pixel = get_pixel(image_surface, j, i);

	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  //if we have a black pixel and not already on something
	  //Writing the red line and putting the Y coordinate in the array
	  if(r == 0 && in_line == 0)
	    {
	      Red_line(image_surface, x, i-1);
	      in_line = 1;
	      final_tab[count_final] = i-1;
	      count_final += 1;
	      j = 0;
	      i += 1;
	    }
	  else if(r == 0 && in_line == 1) //if we have a black pixel but we already are on a para/line
	    {
	      j = 0;
	      i += 1;
	    }
	  else if(r == 255 && in_line == 1 && j == x-1)//if all the pixels of the lines were white but we were supposed to be on something
	    {
	      Red_line(image_surface, x, i+1);
	      in_line = 0;
	      final_tab[count_final] = i+1;
	      count_final += 1;
	      j = 0;
	      i += 1;
	    }
	}
    }
}

/**
 * \author       victor.simonin
 * \description  Filling an array with the x coordinate of the words/chars
                 This function is called on the lines of our image
 * \param    image_surface     The image of the given text
 * \param    final_tab         The array of coordinate to fill
 * \param    ymin, ymax        The y coordinates of the lines
 * \param    count_final       The index for the final_array
 * \return   Nothing but fill the arrays of coordinates
 */
//Same function than the horizontal one but obviously in the vertical way
void Sdl_Vertical_Segmentation(SDL_Surface* image_surface, int *final_tab, int ymin, int ymax, int *count_final)
{
  int x = image_surface->w;
  // int y = image_surface->h;

  int in_line = 0;

  for(int i = 0; i < x; i++)
    {
      for(int j = ymin; j < ymax; j++)
	{
	  //Getting the pixel and its rgb Values
	  Uint32 pixel = get_pixel(image_surface, i, j);

	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	   if(r == 0 && in_line == 0)
	     {
	       Red_line2(image_surface, i-1, ymin, ymax);
	       in_line = 1;
	       final_tab[*count_final] = i-1;
	       *count_final += 1;
	       j = ymin;
	       i += 1;
	     }
	   else if(r == 0 && in_line == 1)
	     {
	       j = ymin;
	       i += 1;
	     }
	   else if(r == 255 && in_line == 1 && j == ymax-1)
	     {
	       Red_line2(image_surface, i+1, ymin, ymax);
	       in_line = 0;
	       final_tab[*count_final] = i+1;
	       *count_final += 1;
	       j = ymin;
	       i += 1;
	     }
	}
    }
}

/**
 * \author       victor.simonin
 * \description  Filling an array with the x coordinate of the words/chars
                 This function is called on the lines of our image
                 (was supposed to hold the char without spaces)
 * \param    image_surface     The image of the given text
 * \param    final_tab         The array of coordinate to fill
 * \param    ymin, ymax        The y coordinates of the lines
 * \param    count_final       The index for the final_array
 * \return   Nothing but fill the arrays of coordinates
 NOT USED
 */
//Same function than the vertical one but trying to hold the char without spaces
void Sdl_Vertical_Segmentation2(SDL_Surface* image_surface, int *final_tab, int ymin, int ymax, int *count_final)
{
  int x = image_surface->w;
  // int y = image_surface->h;

  int h = ymax - ymin;
  int in_line = 0;
  int countblack;

  for(int i = 0; i < x; i++)
    {
      countblack = 0;
      for(int j = ymin; j < ymax; j++)
	{
	  //Getting the pixel and its rgb Values
	  Uint32 pixel = get_pixel(image_surface, i, j);

	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  if(r == 0 && in_line == 0)
	    {
	       Red_line2(image_surface, i-1, ymin, ymax);
	       in_line = 1;
	       final_tab[*count_final] = i-1;
	       *count_final += 1;
	       j = ymin;
	       i += 1;
	       countblack = 0;
	    }
	  else if(r == 0)
	    countblack += 1;
	}

      if(countblack < (h/10) && in_line == 1)
	{
	  Red_line2(image_surface, i+1, ymin, ymax);
	  in_line = 0;
	  final_tab[*count_final] = i+1;
	  *count_final += 1;
	  i += 1;
	}
    }
}

/**
 * \author       victor.simonin
 * \description  Init the tree struct
 * \param    None
 * \return   The struct tree created
 NOT USED
 */
struct TreeAsBin* Init_Tree()
{
  struct TreeAsBin *tree = malloc(sizeof(struct TreeAsBin));

  if(tree)
    {
      tree->child = NULL;
      tree->sibling = NULL;

      //ALed init key
    }

  return tree;
}


/**
 * \author       victor.simonin
 * \description  Was supposed to build the final text with the tree implementation
 * \param    tree           The tree with the data
 * \param    text           The final text to build
 * \return   Nothing but fill the arrays of char
 NOT USED
 */
void Build_Final_Text(struct TreeAsBin *tree, char *text)
{
  if((tree->child) != NULL)
    {
      while((tree->child)->sibling != NULL)
	{
	  Build_Final_Text(tree->child, text);
	  tree->child = (tree->child)->sibling;
	  strcat(text, tree->key);
	}
      Build_Final_Text(tree->child, text);
    }
  strcat(text, (tree->key));
}


/**
 * \author       victor.simonin
 * \description    Apply the rlsa algorithm and the XY-cut algorithm to get the characters
 *         x and y coordinates.
 * \param    path           The path to the image
 * \param    para           Array-to-fill of para y coordinates
 * \param    lines          Array-to-fill of lines y coordinates
 * \param    words          Array-to-fill of words x coordinates
 * \param    chars          Array-to-fill of chars y coordinates
 * \return   Nothing but fill all the arrays of coordinates
 */
void segmentation(char *path, int *para, int *lines, int *words, int *chars)
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
    ///update_surface(screen_surface, image_surface);
    ///wait_for_keypressed();

    rlsa_horizontal(image_surface, 15);
    ///update_surface(screen_surface, image_surface);
    ///wait_for_keypressed();

//Vertical RLSA

    SDL_Surface* image_surface2;
    ///SDL_Surface* screen_surface2;

    //init the sdl
    init_sdl();

    //loading and displaying the image
    image_surface2 = load_image(path);
    ///screen_surface2 = display_image(image_surface2);

    ///wait_for_keypressed();

    img_to_black(image_surface2, 235);
    ///update_surface(screen_surface2, image_surface2);
    ///wait_for_keypressed();

    rlsa_vertical(image_surface2, 25);
    ///update_surface(screen_surface2, image_surface2);
    ///wait_for_keypressed();


//AND on the two first surface

    SDL_Surface* image_surface3;
    ///SDL_Surface* screen_surface3;

    //init the sdl
    init_sdl();

    //loading and displaying the image
    image_surface3 = load_image(path);
    ///screen_surface3 = display_image(image_surface3);

    ///wait_for_keypressed();

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



    ///update_surface(screen_surface3, image_surface3);
    ///wait_for_keypressed();


    //Smoothing the result with some other rlsa
    rlsa_horizontal(image_surface3, 15);
    rlsa_vertical(image_surface3, 25);

    ///update_surface(screen_surface3, image_surface3);
    ///wait_for_keypressed();

    //Writing the red line for the para
    //int para[50] = {0};
    Sdl_Horizontal_Segmentation(image_surface3, para);

    /*for(int i = 0; i < 50; i++)
      {
	printf("%i\n", para[i]);
	}*/



    ///update_surface(screen_surface3, image_surface3);
    ///wait_for_keypressed();


    //FOR LINES

    SDL_Surface* image_surface4;
    ///SDL_Surface* screen_surface4;
    //init the sdl
    init_sdl();
    //loading and displaying the image
    image_surface4 = load_image(path);
    ///screen_surface4 = display_image(image_surface4);
    img_to_black(image_surface4, 235);

    ///wait_for_keypressed();

    rlsa_horizontal(image_surface4, 15);

    ///update_surface(screen_surface4, image_surface4);
    ///wait_for_keypressed();

    //int lines[200] = {0};
    Sdl_Horizontal_Segmentation(image_surface4, lines);
    ///update_surface(screen_surface4, image_surface4);
    ///wait_for_keypressed();

    /*printf("\n");

    for(int i = 0; i < 200; i++)
      {
	printf("%i\n", lines[i]);
	}*/

    //FOR words
    SDL_Surface* image_surface6;
    ///SDL_Surface* screen_surface6;
    //init the sdl
    init_sdl();
    //loading and displaying the image
    image_surface6 = load_image(path);
    ///screen_surface6 = display_image(image_surface6);
    img_to_black(image_surface6, 235);

    ///wait_for_keypressed();

    rlsa_horizontal(image_surface6, 5);

    ///update_surface(screen_surface6, image_surface6);
    ///wait_for_keypressed();

    //int words[4000] = {0};

    int i = 0;
    int j = 0;
    while(lines[i] != 0)
      {
	Sdl_Vertical_Segmentation(image_surface6, words, lines[i], lines[i+1], &j);
        words[j] = -1;
	j += 1;
	i += 2;
      }


    ///update_surface(screen_surface6, image_surface6);
    ///wait_for_keypressed();

    /*for(int i = 0; i < j; i++)
      {
	printf("%i\n", words[i]);
	}*/

    ///printf("\n*************\n");
    int k = 0;
    x = image_surface6->w;
    while(lines[k] != 0)
      {
	Red_line(image_surface6, x, lines[k] );
	k += 1;
      }

    ///update_surface(screen_surface6, image_surface6);

    ///wait_for_keypressed();

    //Search the x,y of the char in the lines

    SDL_Surface* image_surface5;
    SDL_Surface* screen_surface5;
    //init the sdl
    init_sdl();
    //loading and displaying the image
    image_surface5 = load_image(path);
    screen_surface5 = display_image(image_surface5);
    img_to_black(image_surface5, 128);

    ///wait_for_keypressed();

    //int chars[8000] = {0};

    i = 0;
    j = 0;
    while(lines[i] != 0)
      {
	Sdl_Vertical_Segmentation(image_surface5, chars, lines[i], lines[i+1], &j);
	chars[j] = -1;
	j += 1;
	i += 2;
      }

    update_surface(screen_surface5, image_surface5);

    ///wait_for_keypressed();

    /*for(int i = 0; i < j; i++)
      {
	printf("%i\n", chars[i]);
	}*/

    k = 0;
    x = image_surface5->w;
    while(lines[k] != 0)
      {
	Red_line(image_surface5, x, lines[k] );
	k += 1;
      }
    update_surface(screen_surface5, image_surface5);

    ////////////////////////////////////////////////////////////////////////////

    wait_for_keypressed();

    //Free the image surface.
    SDL_FreeSurface(image_surface);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface);

    //Free the image surface.
    SDL_FreeSurface(image_surface2);

    //Free the screen surface.
    //SDL_FreeSurface(screen_surface2);

    //Free the image surface.
    SDL_FreeSurface(image_surface3);

    //Free the screen surface.
    //SDL_FreeSurface(screen_surface3);

    //Free the image surface.
    SDL_FreeSurface(image_surface4);

    //Free the screen surface.
    //SDL_FreeSurface(screen_surface4);

    //Free the image surface.
    SDL_FreeSurface(image_surface5);

    //Free the screen surface.
    SDL_FreeSurface(screen_surface5);

    //Free the image surface.
    SDL_FreeSurface(image_surface6);

    //Free the screen surface.
    //SDL_FreeSurface(screen_surface6);
}
