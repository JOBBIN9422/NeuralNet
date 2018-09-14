#include "NeuralNet.h"
using namespace std;

NeuralNet::NeuralNet(const vector<int> layers) : netError(0.0)
{
    int numLayers = layers.size();

    for (int i = 0; i < numLayers; i++)
    {
        vector<Neuron> currLayer;

        int numOutputs = 0;
        if (i == numLayers - 1)
        {
            numOutputs = 0;
        }
        else
        {
            numOutputs = layers.at(i + 1);
        }

        //add each neuron (and the bias neuron)
        for (int j = 0; j <= layers.at(i); j++)
        {
            currLayer.push_back(Neuron(numOutputs, j, (i == numLayers - 1)));
        }

        //set bias neuron value
        currLayer.back().setOutputVal(1.0);

        //add the newly constructed layer to the 2D layer container
        this->layers.push_back(currLayer);
    }
}

void NeuralNet::feedForward(const vector<double>& inputVals)
{
    int correctNumInputs = layers.at(0).size() - 1;

    //check if input size is equal to number of input neurons (without bias neuron)
    if (inputVals.size() != correctNumInputs)
    {
        //throw exception/handle errors here 
        return;
    }

    //assign input values to input layer of network
    for (int i = 0; i < inputVals.size(); i++)
    {
        layers.at(0).at(i).setOutputVal(inputVals.at(i));
    }

    //iterate across the network and "feed forward" the weights (ignore input neurons)
    for (int i = 1; i < layers.size(); i++)
    {
        vector<Neuron>& prevLayer = layers.at(i - 1);
        //loop across each neuron in the current layer (ignore bias neuron)
        for (int j = 0; j < layers.at(i).size() - 1; j++)
        {
            layers.at(i).at(j).feedForward(prevLayer);
        }
    }
}

void NeuralNet::backPropagate(const vector<double>& targetVals)
{
    //calculate RMS error of entire net
    vector<Neuron>& outputLayer = this->layers.back();
    this->netError = 0.0;

    //calculate the sum of squared differences between output and target outputs
    for (int i = 0; i < outputLayer.size() - 1; i++)
    {
        double difference = targetVals.at(i) - outputLayer.at(i).getOutputVal();
        this->netError += difference * difference;
    }

    //root mean square of errors
    this->netError /= outputLayer.size() - 1;
    this->netError = sqrt(this->netError);

    //calculate output layer gradients
    for (int i = 0; i < outputLayer.size() - 1; i++)
    {
        outputLayer.at(i).calcOutputGradients(targetVals.at(i));
    }
    

    //calculate hidden layer gradients
    for (int i = this->layers.size() - 2; i > 0; i--)
    {
        vector<Neuron>& currLayer = this->layers.at(i);
        vector<Neuron>& nextLayer = this->layers.at(i + 1);

        for (int j = 0; j < currLayer.size(); j++)
        {
            currLayer.at(j).calcHiddenGradients(nextLayer);
        } 
    }

    //for all layers except input : update connection weights
    for (int i = this->layers.size() - 1; i > 0; i--)
    {
        vector<Neuron>& currLayer = this->layers.at(i);
        vector<Neuron>& prevLayer = this->layers.at(i - 1);

        //ignore bias neuron
        for (int j = 0; j < currLayer.size() - 1; j++)
        {
            currLayer.at(j).updateInputWeights(prevLayer);
        }
    }
}

vector<double> NeuralNet::getResults()
{
    vector<double> resultVals;

    //retrieve the output value of each output neuron
    for (int i = 0; i < this->layers.back().size() - 1; i++)
    {
        resultVals.push_back(this->layers.back().at(i).getOutputVal());
    }

    return resultVals;
}

void NeuralNet::printInfo(vector<double> targetVals)
{
    vector<double> resultVals = this->getResults();

    //print letters by shifting ASCII 'A' value
    for (int i = 0; i < resultVals.size(); i++)
    {
        cout << char(i + 'A') << ": " << resultVals.at(i) << "\t" 
        << targetVals.at(i) << endl;
    }
}

