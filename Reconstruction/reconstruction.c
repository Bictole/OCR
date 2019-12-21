/**
 * \author       victor.simonin
 * \description  Contain the functions used for the reconstruction of the text
 */


#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Tools/tools.h"
#include "../Segmentation/segmentation.h"
//#include "../Neural_Network/Matrix.h"

/**
 * \author       victor.simonin
 * \description  Compute the length of a given array
 * \param    array             An array of int
 * \return   The length
 NOT USED
 */
int Length(int* array)
{
  int i = 0;
  while(array[i] != 0)
    {
      i += 1;
    }

  return i;
}

/**
 * \author       victor.simonin
 * \description  Build the Matrix of a char in the given image
 * \param    ymin,ymax         The y coordinates of the char in the image
 * \param    xmin,xmax         The x coordinates of the char in the image
 * \param    image_surface     The given image
 * \return   The Matrix
 NOT USED
 */
/*matrix* CharMatrix(int ymin, int ymax, int xmin, int xmax, SDL_Surface image_surface)
{
  matrix c = build(ymax-ymin, xmax-xmin);

  for(int i = ymin; i < ymax; i++)
    {
      for(int j = xmin; j < xmax; j++)
	{
	  //Getting the pixel and its rgb Values
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  if(r == 255)
	    {
	      //aled
	    }
	  else
	    {
	      //aled
	    }
	}
    }

  return c;
  }*/


/**
 * \author       victor.simonin
 * \description  Build the final text
 * \param    final                   The final array of char
 * \param    path                    The path of the image in the computer
 * \param    para,lines,words,chars  The array of coordinates
 * \return   Nothing but build the array
 */
void Build_final_char(char *final, int *para, int *lines, int *words, int *chars/*, char *path*/)
{
  int p = 0;
  int l = 0;
  int w = 0;
  int c = 0;
  int f = 0;

  while(para[p] != 0)
    {
      while(chars[c+1] <= words[w+1] && chars[c] != -1 && chars[c] != 0)
	{
	  //BuildMatrix(lines[l], lines[l+1], chars[c], chars[c+1], image_surface3
	  //envoyer ça au réseau et en ressortir un char

	  final[f] = '0';//le char qu'on aura sorti
	  f += 1;
	  c += 2;
	}

      if(words[w+2] == -1)
	{
	  l += 2;
	  w += 3;
	  c += 1;
	  final[f] = '\n';
	  f += 1;

	  if(lines[l+1] > para[p+1])
	    {
	      p += 2;
	      final[f] = '\n';
	      f += 1;
	    }
	}
      else if(chars[c] == 0)
	{
	  p += 2;
	}
      else
	{
	  w += 2;
	  final[f] = ' ';
	  f += 1;
	}
    }

  //return final;

}



/**
 * \author       victor.simonin
 * \description  Write and Save the final text in a document.txt
 * \param    final        The final array of char
 * \return   Void
 */
void Reconstruction(char *final)
{
  FILE* fichier = NULL;

  //char aled[] = "ALED";

  fichier = fopen("YourText.txt", "w+");

  if (fichier != NULL)
    {
      fputs(final, fichier);
      fclose(fichier);
    }
  else
    {
      errx(66, "Problem with opening the file .txt");
    }
}

