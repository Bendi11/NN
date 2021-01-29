#include "include/net.hpp"
#include <iostream>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <assert.h>

set trainingData;

float layer::LR = 0.005f; //Default 0.005f

int main(int argc, char* argv[])
{
    int w;
    int h;
    int ch; 

    float *imgDat = stbi_loadf("./moon0.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf("./moon1.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf("./moon2.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf("./moon3.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {1.0f} ) );
    stbi_image_free(imgDat);


    imgDat = stbi_loadf("./non0.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf("./non1.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf("./non2.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);
    imgDat = stbi_loadf("./non3.jpg", &w, &h, &ch, STBI_rgb);
    trainingData.push_back( std::make_pair<std::vector<float>, std::vector<float> >(std::vector<float>(imgDat, imgDat + (w * h * ch) ) , {0.0f} ) );
    stbi_image_free(imgDat);



    net n("seeMoon.NN");
    //n.addLayer(w * h * ch, 1);
    //n.addLayer(500);
    //n.addLayer(500);
    //n.addLayer(1);

    for(unsigned int i = 0; i < 10; ++i)
    {
        n.train(trainingData);
        std::cout << "Epoch: " << i << std::endl;
    }
    n.write("seeMoon.NN");

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
    }   
    //n.propFW(trainingData[0].first);
    //std::cout << "Output after being shown Anno 1800 picture: " << n.getOut().outs[0] << std::endl;

    char c;
    std::cin >> c;
}
