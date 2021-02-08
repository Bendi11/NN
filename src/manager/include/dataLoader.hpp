/*
NOTE ON MANIFEST.JSON
Manifest must have a header defining: 
name: string
inputType: string 
outputType: string
data: array of structs containing input: string / number array and output: string/number array

INPUT/OUTPUT TYPES:
image: data is an array of strings which are file paths to image files
number: data is an array of arrays of raw numbers (floats) 
numberFile: data is an array of string paths to text files containing comma separated numbers
*/

#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

using json = nlohmann::json; //Json instead of long name for namespace

class dataLoader //Class for loading different types of training data from a folder with manifest.json
{
public:
    typedef std::vector<std::pair<std::vector<float>, std::vector<float> > > dataSet; //Ease of use type definitions with key value pairs
    typedef std::pair<std::vector<float>, std::vector<float>> dataPoint; //One data input/output

    bool parseFolder(const std::string path); //Function to set folder to load and parse manifest.json
    const dataPoint loadSingleData(const unsigned int index); //Function to load a single pair of expected and data, for large files not all being loaded to RAM
    const dataSet loadAll();        //Function to load every item from a folder 

    size_t dataNum = 0; //Number of data sets

    enum dataTypes //What kind of data the set is
    {
        image,
        number,
        textFile
    };
    std::string error;     //The error string
    json manifest;         //The manifest.json object in C++

    int w, h, ch; //All image sizes
    
    dataTypes inputType;
    dataTypes outputType;

private:
    std::string m_path;    //Path to folder that data is being loaded from
    std::string name;

};
