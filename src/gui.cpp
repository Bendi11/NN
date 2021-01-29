#include "include/gui.hpp"


std::ofstream logFile; 

namespace GUI
{

SDL_Window* window = NULL;
SDL_GLContext glContext;

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

    window = SDL_CreateWindow("Neural Network", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL); //Attempt to create SDL2 window
    if(window == NULL) //Window creation failed
    {   
        logFile << "Failed to create SDL2 window! Error: " << SDL_GetError() << std::endl; //Get the SDL2 error and exit the program
        exit(-1);
    }

    glContext = SDL_GL_CreateContext(window); //SDL2 OpenGL context creation for window
    SDL_GL_MakeCurrent(window, glContext);

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

    ImGui_ImplSDL2_InitForOpenGL(window, glContext); //Setup window handling system for Dear ImGui
    ImGui_ImplOpenGL3_Init(glsl_version); //Setup OpenGl rendering backend for Dear ImGui
}

void NNGUI::presentCreateWin()
{
    ImGui::Begin("Neural Network Creation"); //Begin drawing to a new Dear ImGui window

    ImGui::Text("Set network file path");
    ImGui::InputText("", &NNFilePath);
    
    if( ImGui::Button("Load Neural Network from Entered Path") ) //User pressed button to load the NN from specified file
    {
        if(future.wait_for(0ms) == std::future_status::ready) //If we aren't running any other threads, go ahead
        {    
            future = std::async(std::launch::async, net::load, neuralNet, NNFilePath); //Spawn a thread to load the NN so that the screen doesn't freeze    
            std::cout << "Thread spawned!" << std::endl;
        }                                    
    }

    ImGui::End();
}

void placeholder() {} //std::future not valid unless you do stupid things

void NNGUI::drawNN()
{
    if(future.valid() == false)
        future = std::async(std::launch::async, placeholder);

    ImGui::BeginMainMenuBar();

    if(future.wait_for(0ms) != std::future_status::ready)
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Another task is running...");

    ImGui::EndMainMenuBar();
    
    this->presentCreateWin();
}

} //End of GUI namespace


