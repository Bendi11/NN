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
                if(ImGui::BeginMenu( ("Hidden Layer #" + std::to_string(enumerated) ).c_str() ) )
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

    ImGui::Text("Neural Network MSE: %f", NN.MSE); //Display mean squared error of network
    ImGui::Text("Neural Network Learning Rate: ");
    ImGui::InputFloat("LR: ", &neural::layer::LR, 0.0F, 0.0F, "%f"); //Input the learning rate

    static int trainingIters = 1; //The number of iterations to train the neural network for
    ImGui::Text("Number of Training Iterations: ");
    ImGui::InputInt("", &trainingIters); //Get the number of iterations to train for

    if(dataLoad.valid) //There is data loaded, we can train
    {
        ImGui::Text("Training Data Loaded!");
        if(ImGui::Button("Train Neural Network Over Data"))
        {
            if(bgProcess.wait_for(std::chrono::seconds(0)) == std::future_status::ready) //If no background process is running...
            {
                processString = "Training over data for " + std::to_string(trainingIters) + " epochs";
                bgProcess = std::async(std::launch::async, &neural::net::train, &NN, trainingData, trainingIters); //Spawn an async process to train
            }
        }
    }
    
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
        try
        {
            processString = "Loading Training Data From " + manifestPath;
            if ( !dataLoad.parseFolder(manifestPath) )
            {
                statusString = dataLoad.error;
                return;
            }
            trainingData = dataLoad.loadAll();
        }
        catch(const std::string& e)
        {
            statusString = e; //Set the error
            exit(-1);
        }
    }

    ImGui::Spacing();

    if(dataLoad.valid) //If valid data is loaded...
    {
        ImGui::Text("Data Points: %d", dataLoad.dataNum); //Display size of dataset
        ImGui::Text("Input Data Type: %s", DATATYPESTR(dataLoad.inputType) );
        ImGui::Text("Output Data Type: %s", DATATYPESTR(dataLoad.outputType) );

        size_t enumerate = 0;
        for(auto& input : dataLoad.manifest["data"]) //For every data input...
        {
            if(ImGui::BeginMenu( ("Input: " + input["input"].get<std::string>()).c_str() ) ) //Begin a menu for this input
            {
                ImGui::Text("Neural Network Outputs: ");
                for(const float& out : NN.getOut().outs)
                {
                    ImGui::Text("%f", out);
                }
                ImGui::Spacing();

                if(ImGui::Button("See Neural Network Output For This Input"))
                {
                    if(bgProcess.wait_for(std::chrono::seconds(0)) == std::future_status::ready) //If no background process is running...
                    {
                        processString = "Propogating Data Forwards";
                        bgProcess = std::async(std::launch::async, &neural::net::propFW, &NN, trainingData[enumerate].first); //Spawn an async process to train
                    }
                }

                ImGui::EndMenu();
            }
            enumerate++;
        }
    }

    ImGui::End();
}

NNView::NNView()
{
    if(!bgProcess.valid()) //Do stupid stuff for std::future
        bgProcess = std::async(temp);
}

bool NNView::display()
{
    creationWin();
    dataWin();
    trainWin();

    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("Options") )
    {   
        if(ImGui::MenuItem("Quit"))
        {
            return false; //Quit the program
        }
        ImGui::EndMenu();
    }



    if(bgProcess.wait_for(std::chrono::seconds(0) ) == std::future_status::ready) //If the background process is done...
    {

    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Process: %s", processString.c_str());
    }
    
    ImGui::Text("%s", statusString.c_str());

    ImGui::EndMainMenuBar();

    return true;
}
