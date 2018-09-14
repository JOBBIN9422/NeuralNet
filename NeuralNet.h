#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "Neuron.h"
#include "Trainer.h"

class NeuralNet
{
    private:
        //primary net structure 
        std::vector<std::vector<Neuron>> layers;
        double netError;

    public:
        //construct a net based on the layer vector 
        NeuralNet(const std::vector<int> layers);

        //learning functions
        void feedForward(const std::vector<double>& inputVals);
        void backPropagate(const std::vector<double>& targetVals);

        //learn for a set number of batches
        void learn(int numBatches, int numBatchMutations);

        //learn until the outputs meet a threshold 
        void learnWithThreshold(double threshold, int numBatchMutations, bool exportData);

        //get the net's output values
        std::vector<double> getResults();

        //print net output versus target outputs (debugging)
        void printInfo(std::vector<double> resultVals);

        //print outputs with a given target letter
        void printOutputs(char targetLetter);

        //print outputs with no target letter
        void printOutputs();

        //get the letter associated with the strongest net output value
        char getOutputLetter();

        //get the strength of the largest output value
        double getMaxOutputVal();
};
