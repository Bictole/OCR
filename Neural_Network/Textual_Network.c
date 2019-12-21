#include "Matrix.h"
#include "Neural_Net_Auxilia.h"


neuralNetwork *Genesis(int In, int Hid, int Out)
{
    neuralNetwork* net = malloc(sizeof(neuralNetwork));

    net->nbIn = In;
    net->nbHidden = Hid;
    net->nbOut = Out;

    net->hiddenWeights = build(Hid, In);
    mat_random_fill(net->hiddenWeights);

    net->outputWeights = build(Out, Hid);
    mat_random_fill(net->outputWeights);

    net->hiddenBias = build(Hid, 1);
    mat_random_fill(net->hiddenBias);

    net->outputBias = build(Out, 1);
    mat_random_fill(net->outputBias);

    net->currentOutput = build(1, Out);
    mat_fill(net->currentOutput, 0);

    net->deviationPercentage = 0;

    return net;
}

void update(neuralNetwork* net, matrix* input, matrix* expectedOutput, double eta)
{
    matrix* z = mat_add(mat_dot(net->hiddenWeights, input), net->hiddenBias);

    matrix* activation1 = sigmoid(z);//Applies sigmoid to each element

    matrix* zH = mat_add(mat_dot(net->outputWeights, activation1), net->outputBias);
    //Passes the hidden output through the weights of the final layer and applies bias

    mat_free(net->currentOutput);

    net->currentOutput = sigmoid(zH);//Calculates the current output

    //Backpropagation

    matrix* delta = mat_memb_mult(mat_sub(net->currentOutput, expectedOutput), sigmoidPrime(zH));//Calculate the delta

    net->deviationPercentage = mat_sub(net->currentOutput, expectedOutput)->data[0];//Note down the deviation

    matrix *sp = sigmoidPrime(z);//Get the softener of the change

    matrix* deltaH = mat_memb_mult(mat_dot(mat_transpose(net->outputWeights), delta), sp);//Get value changes, then soften with sigprime
    //Calculate the modifiers of the net's interior.
    matrix* nbo = delta;
    matrix* nbh = deltaH;

    matrix* nwo = mat_dot(delta, mat_transpose(activation1));

    matrix* nwh = mat_dot(deltaH, mat_transpose(input));
    //Apply the modifiers to the net's values
    net->outputWeights = mat_sub(net->outputWeights, mat_mult(nwo, eta));
    net->hiddenWeights = mat_sub(net->hiddenWeights, mat_mult(nwh, eta));
    net->outputBias = mat_sub(net->outputBias, mat_mult(nbo, eta));
    net->hiddenBias = mat_sub(net->hiddenBias, mat_mult(nbh, eta));
    net->currentOutput = SoftMax(zH);//Weight all the matrix positions against each other
}

void evaluate(neuralNetwork* net, matrix*** test_data)
{

}

void SGD(neuralNetwork* net, matrix ***training_data, int epochs, double eta, int len)
{
    for (int ep = 0; ep<epochs; ep++)
    {
        //Shuffle training data
        for (int b = 0; b < len; b++)
        {
            update(net, training_data[b][0], training_data[b][1], eta);
        }
    }
}



int main()
{
    srand(time(NULL));

    matrix*** data = malloc(4*2*sizeof(matrix**));

    matrix** e1 = malloc(2*sizeof(matrix*));
    matrix** e2 = malloc(2*sizeof(matrix*));
    matrix** e3 = malloc(2*sizeof(matrix*));
    matrix** e4 = malloc(2*sizeof(matrix*));



    matrix* in0 = build (2,1);

    mat_mod_val(in0, 0, 0, 0);
    mat_mod_val(in0, 1, 0, 0);

    e1[0] = in0;

    matrix* in1 = build (2,1);

    mat_mod_val(in1, 0, 0, 0);
    mat_mod_val(in1, 1, 0, 1);

    e2[0] = in1;

    matrix* in2 = build (2,1);

    mat_mod_val(in2, 0, 0, 1);
    mat_mod_val(in2, 1, 0, 0);

    e3[0] = in2;

    matrix* in3 = build (2,1);

    mat_mod_val(in3, 0, 0, 1);
    mat_mod_val(in3, 1, 0, 1);

    e4[0] = in3;

    matrix* out0 = build(2,1);
    mat_mod_val(out0, 1, 0, 1);
    e1[1] = out0;
    matrix* out1 = build(2,1);
    mat_mod_val(out1, 0, 0, 1);
    e2[1] = out1;
    matrix* out2 = build(2,1);
    mat_mod_val(out2, 0, 0, 1);
    e3[1] = out2;
    matrix* out3 = build(2,1);
    mat_mod_val(out3, 1, 0, 1);
    e4[1] = out3;

    data[0]=e1;
    data[1]=e2;
    data[2]=e3;
    data[3]=e4;

    int epochs = 1001;

    neuralNetwork* sun = Genesis(2, 2, 2);

    for (int ep = 0; ep<epochs; ep++)
    {
        for (int p = 0; p < 4; p++)
        {
            update(sun, data[p][0], data[p][1], 0.1);
            if (ep % 100 == 0)
            {
                printf("Current epoch: %-4d; Deviation = %f\n", ep, sun->deviationPercentage);
                printf("Pattern n°: %d\n", p);
                printf("Input: \n");
                print_matrix(data[p][0]);
                printf("Output: \n");
                print_matrix(sun->currentOutput);
                printf("Expected:\n");
                print_matrix(data[p][1]);
                printf("\n");
            }
        }
    }
    saveNetwork(sun);
    return 0;
}
