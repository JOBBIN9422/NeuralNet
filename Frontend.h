#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Input.H>
#include <FL/fl_message.H>
#include <vector>
#include <stdexcept>
#include "PixelButton.h"
#include "NeuralNet.h"
#include "Callbacks.h"

class Frontend : public Fl_Window
{
    private:
        //the 2D button grid for user input
        std::vector<std::vector<PixelButton*>> letterGrid;
        NeuralNet* net;

        //Training settings windows (opened on button-click)
        Fl_Window* trainOptionsWindow;
        Fl_Window* thresholdOptionsWindow;

        Fl_Button* trainMenuButton;   //open the training options
        Fl_Button* trainNetButton;    //train the neural net (submit options)
        Fl_Button* thresholdMenuButton;    //train the neural net according to threshold
        Fl_Button* trainWithThresholdButton;    //train the neural net according to threshold
        Fl_Button* readLetterButton;

        Fl_Check_Button* exportCheckBox;    //flag to export training data

        //training settings inputs
        Fl_Int_Input* numTrialsInput;
        Fl_Int_Input* numMutationsInput;
        Fl_Int_Input* numMutationsThresholdInput;

        //training settings inputs
        Fl_Input* letterInput;
        Fl_Input* thresholdInput;

    public:
        Frontend(NeuralNet* net);
};
