#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stdlib.h>
#include <SDL.h>

struct TreeAsBin
{
  //First Child
  struct TreeAsBin   *child;

  //Right Sibling
  struct TreeAsBin   *sibling;

  char                key[3];
  
};


void Line_Segmentation(int array[], int final_array[],  size_t width, size_t height);

void Character_Segmentation(int array[], int final_array[],  size_t ymin, size_t ymax, size_t width);

void Build_Char_Matrix(int array[], int final_array[],  size_t xmin, size_t xmax, size_t ymin, size_t ymax);

void Sdl_Horizontal_Segmentation(SDL_Surface* image_surface, int *final_tab);

void Sdl_Vertical_Segmentation(SDL_Surface* image_surface, int *final_tab, int ymin, int ymax, int *count_final);

void Red_line(SDL_Surface* image_surface, int x, int y);

void Red_line2(SDL_Surface* image_surface, int x, int ymin, int ymax);

struct TreeAsBin* Init_Tree();

void Build_Final_Text(struct TreeAsBin *tree, char *text);

void segmentation(char *path, int *para, int *lines, int *words, int *chars); 

#endif
