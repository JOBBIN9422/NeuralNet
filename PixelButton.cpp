#include "PixelButton.h"

using namespace std;

PixelButton::PixelButton(int windowX, int windowY, int rowNum, int colNum, int buttonSize) 
: Fl_Button(windowX, windowY, buttonSize, buttonSize), rowNum(rowNum), colNum(colNum), isMarked(false)
{
    //draw buttons as black/white boxes with border
    this->box(FL_BORDER_BOX);
    this->color(FL_WHITE);
}

int PixelButton::handle(int event)
{
    switch (event)
    {
        //when clicked 
        case FL_PUSH:
            if (Fl::event_button() == FL_LEFT_MOUSE)
            {
                //toggle cell state
                this->isMarked = !this->isMarked;

                //toggle cell color
                if (this->color() == FL_WHITE)
                {
                    this->color(FL_BLACK);
                }
                else
                {
                    this->color(FL_WHITE);
                }
                this->redraw();

                return 1;
            }
            
        default:
            return Fl_Widget::handle(event);
    }
}

bool PixelButton::checkMarked()
{
    return this->isMarked;
}
