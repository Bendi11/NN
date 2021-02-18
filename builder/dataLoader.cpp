#include "include/dataLoader.hpp"
#include <stb_image.h>
#include <exception>

json REQUIRE(const json& jsonObj, const char* name) //Function to throw errors if a JSON object is not found and return a json object if it is
{
    if(jsonObj[name].empty()) //The JSON value doesn't exist
    {
        throw std::string("JSON value " + std::string(name) + " not found!"); //Throw the error message
    }

    return jsonObj[name];
}

json REQUIRE(const json& jsonObj, const size_t idx) //Function to throw errors if a JSON object is not found and return a json object if it is
{
    if(jsonObj[idx].empty()) //The JSON value doesn't exist
    {
        throw std::string("JSON value " + std::to_string(idx) + " not found!"); //Throw the error message
    }

    return jsonObj[idx];
}


bool dataLoader::parseFolder(const std::string path)
{
    std::ifstream maniFile(path + "/manifest.json"); //Open JSON file
    m_path = path + '/';

    if(!maniFile.is_open()) //If the file failed to open
    {
        error = "Failed to open manifest.json at " + path;
        return false;
    }

    maniFile >> manifest; //Parse the manifest file from the ifstream
    maniFile.close(); //Close the manifest file

    try
    {
        dataNum = REQUIRE(manifest, "data").size(); //Get the size of the data array
        name = REQUIRE(manifest, "name"); //Get the name of the dataset

        std::string inType = REQUIRE(manifest, "inputType"); //Get the input type

        if(inType == "image") //Image input type
        {
            inputType = dataTypes::image;
        }
        else if(inType == "number")
        {
            inputType = dataTypes::number;
        }
        else
        {
            error = "inputType: unrecognized data type " + inType;
            return false;
        }

        std::string outType = REQUIRE(manifest, "outputType"); //Get the output type
        if(outType == "image")
        {
            outputType = dataTypes::image;
        }
        else if(outType == "number")
        {
            outputType = dataTypes::number;
        }
        else
        {
            error = "outputType: unrecognized data type " + outType;
            return false;
        }
    }
    catch(const std::string& e) //Catch any error thrown by REQUIRE function
    {
        error = e;
        return false;
    }
    
    valid = true;
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

    try
    {
        
        switch(inputType) //Load image, number, text based on input type
        {
            case dataTypes::image:
            {
                std::string fName = m_path; //We heven't changed dirs, so load dir name before file
                
                fName += REQUIRE(
                    REQUIRE(
                        REQUIRE(manifest, 
                        "data"), 
                        index),
                        "input"
                    ); //Get input file name

                float* imgDat = stbi_loadf(fName.c_str(), &w, &h, &ch, 0); //Use stb_image to load the file
                if(imgDat == nullptr)
                {
                    error = "The image file at " + fName + " could not be loaded";
                    throw std::string("An image file failed to load from path" + fName);
                }
            
                ret.first = std::vector<float>(imgDat, imgDat + (w * h * ch) ); //Make the image a std::vector 
                stbi_image_free(imgDat); //NO MEMORY LEAKS
            }
            break;

            case dataTypes::number:
            ret.first = REQUIRE(
                    REQUIRE(
                        REQUIRE(manifest, 
                        "data"), 
                        index),
                        "input"
                    ).get<std::vector<float>>(); //Just load the array right into std::vector
            break;
        }

        switch(outputType) //Load image, number, text based on input type
        {
            case dataTypes::image:
            {
                std::string fName = m_path; //We heven't changed dirs, so load dir name before file
                fName += REQUIRE(
                    REQUIRE(
                        REQUIRE(manifest, 
                        "data"), 
                        index),
                        "output"
                    ); //Get input file name

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
            ret.second = REQUIRE(
                    REQUIRE(
                        REQUIRE(manifest, 
                        "data"), 
                        index),
                        "output"
                    ).get<std::vector<float>>(); //Just load the array right into std::vector
            break;
        }
    }
    catch(const std::string& e) //Catch any errors the REQUIRE blocks may have thrown and propogate them
    {
        throw e;
    }

    return ret;

}

const dataLoader::dataSet dataLoader::loadAll() //Function to load all data from manifest.JSON
{
    dataSet ret; //Return value

    for(unsigned i = 0; i < dataNum; ++i) //For every data point in the file...
    {
        try
        {
            ret.push_back(loadSingleData(i)); //Add the data to our data set
        }
        catch(const std::string& e)
        {
            error = e;
            throw e;
        }
        
    }

    return ret;
}

