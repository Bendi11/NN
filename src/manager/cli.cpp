#include "include/cli.hpp"
#include <string>

NNView::NNView(void)
{
    initscr(); //Start ncurses
    start_color(); //Start color mode in ncurses

    getmaxyx(stdscr, MAX_ROWS, MAX_COLS); //Get maximum dimensions of the screen

    cbreak();     //Make typed characters be passed directly to program
    keypad(stdscr, TRUE); //Enable arrow keys and other misc. things

    //Color pair inits
    init_pair(termCols::WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(termCols::YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(termCols::RED, COLOR_RED, COLOR_BLACK);
    init_pair(termCols::CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(termCols::BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(termCols::GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(termCols::LIME, COLOR_WHITE | COLOR_GREEN, COLOR_BLACK);
}

NNView::~NNView(void)
{
    endwin(); //Stop ncurses mode
}

void NNView::mainMenu(void) //Function to present the main menu dialog with general options
{
    wclear(stdscr); //Empty terminal of any text
    attron(A_BOLD | A_UNDERLINE | COLOR_PAIR(termCols::CYAN) ); //Attributes for the title

    mvaddstr(1, (MAX_ROWS / 2) - 9, "Neural Network Tool"); //Center title text
    attron(COLOR_PAIR(termCols::WHITE));

    refresh();
}


bool NNView::refresh(void) //Function ran every while loop  
{
    //mainMenu();
    return true;
}

