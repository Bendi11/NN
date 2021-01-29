#include "include/gui.hpp"

std::ofstream logFile; 

namespace GUI
{

void initScreen(unsigned int w, unsigned int h)
{
    logFile.open("log.txt"); //Open log file
    if(!logFile.is_open())
    {
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) //Start all SDL2 things
    {
        exit(-1); //Exit the program if SDL couldn't start
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130"; //Set openGL version name
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL version 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Make the window double buffered to avoid drawing to the displayed screen
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Neural Network", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL); //Attempt to create SDL2 window
    if(window == NULL) //Window creation failed
    {   
        logFile << "Failed to create SDL2 window! Error: " << SDL_GetError() << std::endl; //Get the SDL2 error and exit the program
        exit(-1);
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window); //SDL2 OpenGL context creation for window
    SDL_GL_MakeCurrent(window, gl_context);

    bool error = gladLoadGL() == 0; //Attempt to load all OpenGL extensions with glad
    if(error)
    {
        logFile << "Error: failed to load OpenGL extensions from glad Error: " << glad_glGetError() << std::endl;
        exit(-1); //OpenGL doesn't work -> can't display UI -> Exit program
    }

    //Start setting up Dear ImGui
    IMGUI_CHECKVERSION(); //Get Dear ImGui version
    ImGui::CreateContext(); 

    ImGui::StyleColorsDark(); //Enable dark color scheme by default
    ImGui::GetStyle().WindowRounding = 7.5f; //Make windows in Dear ImGui rounded at the corners

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context); //Setup window handling system for Dear ImGui
    ImGui_ImplOpenGL3_Init(glsl_version); //Setup OpenGl rendering backend for Dear ImGui
}

} //End of GUI namespace