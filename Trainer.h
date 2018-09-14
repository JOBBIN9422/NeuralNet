#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "Font.h"

typedef std::vector<std::pair<std::vector<double>, std::vector<double>>> DataSet;
class Trainer
{
    public:
        //parse the font array and return a 35-element bit vector representing the requested letter
        static std::vector<double> parseData(int letterIndex);

        //build the 26 input/target data pairs for letters A-Z
        static DataSet buildDataSet();

        //build a dataset of the specified size with a specified number of mutations to each letter
        static DataSet buildMutatedSet(int numElements, int numMutations);
       
        //mutate an input set by randomly flipping a specified number of bits
        static void mutateInputData(std::vector<double>& inputData, int numBitFlips);
        static void mutateDataSet(DataSet& dataSet, int numMutations);
        static void mutateIndex(std::vector<double>& inputData, int randIndex);

        //return the letter represented by an output data vector 
        static char getTargetLetter(const std::vector<double>& targetData);
};
