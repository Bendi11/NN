#pragma once

#include <vector>
#include <iostream>
#include <random>
#include <fstream>

extern std::ofstream logFile; //Yucky global variable but I need it to have a consistent log files

typedef std::vector<std::pair<std::vector<float>, std::vector<float> > > set; //Ease of use type definitions with key value pairs

struct layer //General layer class containing neurons and weights
{
public:
    layer(size_t numIn, size_t numOut); //Constructor to make a new neural network with specified dimensions
    layer(std::ifstream& file); //Constructor for loading a layer from a file
    void propFW(const layer& prev); //Function to calculate the outputs of layer based on input
    void calcOutputGradients(const std::vector<float>& expected); //Function to calculate neuron gradients if this is the output layer
    void calcHiddenGradients(const layer& next); //Function to calculate gradients of a hidden network layer
    void updateWeights(const layer& next); //Function to update weights based on calculated gradients
    void write(std::ofstream& fStream); //Function to write a network to a file

    std::vector<float> outs;
    std::vector<float> bias;
    std::vector<float> gradients;
    std::vector< std::vector<float> > weights;
    size_t size; //The number of numbers in the layer
    static float LR; //Learning rate of the entire network

};

class net
{
public:
    net(std::string fNames); //Constructor to load a NN from a file
    net() {} //Default constructor
    void addLayer(unsigned int numOuts, unsigned int numIn = 0); //Function to add a layer to the network
    void train(const set& in); //Convenience function to load a training set and train on it
    layer& getOut(); //Function returning network outputs
    void write(std::string path); //Function to write a neural network to a file

    void propFW(const std::vector<float>& in); //Function to propogate input data through the network
    void propFW(float* in, size_t size); //Function to load a float array to the network
    void backProp(const std::vector<float>& expected); //Function to update weights based on expected outputs

    size_t numLays; //The number of layers in network

private:
    std::vector<layer> layers; //The layout of the network
    std::string name; //Name of neural network

};
