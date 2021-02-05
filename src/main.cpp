#include "include/net.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/cli.hpp"
#include <assert.h>

neural::set trainingData;


int main(int argc, char* argv[])
{
    NNView netScreen;
    while(netScreen.bRun)
    {
        if( !netScreen.refresh() ) //Refresh returns false on error
        {
            break;
        }
    }
    return 0;
}
