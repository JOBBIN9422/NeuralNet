#pragma once
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>

class PixelButton : public Fl_Button
{
    private:
        //position variables
        int rowNum;
        int colNum;

        //is the current cell 'on'?
        bool isMarked;

    public:
        PixelButton(int windowX, int windowY, int rowNum, int colNum, int buttonSize);

        //FLTK event handler
        int handle(int event);

        bool checkMarked();
};
