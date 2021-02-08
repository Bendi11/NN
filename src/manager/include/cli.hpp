#pragma once
#include <curses.h>

#include "net.hpp"

class NNView //NN CLI display class
{
public:
    NNView(void);       //Constructor to setup curses stuff
    ~NNView(void);      //Need explicit destructor to call curses shutdown functions

    bool display(void); //Function to be ran in main loop

    bool bRun = true; //Set to false if the program should exit
private:
    enum termCols
    {
        WHITE,  //White text, black BG
        YELLOW, //Yellow text, black BG
        RED,    //Red text, black BG
        CYAN,   //Cyan text, black BG
        BLUE,   //Blue text
        GREEN,  //Darker green text
        LIME,   //Lime text
    };
    neural::net NN; //The neural network object the CLI will manipulate
    
    /*Start of internal management screen functions*/
    void mainMenu(void); //Create, edit, load, etc.

    int MAX_ROWS;
    int MAX_COLS;
};
