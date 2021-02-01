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

#include "stb_image.h"

namespace GUI
{

struct image //Convenience class for displaying images in Dear ImGui windows
{
    GLuint txID; //Texture ID in OpenGL
    int width; //Filled when image is loaded
    int height;
    int channels;

    image::image(std::string path)
    {
        unsigned char* imgData = stbi_load(path.c_str(), &width, &height, &channels, 4);
        logFile.write((char *)imgData, width * height * channels);
        if(imgData == NULL) exit(-1);
        glGenTextures(1, &txID);
        glBindTexture(GL_TEXTURE_2D, txID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
        stbi_image_free(imgData);
    }

    image(const std::vector<float> imgDat, int w, int h)
    {
        glGenTextures(1, &txID);
        glBindTexture(GL_TEXTURE_2D, txID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, (void *)imgDat.data());
    }

    image() 
    {
        width = 0;
        height = 0;
        txID = 0;
    }

    ~image() //Destructor to clean up resources
    {
        glDeleteTextures(1, &txID);
    }
};

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
    NNGUI() {t = image("data/non0.jpg");}

private:
    net neuralNet; //Internal neural network object
    std::string NNFilePath; //The path to load a NN file from / save to
    std::string statusString; //What string to display in the header
    dataLoader datLoad; //The data loader object that will (duh) load all of our data for NN
    dataLoader::dataSet loadedSet; //The loaded training data
    std::future<void> future; //If a thread is running a task currently

    image t; //USELESS FUCKING DEBUG SHIT FUCK FUCK FUCKFUCKFUCK

};

}