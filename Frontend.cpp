#include "Frontend.h"

using namespace std;

void trainButtonCallback(Fl_Widget* w, void* v)
{
    Fl_Window* optionsWindow = (Fl_Window*)v;
    optionsWindow->show();
}

void trainNetCallback(Fl_Widget* w, void* v)
{
    //unpack arguments from argument struct
    TrainNetArgs* arguments = (TrainNetArgs*)v;
    NeuralNet* net = (NeuralNet*)arguments->net;
    Fl_Window* optionsWindow = (Fl_Window*)arguments->optionsWindow;
    Fl_Int_Input* numTrialsInput = (Fl_Int_Input*)arguments->numTrialsInput;
    Fl_Int_Input* numMutationsInput = (Fl_Int_Input*)arguments->numMutationsInput;

    optionsWindow->hide();

    //store user input
    int numTrials;
    int numMutations;

    try
    {
        //read user values from numeric inputs
        numTrials = stoi(numTrialsInput->value()); 
        numMutations = stoi(numMutationsInput->value()); 
    }
    catch (invalid_argument& error)
    {
        fl_message("Please enter integer values for trial and mutation count.");
        return;
    }

    //train the net for the user-specified number of trials
    net->learn(numTrials, numMutations);

    fl_message("Training complete.");
}

void thresholdMenuCallback(Fl_Widget* w, void* v)
{
    Fl_Window* optionsWindow = (Fl_Window*)v;
    optionsWindow->show();
}

void trainWithThresholdCallback(Fl_Widget* w, void* v)
{
    TrainWithThresholdArgs* arguments = (TrainWithThresholdArgs*)v;

    Fl_Window* optionsWindow = arguments->optionsWindow;
    Fl_Input* thresholdInput = arguments->thresholdInput;
    Fl_Int_Input* numMutationsInput = arguments->numMutationsInput;
    Fl_Check_Button* exportCheckBox = arguments->exportCheckBox;
    NeuralNet* net = arguments->net;

    optionsWindow->hide();

    double threshold;
    int numMutations;

    try
    {
        threshold = stod(thresholdInput->value()); 
        numMutations = stoi(numMutationsInput->value()); 
    }
    catch (invalid_argument& error)
    {
        fl_message("Please enter valid values for threshold and mutation count.");
        return;
    }

    net->learnWithThreshold(threshold, numMutations, exportCheckBox->value());

    fl_message("Training complete.");
}

void readLetterCallback(Fl_Widget* w, void* v)
{
    //unpack arguments from argument struct
    ReadLetterArgs* arguments = (ReadLetterArgs*)v;
    vector<vector<PixelButton*>>* letterGrid = arguments->letterGrid;
    NeuralNet* net = arguments->net;

    //container for user-entered letter
    vector<double> userLetterInput;

    //iterate across the button grid and build the 35-element input vector
    for (int i = 0; i < letterGrid->at(0).size(); i++)
    {
        for (int j = 0; j < letterGrid->size(); j++)
        {
            //push back float values (net reads float inputs)
            if (letterGrid->at(j).at(i)->checkMarked())
            {
                userLetterInput.push_back(1.0);
            }
            else
            {
                userLetterInput.push_back(0.0);
            }
        }
    }
    
    //send user-entered letter to net and print results
    net->feedForward(userLetterInput);
    net->printOutputs();

    //get output info and notify the user
    char outputChar = net->getOutputLetter();
    double outputConfidence = net->getMaxOutputVal() * 100;
    fl_message("I read the letter %c (%f%% certainty). See terminal for detailed output."
               , outputChar, outputConfidence);
}

Frontend::Frontend(NeuralNet* net) : Fl_Window(280, 440, "Neural Network"), net(net)
{
    //define grid parameters
    int buttonSize = 40;
    int numRows = 7;
    int numCols = 5;

    //construct the letter input buttons
    for (int i = 0; i < numCols; i++)
    {
        vector<PixelButton*> currRow;
        for (int j = 0; j < numRows; j++)
        {
            currRow.push_back(new PixelButton(buttonSize * i + buttonSize, 
                              buttonSize * j + 10, j, i, buttonSize));
        }

        this->letterGrid.push_back(currRow);
    }

    //construct home-screen menu buttons
    trainMenuButton = new Fl_Button(40, 380, 200, 40, "Train for n Trials");
    thresholdMenuButton = new Fl_Button(40, 340, 200, 40, "Train until Threshold");
    readLetterButton = new Fl_Button(40, 300, 200, 40, "Read Grid");
    
    //Create argument struct for letter reading
    ReadLetterArgs* readArgs = new ReadLetterArgs();     
    readArgs->letterGrid = &this->letterGrid;
    readArgs->net = this->net;
    readLetterButton->callback(readLetterCallback, readArgs);

    //create the manual training options window and inputs
    trainOptionsWindow = new Fl_Window(180, 140, "Training Options");

    numTrialsInput = new Fl_Int_Input(110, 20, 60, 20, "Trial Count");
    numMutationsInput = new Fl_Int_Input(110, 60, 60, 20, "Mutation Count");

    //set default values for training inputs
    numTrialsInput->value("10000");
    numMutationsInput->value("0");

    //create manual training arguments
    trainNetButton = new Fl_Button(40, 100, 100, 20, "Train Net");
    TrainNetArgs* netArgs = new TrainNetArgs();
    netArgs->optionsWindow = this->trainOptionsWindow;
    netArgs->numTrialsInput = this->numTrialsInput;
    netArgs->numMutationsInput = this->numMutationsInput;
    netArgs->net = this->net;

    //assign training callbacks
    trainNetButton->callback(trainNetCallback, netArgs);
    trainMenuButton->callback(trainButtonCallback, trainOptionsWindow);

    //create threshold training window and inputs
    thresholdOptionsWindow = new Fl_Window(180, 140, "Training Options");
    numMutationsThresholdInput = new Fl_Int_Input(110, 60, 60, 20, "Mutation Count");
    thresholdInput = new Fl_Input(110, 20, 60, 20, "Threshold");
    trainWithThresholdButton = new Fl_Button(40, 100, 100, 20, "Train Net");
    exportCheckBox = new Fl_Check_Button(10, 120, 150, 20, "Export data as .xls");

    //set default values for training inputs
    thresholdInput->value("0.9");
    numMutationsThresholdInput->value("0");

    //create threshold training arguments
    TrainWithThresholdArgs* thresholdArgs = new TrainWithThresholdArgs();
    thresholdArgs->optionsWindow = this->thresholdOptionsWindow;
    thresholdArgs->thresholdInput = this->thresholdInput;
    thresholdArgs->numMutationsInput = this->numMutationsThresholdInput;
    thresholdArgs->exportCheckBox = this->exportCheckBox;
    thresholdArgs->net = this->net;

    //assign callbacks
    thresholdMenuButton->callback(thresholdMenuCallback, thresholdOptionsWindow);
    trainWithThresholdButton->callback(trainWithThresholdCallback, thresholdArgs);


    this->show();
    fl_message("Please train the neural net by selecting one of the provided options.\n (Default values have been added for convenience.)");
}
