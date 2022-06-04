#ifndef NEURAL_NET_AUXILIA_H
#define NEURAL_NET_AUXILIA_H

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Matrix.h"
#include "netStruct.h"

void saveNetwork(neuralNetwork *net);

void loadNetwork (neuralNetwork *net);

#endif
