#pragma once

//All UI library includes
#include <imgui.h>
#include <SDL.h>
#include <glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include "net.hpp"


namespace GUI
{

extern SDL_Window* window;
extern SDL_GLContext glContext;
void initScreen(unsigned int w, unsigned int h);

class NNGUI //Neural network GUI class
{
public:
    void presentCreateWin(); //Function to draw the Dear ImGui window for creating a NN 

private:
    net neuralNet; //Internal neural network object
    std::string NNFilePath; //The path to load a NN file from / save to
};

}