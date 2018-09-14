#pragma once
#include <vector>
#include <cstdlib>
#include <cmath>

class Neuron
{
    private:
        //the neuron's output value as determined by its transfer function
        double outputVal;
        
        //gradient value - used in gradient descent to correct errors
        double gradient;

        int index; //neuron's position in the current layer

        bool isOutput; //is the neuron in the output layer?

        //store connections and differences (for error correction)
        std::vector<double> outputWeights;
        std::vector<double> deltaWeights;

        static double learningRate; //overall net learning rate
        static double alpha;        //multiplier for weight changes (momentum)

        //helper function; creates random weights during net construction
        static double getRandomWeight();

        //activation functions/derivatives
        static double sigmoidFunction(double val);
        static double sigmoidPrime(double val);
        static double tanHPrime(double val);

        //accumulate net errors 
        double sumDOW(const std::vector<Neuron>& nextLayer);

    public:
        Neuron(int numOutputs, int index, bool isOutput);

        double getOutputVal();

        void setOutputVal(double outputVal);
        void feedForward(const std::vector<Neuron>& prevLayer);
        void calcOutputGradients(double targetVal);
        void calcHiddenGradients(const std::vector<Neuron>& nextLayer);
        void updateInputWeights(std::vector<Neuron>& prevLayer);
};
