#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Matrix.h"
#include "netStruct.h"

void saveInt(FILE *file, unsigned int number)
{
    unsigned int temp = number;
    unsigned int nb = 0;
    while (temp > 0)
    {
        temp /= 10;
        nb++;
    }
    temp = number;
    while(nb > 1)
    {
        fputc(temp / (unsigned int) powl(10, nb - 1) + 48, file);
        temp %= (unsigned int) powl(10, nb - 1);
        nb--;
    }
    fputc(temp % 10 + 48, file);
}

void saveDouble(FILE *file, double number)
{
    //NEGATIVE
    if (number < 0)
    {
        fprintf(file, "-");
        number = -number;
    }

    unsigned int ipart = (unsigned int)number;
    double fpart = number - (double)ipart;

    saveInt(file, ipart);
    fprintf(file, ".");

    //NON-INT SAVE
    unsigned int decim = 6;
    if (decim != 0)
    {
        fpart = fpart * powl(10, decim);
        saveInt(file, (unsigned int)fpart);
    }
}


void saveNetwork(neuralNetwork *net)
{
    FILE *saveFile;
    saveFile = fopen("./Savepoint.txt", "w");

    saveInt(saveFile, (unsigned int) net->nbIn);
    fprintf(saveFile, "\n");

    saveInt(saveFile, (unsigned int) net->nbHidden);
    fprintf(saveFile, "\n");

    saveInt(saveFile, (unsigned int) net->nbOut);
    fprintf(saveFile, "\n");

    for (size_t i = 0; i < net->nbHidden; i++)
    {
        saveDouble(saveFile, net->hiddenBias->data[i]);
        fprintf(saveFile, "\n");
    }

    for (size_t i = 0; i < net->nbOut; i++)
    {
        saveDouble(saveFile, net->outputBias->data[i]);
        fprintf(saveFile, "\n");
    }

    for (size_t i = 0; i < net->nbIn * net->nbHidden; i++)
    {
        saveDouble(saveFile, net->hiddenWeights->data[i]);
        fprintf(saveFile, "\n");
    }

    for (size_t i = 0; i < net->nbHidden * net->nbOut; i++)
    {
        saveDouble(saveFile, net->outputWeights->data[i]);
        fprintf(saveFile, "\n");
    }
}

void loadNetwork (neuralNetwork *net)
{
    FILE *loadFile;
    loadFile = fopen("./Savepoint.txt", "r");

    char *Inline = NULL;
    ssize_t len = 0;
    ssize_t np = getline(&Inline, &len, loadFile);
    np++;
    sscanf(Inline, "%d", &net->nbIn);

    char *Hline = NULL;
    len = 0;
    np = getline(&Hline, &len, loadFile);
    np++;
    sscanf(Hline, "%d", &net->nbHidden);

    char *Oline = NULL;
    len = 0;
    np = getline(&Oline, &len, loadFile);
    np++;
    sscanf(Oline, "%d", &net->nbOut);

    //Free the previous matrices?

    net->hiddenBias = build(net->nbHidden, 1);
    net->outputBias = build(net->nbOut, 1);

    net->hiddenWeights = build(net->nbHidden, net->nbIn);
    net->outputWeights = build(net->nbOut, net->nbHidden);

    for (size_t i = 0; i < net->nbHidden; i++)
    {
        char *line = NULL;
        ssize_t len = 0;
        ssize_t np = getline(&line, &len, loadFile);
        np++;
        sscanf(line, "%le", &net->hiddenBias->data[i]);
    }

    for (size_t i = 0; i < net->nbOut; i++)
    {
        char *line = NULL;
        ssize_t len = 0;
        ssize_t np = getline(&line, &len, loadFile);
        np++;
        sscanf(line, "%le", &net->outputBias->data[i]);
    }

    for (size_t i = 0; i < net->nbIn * net->nbHidden; i++)
    {
        char *line = NULL;
        ssize_t len = 0;
        ssize_t np = getline(&line, &len, loadFile);
        np++;
        sscanf(line, "%le", &net->hiddenWeights->data[i]);
    }

    for (size_t i = 0; i < net->nbHidden * net->nbOut; i++)
    {
        char *line = NULL;
        ssize_t len = 0;
        ssize_t np = getline(&line, &len, loadFile);
        np++;
        sscanf(line, "%le", &net->outputWeights->data[i]);
    }
}