//print output values to terminal with a target letter in title
void NeuralNet::printOutputs(char targetLetter)
{
    cout << "---------------------------" << endl << "Outputs for input letter " 
         << targetLetter << ":" << endl
         << "---------------------------" << endl;

    vector<double> resultVals = this->getResults();

    for (int i = 0; i < resultVals.size(); i++)
    {
        cout << char(i + 'A') << ": " << resultVals.at(i) << endl;
    }
}

//print output values to terminal 
void NeuralNet::printOutputs()
{
    cout << "---------------------------" << endl << "Outputs" << endl 
         << "---------------------------" << endl;

    vector<double> resultVals = this->getResults();

    for (int i = 0; i < resultVals.size(); i++)
    {
        cout << char(i + 'A') << ": " << resultVals.at(i) << endl;
    }

}

//get the character corresponding to the strongest net output
char NeuralNet::getOutputLetter()
{
    int maxIndex = 0;
    double maxVal = numeric_limits<double>::lowest();

    vector<double> resultVals = this->getResults();

    for (int i = 0; i < resultVals.size(); i++)
    {
        //update max value and position if we find a larger output 
        if (resultVals.at(i) > maxVal)
        {
            maxVal = resultVals.at(i);
            maxIndex = i;
        }
    }

    //return ASCII representation (shift ASCII 'A' by max index)
    return char(maxIndex + 'A');
}

//get the net's strongest output value
double NeuralNet::getMaxOutputVal()
{
    double maxVal = numeric_limits<double>::lowest();

    vector<double> resultVals = this->getResults();

    for (int i = 0; i < resultVals.size(); i++)
    {
        //update max value if we find a larger output value
        if (resultVals.at(i) > maxVal)
        {
            maxVal = resultVals.at(i);
        }
    }

    return maxVal;
}

//train for a specified number of trials
void NeuralNet::learn(int numBatches, int numBatchMutations)
{
    //build a dataset of randomly mutated letters and their target values
    DataSet dataSet = Trainer::buildMutatedSet(numBatches, numBatchMutations);

    //iterate over the dataset and teach the net 
    for (int i = 0; i < dataSet.size(); i++)
    {
        cout << "Trial " << i << " ... ";

        vector<double> trainingData = dataSet.at(i).first;
        vector<double> targetData = dataSet.at(i).second;

        this->feedForward(trainingData);
        this->backPropagate(targetData);

        cout << "done" << endl;
    }
}

//train until the net reaches an accuracy threshold 
void NeuralNet::learnWithThreshold(double threshold, int numBatchMutations, bool exportData)
{
    //create and mutate the data set for training
    DataSet dataSet = Trainer::buildDataSet();
    //Trainer::mutateDataSet(dataSet, numBatchMutations);

    int numCorrectOutputs = 0;
    int numTrials = 0;

    //write to file if user specified
    ofstream dataFile;

    if (exportData)
    {
        dataFile.open("trainingData.xls");
        dataFile << "Trial" << "\t" << "Accuracy" << "\r";
    }

    double accuracyPercentage = 0.0;
    do
    {
        cout << "Trial " << numTrials << " ... ";

        int randIndex = rand() % dataSet.size();

        //fetch input and output data
        vector<double> trainingData = dataSet.at(randIndex).first;
        vector<double> targetData = dataSet.at(randIndex).second;

        //mutate data set 
        if (numBatchMutations  > 0)
        {
            Trainer::mutateInputData(trainingData, numBatchMutations);
        }

        //teach the net on the current batch
        this->feedForward(trainingData);
        this->backPropagate(targetData);

        //get the net's output character and the correct output character
        char outputChar = this->getOutputLetter();
        char targetChar = Trainer::getTargetLetter(targetData);

        //if net output the correct character, keep a tally of correct outputs
        if (outputChar == targetChar)
        {
            numCorrectOutputs++;
        }

        //calculate accuracy
        numTrials++;
        accuracyPercentage = double(numCorrectOutputs) / double(numTrials)  * 100;

        //write to file
        if (exportData)
        {
            dataFile << numTrials << "\t" << accuracyPercentage << "\r";
        }

        cout << "done" << " (" << accuracyPercentage
             << "\% correct outputs)" << endl;

    } while (accuracyPercentage < threshold * 100 || numTrials < 50);

    if (exportData)
    {
        dataFile.close();
    }
}
