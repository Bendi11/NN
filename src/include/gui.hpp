#pragma once

//All UI library includes
#include <imgui.h>
#include <SDL.h>
#include <glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

//Multithreading libraries to make NN do stuff without the UI freezing
#include <thread>
#include <future>
#include <chrono>

#include "net.hpp"
#include "dataLoader.hpp"

namespace GUI
{

using namespace std::chrono_literals; //For ms, us

extern SDL_Window* window;
extern SDL_GLContext glContext;
void initScreen(unsigned int w, unsigned int h);



class NNGUI //Neural network GUI class
{
public:
    void presentCreateWin(); //Function to draw the Dear ImGui window for creating a NN 
    void presentTrainWin();  //Function to draw Dear ImGui NN training window
    void presentDataWin(); //Function to display data loading from folder with manifest.json
    
    void drawNN();           //Function to draw all windows
    NNGUI() { }

private:
    net neuralNet; //Internal neural network object
    std::string NNFilePath; //The path to load a NN file from / save to
    std::string statusString; //What string to display in the header
    dataLoader datLoad; //The data loader object that will (duh) load all of our data for NN
    dataLoader::dataSet loadedSet; //The loaded training data
    std::future<void> future; //If a thread is running a task currently

};

}