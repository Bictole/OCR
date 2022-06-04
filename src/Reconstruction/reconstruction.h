#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

//#include "../Neural_Network/Matrix.h"

void Reconstruction(char *final);

//matrix* CharMatrix(int ymin, int ymax, int xmin, int xmax, SDL_Surface image_surface);

void Build_final_char(char *final, int *para, int *lines, int *words, int *chars/*, char *path*/);

int Length(int* array);

#endif
