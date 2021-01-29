#include "include/net.hpp"
#include <iostream>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <assert.h>

#include "include/gui.hpp"

set trainingData;

float layer::LR = 0.00005f; //Default 0.005f

int main(int argc, char* argv[])
{
    GUI::initScreen(1280, 720);
    SDL_Event e;
    ImGuiIO io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

    GUI::NNGUI nnGUI;

    bool bRun = true; //If we should keep running the main loop
    while(bRun)
    {
        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e); //Process Dear ImGui SDL2 events
            if (e.type == SDL_QUIT)
                bRun = false;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(GUI::window))
                bRun = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(GUI::window);
        ImGui::NewFrame();

        //--------------------DRAWING DEAR IMGUI WINDOWS-------------------//

        nnGUI.drawNN();

        //--------------------END OF DRAWING DEAR IMGUI WINDOWS------------//



        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        unsigned int t = ImGui::GetIO().DeltaTime; //Get time to render the last frame
        if(t < (1000 / 60)) //Cap FPS to 60
        {
            SDL_Delay( (1000 / 60) - t);
        }

        SDL_GL_SwapWindow(GUI::window);
    }

    int w;
    int h;
    int ch; 

    float *imgDat = stbi_loadf(".\\anno.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);
    //imgDat = stbi_loadf(".\\moon1.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    //stbi_image_free(imgDat);
    //imgDat = stbi_loadf(".\\moon2.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    //stbi_image_free(imgDat);
    //imgDat = stbi_loadf(".\\moon3.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    //stbi_image_free(imgDat);


    //imgDat = stbi_loadf(".\\non0.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    //stbi_image_free(imgDat);
    //imgDat = stbi_loadf(".\\non1.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    //stbi_image_free(imgDat);
    //imgDat = stbi_loadf(".\\non2.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    //stbi_image_free(imgDat);
    //imgDat = stbi_loadf(".\\non3.jpg", &w, &h, &ch, STBI_rgb);
    //trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    //stbi_image_free(imgDat);



    //net n("seeMoon.NN");
    //n.addLayer(w * h * ch, 1);
    //n.addLayer(500);
    //n.addLayer(500);
    //n.addLayer(1);

    /*for(unsigned int i = 0; i < 20; ++i)
    {
        //n.train(trainingData);
        //std::cout << "Epoch: " << i << std::endl;
    }
    //n.write("seeMoon.NN");

    for(unsigned j = 0; j < trainingData.size(); ++j)
    {
        n.propFW(trainingData[j].first);
        std::cout << "Input: [ "; 
        if(trainingData[j].second[0] == 1.0f)
        {
            std::cout << "Moon Picture";
        }
        else
        {
            std::cout << "Non-Moon Picture";
        }
        std::cout << " ] " << "\tOutput: [ " << n.getOut().outs[0]  << " ]" << std::endl;
    }   */
    /*n.propFW(trainingData[0].first);
    std::cout << "Output after being shown Anno 1800 picture: " << n.getOut().outs[0] << std::endl;

    char c;
    std::cin >> c;*/

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(GUI::glContext);
    SDL_DestroyWindow(GUI::window);
    SDL_Quit();

    return 0;
}
