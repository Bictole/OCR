#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "Segmentation/segmentation.h"
#include "Segmentation/rlsa.h"
#include "Tools/tools.h"
#include "Black_White/Black_White.h"
#include "Reconstruction/reconstruction.h"
#include "Tools/load_image.h"


void Segmentation(char *path)
{
    int para[50] = {0};
    int lines[200] = {0};
    int words[4000] = {0};
    int chars[8000] = {0};

    segmentation(path, para, lines, words, chars);//mettre en arg le path de l'img recup avec l'interface

    int lenchars = Length(chars)/2;
    int lenlines = Length(lines)/2;
    int lenpara = Length(lines)/2;
    int lenwords = Length(words)/2;

    //Initializing the final "string"
    int lenfinal = lenchars + lenlines + lenwords + lenpara;
    char final[lenfinal];

    //Reconstruction();
    Build_final_char(final, para, lines, words, chars/*, path*/);//changer path
    Reconstruction(final);

    /*for(int i = 0; i < lenfinal; i ++)
    {
        printf("%c", final[i]);
	}*/
}


int main()
{
    Segmentation("Test_img/algo.png");
    Segmentation("Test_img/rap.png");
    Segmentation("Test_img/rap2.png");
    Segmentation("Test_img/image9.png");
    Segmentation("Test_img/image7.png");
    Segmentation("Test_img/test.bmp");

    return 0;
}
