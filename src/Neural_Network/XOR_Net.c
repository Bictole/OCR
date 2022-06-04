#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double sigmoid(double z)
{
  return (double) (1/(1+exp(-z)));
}

double sigmoidPrime(double z)
{
  return (double) sigmoid(z)*(1-sigmoid(z));
}


int main()
{
    srand(time(NULL));//Initialise the randomizer

    //Some values here to describe the net
    int NbIn = 2;
    int NbHidden = 2;
    int NbOut = 1;
    int epochs = 10000;

    size_t patterns = 4;

    //The training data. Aka the most important thing for the net.
    double inputs[4][2] = {{0,0},{1,0},{0,1},{1,1}};
    double expected_outputs[4] = {0,1,1,0};
    //Somewhere to store the weighs
    double hidden_weights[patterns][NbIn][NbHidden];
    double output_weights[patterns][NbHidden][NbOut];
    //Somewhere to store the biases
    double hidden_bias[patterns][NbHidden];
    double output_bias[patterns][NbOut];
    //Somewhere to store the outputs
    double hidden_outputs[patterns][NbHidden];
    double predicted_output[patterns];
    //Storage for the deltas. You know the drill.
    double d_hidden_weights[patterns][NbIn][NbHidden];
    double d_predicted_output[patterns];
    //The sneeky breeky values that can break the entire net.
    double deviation = 0.0;
    double lr = 0.1;

    //The following 21 lines all serve to initialize the arrays defined previously
    for (size_t pat = 0; pat < patterns; pat++)
    {
        for (size_t inp = 0; inp < NbIn; inp++)
        {
            for (size_t hide = 0; hide < NbHidden; hide++)
            {
                hidden_bias[pat][hide] = (double) ((rand() % 50)/50);
                hidden_weights[pat][inp][hide] = (double) ((rand() % 50)/50);
                hidden_outputs[pat][NbHidden];
                d_hidden_weights[pat][inp][hide] = 0.0;
            }
        }

        for (size_t hide = 0; hide < NbHidden; hide++)
        {
           output_weights[pat][hide][0] = (double) ((rand() % 50)/50);
        }

        predicted_output[pat] = 0.0;
        d_predicted_output[pat] = 0.0;
        output_bias[pat][0] = (double) ((rand() % 50)/50);
    }

    for (int epoch = 0; epoch < epochs; epoch++)
    {
        deviation = 0.0;
        //Forward Pass for which we emulate the matrices using arrays and pure skill.
        for (size_t pat = 0; pat < patterns; pat++)
        {
            for (size_t hide = 0; hide < NbHidden; hide++)
            {
                double storage = 0.0;
                for (size_t inp = 0; inp < NbIn; inp++)
                {//We accumulate the activations
                    storage += (inputs[pat][inp] * hidden_weights[pat][inp][hide]);
                }//To insert them right here.
                hidden_outputs[pat][hide] = sigmoid(storage + hidden_bias[pat][hide]);
            }
            //Start accumulating the final activator
            double activator = 0.0;

            for (size_t hide = 0; hide < NbHidden; hide++)
            {//Great Scott! We are collecting the outputs from the hidden layer!
                activator += hidden_outputs[pat][hide] * output_weights[pat][hide][0];
            }

            activator += output_bias[pat][0];
            //Finally use it to predict an output
            predicted_output[pat] = sigmoid(activator);
        }
        //Backpropagation
        for (size_t pat = 0; pat < patterns; pat++)
        {
            double error = expected_outputs[pat] - predicted_output[pat]; //Output error

            deviation = error; //Deviation from expected answer

            d_predicted_output[pat] = error * sigmoidPrime(predicted_output[pat]); //Gives delta of prediction
        }
            double error_hidden[patterns][NbHidden];//Stores error of hidden layer
            double d_hidden_layer[patterns][NbHidden];//Stores the variations of the deltas of the hidden layer

        for (size_t pat = 0; pat < patterns; pat++)
        {
            for (size_t invert_hide = 0; invert_hide < NbHidden; invert_hide++)
            {//Reverse the flow and start to slowly correct the values. First come the weighs
                error_hidden[pat][invert_hide] = d_predicted_output[pat] * output_weights[pat][invert_hide][0];
            }

            for (size_t hide = 0; hide < NbHidden; hide ++)
            {//Slowly fills the delta of the hidden layer
                d_hidden_layer[pat][hide] = error_hidden[hide][0] * sigmoidPrime(hidden_outputs[pat][hide]);
            }

            for (size_t hide = 0; hide < NbHidden; hide ++)
            {//Modifies the output weighs to better suit the environment.
                output_weights[pat][hide][0] += hidden_outputs[pat][hide] * d_predicted_output[pat] * lr;
            }

            for (size_t hide = 0; hide < NbHidden; hide++)
            {
                for (size_t inp; inp < NbIn; inp++)
                {//Modifies the hidden weights to give better results.
                    hidden_weights[pat][inp][hide] = inputs[pat][inp] * d_hidden_layer[pat][hide] * lr;
                }
            }
        }

        double output_sum = 0.0;
        double hidden_sum[NbHidden];

        for (size_t pat = 0; pat < patterns; pat++)
        {//We prepare update the biases here. With much gusto, since this is nearly the end.
            output_sum += d_predicted_output[pat];

            for (size_t yos = 0; yos < NbIn; yos++)
            {
                for (size_t hide = 0; hide < NbHidden; hide++)
                {
                    hidden_sum[hide] += d_hidden_layer[pat][hide] * lr;
                }
            }
        }

        for (size_t pat = 0; pat < patterns; pat++)
        {//We update the biases here, savouring the sweet air of freedom.
            output_bias[pat][0] += output_sum * lr;
            for (size_t hide = 0; hide < NbHidden; hide++)
            {
                    hidden_bias[pat][hide] += hidden_sum[hide] * lr;
            }
        }

        if (epoch % 100 == 0)
        {//As is the tradition, we leave a small note at the door as we leave, detailing the changes over the epochs
            printf("Current epoch: %-4d; Deviation = %f\n", epoch, deviation);
            for (size_t pat = 0; pat < patterns; pat++)
                printf("Pattern n°: %d | Input 01 : %f | Input 02 : %f | Output: %f \n", pat, inputs[pat][0], inputs[pat][1], predicted_output[pat]);
        }

    }//PS: All hail the inventors of coffee!
    return 0;
}
