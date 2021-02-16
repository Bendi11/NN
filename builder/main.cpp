#include <net.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <assert.h>

#include "include/gui.hpp"

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) //Start SDL2, timer required for Dear ImGui's  delta time feature
    {
        return 1; //Exit the program if SDL2 couldn't start
    }


    const char* glsl_ver = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Enable drawing to frame before frame is displayed

    /*Stuff recommended for Dear ImGui*/
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    SDL_Window* win = SDL_CreateWindow("Neural Network Builder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if(NULL == win) //If SDL window couldn't be created, exit
    {
        return 1; 
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(win); //Make an opengl context for SDL2
    SDL_GL_MakeCurrent(win, glContext);
    SDL_GL_SetSwapInterval(1); //Enable Vsync

    if(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) != 0)
    {
        return 1;
    }

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark(); //TODO: Change default Dear ImGui settings / add menu for theme

    ImGui_ImplSDL2_InitForOpenGL(win, glContext);
    ImGui_ImplOpenGL3_Init(glsl_ver);

    SDL_Event inputEvent; 

    bool run = true; //When user presses 'X' to exit
    while(run)
    {
        while(SDL_PollEvent(&inputEvent))
        {
            ImGui_ImplSDL2_ProcessEvent(&inputEvent); //Let Dear ImGui do stuff with the input
            if(inputEvent.type == SDL_QUIT) run = false; //Quit the application if the user pressed 'X'
        }

        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui_ImplSDL2_NewFrame(win);
        ImGui::NewFrame();

        ImGui::Begin("Test Window");
        ImGui::Text("Test Text");
        if(ImGui::Button("Test Button"))
        {
            ImGui::Text("You Pushed the Test Button!");
        }
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(win); //Do double buffering with frames
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
