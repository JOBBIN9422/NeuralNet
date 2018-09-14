#include "Neuron.h"

using namespace std;

double Neuron::learningRate = 0.10;
double Neuron::alpha = 0.3;

Neuron::Neuron(int numOutputs, int index, bool isOutput) 
: index(index), isOutput(isOutput), outputVal(0.0), gradient(0.0)

{
    for (int i = 0; i < numOutputs; i++)
    {
        //initialize weight container and delta container
        this->outputWeights.push_back(getRandomWeight());
        this->deltaWeights.push_back(getRandomWeight());
    }
}

void Neuron::setOutputVal(double outputVal)
{
    this->outputVal = outputVal;
}

double Neuron::getOutputVal()
{
    return this->outputVal;
}

//return a random weight between 0.0 and 1.0
double Neuron::getRandomWeight()
{
    return rand() / double(RAND_MAX);
}

double Neuron::sigmoidFunction(double val)
{
    return (1.0 / (1.0 + exp(-1 * val)));
}

//sigmoid function derivative
double Neuron::sigmoidPrime(double val)
{
    double sigmoidVal = sigmoidFunction(val);
    return sigmoidVal * (1.0 - sigmoidVal);
}

//hyperbolic tangent derivative
double Neuron::tanHPrime(double val)
{
    double tanHVal = tanh(val);
    return 1.0 - tanHVal * tanHVal;
}

//accumulate weight errors (for back-propagation)
double Neuron::sumDOW(const std::vector<Neuron>& nextLayer)
{
    double sum = 0.0;

    //sum error contributions
    for (int i = 0; i < nextLayer.size() - 1; i++)
    {
        sum += this->outputWeights.at(i) * nextLayer.at(i).gradient;
    }

    return sum;
}

void Neuron::feedForward(const vector<Neuron>& prevLayer)
{
    double sum = 0.0;

    //calculate weighted sum of the previous layer's outputs
    for (int i = 0; i < prevLayer.size(); i++)
    {
        sum += prevLayer.at(i).outputVal * prevLayer.at(i).outputWeights.at(this->index);
    }

    //this->outputVal = tanh(sum);
    //evaluate the activation function for the sum (logistic sigmoid for output layer)
    if (this->isOutput)
    {
        this->outputVal = sigmoidFunction(sum);
    }
    else
    {
        this->outputVal = tanh(sum);
    }
}

void Neuron::calcOutputGradients(double targetVal)
{
    double difference = targetVal - this->outputVal;
    this->gradient = difference * sigmoidPrime(this->outputVal);
}

void Neuron::calcHiddenGradients(const std::vector<Neuron>& nextLayer)
{
    double derivedWeights = sumDOW(nextLayer);
    this->gradient = derivedWeights * tanHPrime(this->outputVal);
}

void Neuron::updateInputWeights(std::vector<Neuron>& prevLayer)
{
    //update all connections
    for (int i = 0; i < prevLayer.size(); i++)
    {
        Neuron& currNeuron = prevLayer.at(i);
        double oldDiff = currNeuron.deltaWeights.at(this->index);

        //newDiff consists of overall learning rate, momentum (alpha), output value, and gradient
        double newDiff = 
        learningRate * 
        currNeuron.getOutputVal() *
        this->gradient + 
        alpha *
        oldDiff;

        //update weight vectors for the current neuron
        currNeuron.deltaWeights.at(this->index) = newDiff;
        currNeuron.outputWeights.at(this->index) += newDiff;
    }
}
