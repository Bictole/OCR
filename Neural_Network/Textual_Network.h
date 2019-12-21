#ifndef TEXTUAL_NETWORK_H
#define TEXTUAL_NETWORK_H

void SGD(neuralNetwork* net, matrix ***training_data, int epochs, double eta, int len);

void identify(neuralNetwork* net, matrix* input);

#endif
