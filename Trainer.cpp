#include "Trainer.h"

using namespace std;

std::vector<double> Trainer::parseData(int letterIndex)
{
    //partition the section of the font array based on what letter was requested
    //(each letter is 5 bytes)
    int startIndex = 5 * letterIndex;    
    int endIndex = startIndex + 5;

    //read the font data for the requested letter into a 2D char vector
    vector<string> bitStrings;
    for (int i = startIndex; i < endIndex; i++)
    {
        string currByteString = bitset<8>(Font5x7[i]).to_string();
        bitStrings.push_back(currByteString);
    }

    vector<double> outputData;
    
    //transpose and flatten the 2D bit vector into a 1D double vector 
    //(this can be used as net input)
    for (int i = bitStrings.at(0).size() - 1; i > 0; i--)
    {
        for (int j = 0; j < bitStrings.size(); j++)
        {
            //convert the current char to a double based on its ASCII value 
            //(character '0' is indexed at table #48)
            outputData.push_back(double(bitStrings.at(j).at(i) - '0'));
        }
    }

    return outputData;
}

DataSet Trainer::buildDataSet()
{
    //build input data (letters A-Z as 1D double bit vectors)
    vector<vector<double>> trainingData;
    for (int i = 0; i < 26; i++)
    {
        trainingData.push_back(Trainer::parseData(i));
    }

    //build target data (1.0 for target letter, 0.0 for all other letters)
    vector<vector<double>> targetData;
    for (int i = 0; i < 26; i++)
    {
        vector<double> currData;
        for (int j = 0; j < 26; j++)
        {
            if (i == j)
            {
                currData.push_back(1.0);
            }
            else
            {
                currData.push_back(0.0);
            }
        }
        targetData.push_back(currData);
    }

    //create a vector of training data/target data pairs 
    DataSet dataSet;
    for (int i = 0; i < trainingData.size(); i++)
    {
        dataSet.push_back(make_pair(trainingData.at(i), targetData.at(i)));
    }

    return dataSet;
}

void Trainer::mutateInputData(vector<double>& inputData, int numBitFlips)
{
    vector<bool> mutatedBits; //keep track of which bits we have modified

    //no bits have been modified initially
    for (int i = 0; i < inputData.size(); i++)
    {
        mutatedBits.push_back(false);
    }

    int mutationsRemaining = numBitFlips;

    while (mutationsRemaining > 0)
    {
        int randIndex = rand() % inputData.size();

        //reassign randIndex if we've already modified that position
        while (mutatedBits.at(randIndex))
        {
            randIndex = rand() % inputData.size();
        }

        mutateIndex(inputData, randIndex);
        
        //flag the position so we know not to flip it again
        mutatedBits.at(randIndex) = true;
        mutationsRemaining--;
    }
}

//negate the current bit
void Trainer::mutateIndex(vector<double>& inputData, int randIndex)
{
    if (inputData.at(randIndex) == 1.0)
    {
        inputData.at(randIndex) = 0.0;
    }
    else 
    {
        inputData.at(randIndex) = 1.0;
    }
}

void Trainer::mutateDataSet(DataSet& dataSet, int numMutations)
{
    //iterate over the data set and mutate each input data
    for (int i = 0; i < dataSet.size(); i++)
    {
        mutateInputData(dataSet.at(i).first, numMutations);
    }
}

DataSet Trainer::buildMutatedSet(int numElements, int numMutations)
{
    //get the unmodified dataset (26 input/target output pairs)
    DataSet dataSet = buildDataSet();

    //container for the randomly mutated data set
    DataSet mutatedSet; 

    for (int i = 0; i < numElements; i++)
    {
        mutatedSet.push_back(dataSet.at(rand() % dataSet.size()));
        mutateInputData(mutatedSet.back().first, numMutations);
    }

    return mutatedSet;
}

char Trainer::getTargetLetter(const std::vector<double>& targetData)
{
    int letterIndex = 0;
    
    //find the nonzero entry in the target data corresponding to the letter
    for (int i = 0; i < targetData.size(); i++)
    {
        if (targetData.at(i) == 1.0)
        {
            letterIndex = i;
        }
    }

    //add ascii offset to letter index to get correct letter
    return char(letterIndex + 'A');

}
