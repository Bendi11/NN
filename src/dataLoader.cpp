#include "include/dataLoader.hpp"
#include <stb_image.h>
#include <exception>

bool dataLoader::parseFolder(const std::string path)
{
    std::ifstream maniFile(path + "/manifest.json"); //Open JSON file
    m_path = path + '/';

    if(!maniFile.is_open())
    {
        error = "Failed to open manifest.json at " + path;
        return false;
    }

    maniFile >> manifest; //Parse the manifest file from the ifstream
    maniFile.close();

    if(manifest.empty())
    {
        error = "Failed to parse manifest.json file";
        return false;
    }

    dataNum = manifest["data"].size();
    name = manifest["name"];

    if(manifest["inputType"] == "image") //Image input type
    {
        inputType = dataTypes::image;
    }
    else if(manifest["inputType"] == "number")
    {
        inputType = dataTypes::number;
    }
    else
    {
        error = "inputType: unrecognized data type " + manifest["inputType"].get<std::string>();
        return false;
    }

    if(manifest["outputType"] == "image")
    {
        outputType = dataTypes::image;
    }
    else if(manifest["outputType"] == "number")
    {
        outputType = dataTypes::number;
    }
    else
    {
        error = "outputType: unrecognized data type " + manifest["outputType"].get<std::string>();
        return false;
    }

    return true;
}

const dataLoader::dataPoint dataLoader::loadSingleData(const unsigned int index) //Loading one data input / output
{
    if(index >= dataNum) //Make sure we aren't out of bounds
    {
        error = "Index is greater than number of data points in set";
        throw "Index out of bounds";
    }
    dataPoint ret; //Returned datapoint

    switch(inputType) //Load image, number, text based on input type
    {
        case dataTypes::image:
        {
            std::string fName = m_path; //We heven't changed dirs, so load dir name before file
            fName += manifest["data"][index]["input"]; //Get input file name

            float* imgDat = stbi_loadf(fName.c_str(), &w, &h, &ch, 0); //Use stb_image to load the file
            if(imgDat == nullptr)
            {
                error = "The image file at " + fName + " could not be loaded";
                throw "An image file failed to load";
            }
        
            ret.first = std::vector<float>(imgDat, imgDat + (w * h * ch) ); //Make the image a std::vector 
            stbi_image_free(imgDat); //NO MEMORY LEAKS
        }
        break;

        case dataTypes::number:
        ret.first = manifest["data"][index]["input"].get<std::vector<float>>(); //Just load the array right into std::vector
        break;
    }

    switch(outputType) //Load image, number, text based on input type
    {
        case dataTypes::image:
        {
            std::string fName = m_path; //We heven't changed dirs, so load dir name before file
            fName += manifest["data"][index]["output"]; //Get input file name

            float* imgDat = stbi_loadf(fName.c_str(), &w, &h, &ch, 0); //Use stb_image to load the file
            if(imgDat == nullptr)
            {
                error = "The image file at " + fName + " could not be loaded";
                throw "An image file failed to load";
            }
        
            ret.second = std::vector<float>(imgDat, (imgDat + w * h * ch) ); //Make the image a std::vector 
            stbi_image_free(imgDat); //NO MEMORY LEAKS
        }
        break;

        case dataTypes::number:
        ret.second = manifest["data"][index]["output"].get<std::vector<float>>(); //Just load the array right into std::vector
        break;
    }

    return ret;

}

const dataLoader::dataSet dataLoader::loadAll()
{
    dataSet ret; //Return value

    for(unsigned i = 0; i < dataNum; ++i) //For every data point in the file...
    {
        try
        {
            ret.push_back(loadSingleData(i)); //Add the data to our data set
        }
        catch(...)
        {
            
        }
        
    }

    return ret;
}

