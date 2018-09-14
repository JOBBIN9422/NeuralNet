#include <bitset>
#include <FL/Fl.H>
#include "NeuralNet.h"
#include "Frontend.h"

using namespace std;

int main()
{
    vector<int> layerStructure = {35, 30, 26};
    NeuralNet* net = new NeuralNet(layerStructure);

    Frontend* frontend = new Frontend(net);
    return Fl::run();
}
