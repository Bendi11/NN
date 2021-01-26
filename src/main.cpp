#include "include/net.hpp"
#include <iostream>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <assert.h>

std::vector<float> ins[4];

std::vector<float> expected[4];

int main(int argc, char* argv[])
{

    expected[0].push_back(1);

    expected[1].push_back(0);

    expected[2].push_back(1);

    expected[3].push_back(0);

    int w;
    int h;
    int ch;

    std::vector<float> inputs[10];


    float *imgDat = stbi_loadf(".\\testMoon.bmp", &w, &h, &ch, STBI_rgb);
    inputs[0] = std::vector<float>(imgDat, imgDat + (w * h * ch) );
    stbi_image_free(imgDat);



    imgDat = stbi_loadf(".\\testNon.bmp", &w, &h, &ch, STBI_rgb);
    std::vector<float> img2Vec(imgDat, imgDat + (w * h * ch));
    inputs[1] = std::vector<float>(imgDat, imgDat + (w * h * ch) );


    stbi_image_free(imgDat);


    net n;
    n.addLayer(w * h * ch, 1);
    n.addLayer(500);
    n.addLayer(500);
    n.addLayer(1);

    std::vector<float> ans[2];
    ans[0].push_back(1.0f);
    ans[1].push_back(0.0f);
    
    for(unsigned int i = 0; i < 100; ++i)
    {

        n.propFW(inputs[0]);
        n.backProp(ans[0]);
        std::cout << "Input: [ " << "Moon picture"<< " ] " << "\tOutput: [ " << n.getOut().outs[0]  << " ]" << "\tExpected: " << ans[0][0] << std::endl;

        n.propFW(inputs[1]);
        n.backProp(ans[1]);
        std::cout << "Input: [ " << "Neutron star picture"<< " ] " << "\tOutput: [ " << n.getOut().outs[0]  << " ]" << "\tExpected: " << ans[1][0] << std::endl;

        std::cout << "Epoch: " << i << std::endl;
    }

    for(unsigned j = 0; j < 2; ++j)
    {
        n.propFW(inputs[0]);
        std::cout << "Input: [ " << "Moon picture"<< " ] " << "\tOutput: [ " << n.getOut().outs[0]  << " ]" << std::endl;

        n.propFW(inputs[1]);
        std::cout << "Input: [ " << "Neutron star picture"<< " ] " << "\tOutput: [ " << n.getOut().outs[0]  << " ]" << std::endl;
    }   

    char c;
    std::cin >> c;
}
