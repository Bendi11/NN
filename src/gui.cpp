#include "include/net.hpp"
#include <iostream>
#include <time.h>
#include "include/gui.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <assert.h>


bool guiApp::OnInit()
{
    nnFrame* f = new nnFrame();
    f->Show(true);
    return true;
}

bool guiApp::OnClose()
{
    return true;
}

nnFrame::nnFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    int w;
    int h;
    int ch; 

    set trainingData;
    net network;

    float *imgDat = stbi_loadf(".\\moon0.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf(".\\moon1.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf(".\\moon2.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);

    imgDat = stbi_loadf(".\\moon3.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);



    imgDat = stbi_loadf(".\\non0.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf(".\\non1.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf(".\\non2.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf(".\\non3.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);

    network.addLayer(w * h * ch, 1); 
    network.addLayer(500);
    network.addLayer(500);
    network.addLayer(1);


    wxJPEGHandler* jpgHandle = new wxJPEGHandler(); //Object used to save and load JPEG images
    wxImage::AddHandler(jpgHandle); //Start JPG loading

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL); //Sizer object to auto place GUI widgets

    wxPanel* pan = new wxPanel(this); //Panel containing all widgets
    
    labelText = new wxStaticText(pan, wxID_ANY, "Picture: Non-Moon");
    outText = new wxStaticText(pan, wxID_ANY, "Moon: 0");

    wxButton* trainButton = new wxButton(pan, ID_TRAIN, "Train neural network");
    wxButton* feedButton = new wxButton(pan, ID_FEED, "Predict data");
    wxButton* saveButton = new wxButton(pan, ID_SAVE, "Save neural network to file");
    wxButton* loadButton = new wxButton(pan, ID_LOAD, "Load neural network from file");
    
    sizer->Add(labelText, 0, wxALIGN_CENTER);
    sizer->Add(outText, 0, wxALIGN_CENTER);

    sizer->AddSpacer(30);

    sizer->Add(trainButton, 0, wxALIGN_CENTER);
    sizer->Add(feedButton, 0, wxALIGN_CENTER);
    sizer->AddSpacer(30);
    sizer->Add(saveButton, 0, wxALIGN_CENTER);
    sizer->Add(loadButton, 0, wxALIGN_CENTER);

    pan->Show(true);
    pan->SetSizer(sizer);

    Bind(wxEVT_BUTTON, [&](wxEvent& e) 
    {
        //network.propFW(trainingData[0].first);
        //network.backProp(trainingData[0].second);
        //labelText->SetLabelText((trainingData[i].second[0] == 0.0f) ? "Not a Moon picture" : "Moon picture");
        float i = network.getOut().outs[0];
        //outText->SetLabelText("Moon: " + std::to_string(1) );
    }, ID_TRAIN);

}
