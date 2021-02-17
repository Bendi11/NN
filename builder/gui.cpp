#include "include/gui.hpp"

#include "imgui_stdlib.h" //For input text with std::string

void temp() 
{

}

void NNView::creationWin()
{
    ImGui::Begin("Neural Network Creation");

    static std::string path; //The path to save / load NN from

    ImGui::Text("Path to Save / Load Neural Network From:"); //Prompt user for the path
    ImGui::InputText("", &path);

    if(ImGui::Button("Load Neural Network From Path"))
    {
        if(bgProcess.wait_for(std::chrono::seconds(0)) == std::future_status::ready) //If no background process is running...
        {
            processString = "Loading Neural Network from " + path;
            bgProcess = std::async(std::launch::async, &neural::net::load, &NN, path);
        }
    }
    if(ImGui::Button("Save Neural Network To File"))
    {
        if(bgProcess.wait_for(std::chrono::seconds(0)) == std::future_status::ready) //If no background process is running...
        {
            processString = "Saving Neural Network to " + path;
            bgProcess = std::async(std::launch::async, &neural::net::write, &NN, path); //Begin the async function to save the network to the file
        }
    }

    if(ImGui::BeginMenu("Neural Network Layer Editor")) //Menu to display and edit all neural network layers
    {
        size_t enumerated = 0; //Enumerated layer numbers
        for(auto& layer : NN.layers) //Display a menu item for every layer
        {
            if(enumerated == 0)
            {
                if(ImGui::BeginMenu("Input Layer"))
                {
                    ImGui::Text("Number of Inputs: %d", layer.size);
                    ImGui::EndMenu();
                }
            }
            else if(enumerated == NN.layers.size() - 1)
            {
                if(ImGui::BeginMenu("Output Layer"))
                {
                    ImGui::Text("Input Number: %d", layer.weights[0].size()); //Display size of inputs
                    ImGui::Text("Output Number: %d", layer.size);

                    ImGui::EndMenu();
                }
            }
            else
            {
                if(ImGui::BeginMenu("Hidden Layer"))
                {
                    ImGui::Text("Input Number: %d", layer.weights[0].size()); //Display size of inputs
                    ImGui::Text("Output Number: %d", layer.size);

                    ImGui::EndMenu();
                }
            }

            enumerated++;
        }

        static int layerSize; //Added layer size input
        ImGui::Text("Size of Added Layer: ");
        ImGui::InputInt("", &layerSize); 
        
        if(ImGui::Button("Add Layer With Given Size"))
        {
            if(bgProcess.wait_for(std::chrono::seconds(0)) == std::future_status::ready) //If no background process is running...
            {
                processString = "Adding Neural Network Layer of Size " + std::to_string(layerSize);
                bgProcess = std::async(std::launch::async, &neural::net::addLayer, &NN, layerSize, 0);
            }
        }

        ImGui::EndMenu();
    }

    ImGui::End();
}

void NNView::trainWin()
{
    ImGui::Begin("Neural Network Training");

    ImGui::Text("Neural Network MSE: %d", NN.MSE); //Display mean squared error of network

    ImGui::End();
}

void NNView::dataWin()
{
    ImGui::Begin("Data Loading");

    static std::string manifestPath; //Manifest file path
    ImGui::Text("Manifest.json file path: ");
    ImGui::InputText("", &manifestPath);

    if(ImGui::Button("Load Data Using Manifest.json"))
    {
        
    }

    ImGui::End();
}

bool NNView::display()
{
    if(!bgProcess.valid()) //Do stupid stuff for std::future
        bgProcess = std::async(temp);

    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("Options") )
    {   
        if(ImGui::MenuItem("Quit"))
        {
            return false; //Quit the program
        }
        ImGui::EndMenu();
    }

    ImGui::Text("%s", statusString.c_str());


    if(bgProcess.wait_for(std::chrono::seconds(0) ) == std::future_status::ready) //If the background process is done...
    {

    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Process: %s", processString.c_str());
    }
    ImGui::EndMainMenuBar();

    creationWin();

    return true;
}
