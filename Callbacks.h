//argument struct for manual training
struct TrainNetArgs
{
    Fl_Window* optionsWindow;

    Fl_Int_Input* numTrialsInput;
    Fl_Int_Input* numMutationsInput;

    NeuralNet* net;
};

//argument struct for reading user input grid
struct ReadLetterArgs
{
    std::vector<std::vector<PixelButton*>>* letterGrid;

    NeuralNet* net;
};

//argument struct for training with acccuracy threshold
struct TrainWithThresholdArgs
{
    Fl_Window* optionsWindow;

    Fl_Input* thresholdInput;
    Fl_Int_Input* numMutationsInput;

    Fl_Check_Button* exportCheckBox;

    NeuralNet* net;
};

//forward-declarations for button callbacks
void trainButtonCallback(Fl_Widget* w, void* v);
void trainNetCallback(Fl_Widget* w, void* v);
void thresholdMenuCallback(Fl_Widget* w, void* v);
void trainWithThresholdCallback(Fl_Widget* w, void* v);
void readLetterCallback(Fl_Widget* w, void* v);
