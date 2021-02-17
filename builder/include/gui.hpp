#pragma once

#include <future> //For allowing NN to compute things in the background while the screen refreshes
#include <chrono>
#include <string>

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <glad/glad.h>

#include <net.hpp>

class NNView
{
public:

    bool display(); //Function called every frame 

private:

    neural::net NN; //Internal neural network object
    std::future<void> bgProcess; //Background process future object

    std::string statusString; //The status string to display on the main menu bar
    std::string processString; //The string displaying any currently running processes
    
    void creationWin(); //Function to display the NN creation window

    void trainWin();    //Function to display NN training window

    void dataWin();     //Function to load data from file / manifest file
};
