typedef struct Net
{
    //META VALUES
    int nbIn;
    int nbHidden;
    int nbOut;

    //INTERNAL VALUES
    matrix* hiddenWeights;
    matrix* outputWeights;

    matrix* hiddenBias;
    matrix* outputBias;

    matrix* currentOutput;

    double deviationPercentage;
} neuralNetwork;
