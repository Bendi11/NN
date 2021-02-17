#include "net.hpp"
#include <assert.h>
#include <string>

namespace neural
{

#define MAX(x, y) (x > y) ? x : y
float layer::LR = 0.005f; //Default 0.005f


//#define _DEBUG_

float sig(float x) //Default activation function for neural network
{
    return 1 / (1 + std::exp(-x));

    //return tanhf(x);
}

float sig_d(float x) //Derviative of sigmoid activation used to train network
{
    return x * (1 - x);

    //return 1 - (x * x);
}

layer::layer(size_t numIn, size_t numOut) : size(numOut) //Function to construct a layer based on given size
{
    outs.resize(size, 1.0f); 
    gradients.resize(size, 1.0f);
    bias.resize(size, 0.5f);
    weights.resize(size);

    for(auto &w : weights) //For every neuron weight vector...
    {
        w.resize(numIn); //Resize the vector to accomodate all inputs
        for(auto &w2 : w)
        {
            w2 = ( ( (float)rand() / float(RAND_MAX) ) - 0.5f); //Set every weight to a random value between 0.5 and 1
        }
    }
}

void layer::propFW(const layer& prev) //Function to calculate outputs based on layer inputs
{
    for(unsigned i = 0; i < size; ++i) //For every neuron in this layer...
    {
        float cache = 0.0f; //Cache used to sum up product of weights and inputs before activation function
        size_t psize = prev.outs.size(); //Get size once to avoid needing to get it every for loop iteration
        for(unsigned j = 0; j < psize; ++j) //For every input...
        {
            cache += weights[i][j] * prev.outs[j]; //Do dot product
        }
        cache += bias[i]; //Add bias to the cache value
        outs[i] = sig(cache); //Apply activation function
    }
}

void layer::calcOutputGradients(const std::vector<float>& expected) //Function to calculate neuron gradients as the output layer
{
    assert(expected.size() == outs.size()); //Ensure that the sizes of both vectors match

    for(unsigned i = 0; i < size; ++i) //For every output neuron...
    {
        float delta = -(expected[i] - outs[i]); //Get error of the output neurons using expected data
       
        gradients[i] = delta * sig_d(outs[i]);

        #ifdef _DEBUG_ 
        logFile << "Output neuron #" << i << " gradient: " << gradients[i] << " Delta: " << delta << std::endl;
        #endif
    }
}

void layer::calcHiddenGradients(const layer& next) //Function to calculate hidden layer gradients
{
    float sum = 0.0f;
    for(unsigned i = 0; i < size; ++i) //Iterate over all of our neurons
    {
        sum = 0.0; //Used to do dot product of next gradient and next weights

        for(unsigned n = 0; n < next.size; ++n) //For each neuron this neuron feeds in the next layer
        {
            sum += next.weights[n][i] * next.gradients[n];
        }

        gradients[i] = sum * sig_d(outs[i]);
    }
}

void layer::updateWeights(const layer& prev) //Function to update weights based on gradients
{
    for(unsigned i = 0; i < size; ++i) //For every neuron in this layer...
    {
        bias[i] -= gradients[i]; //Adjust bias based on gradient

        for(unsigned j = 0; j < prev.size; ++j) //For every neuron in the previous layer
        {
            float delta = LR * prev.outs[j] * gradients[i]; //TODO: add momentum if specified by user
            weights[i][j] -= delta;
        }
    }
}

void net::addLayer(unsigned int numOuts, unsigned int numIn) //Function to add a layer to the network
{
    if(layers.size() == 0) //This is the first layer added
    {
        layers.push_back(layer(numIn, numOuts)); //Can't call layers.back() if it is empty, so I need a special case
        numLays = 1;
        return;
    }
    layers.push_back(layer(layers.back().size, numOuts)); //Add the layer to the network
    
    #ifdef _DEBUG_
    logFile << "Layer added with " << layers.back().size << " inputs and " << numOuts << " outputs" << std::endl;
    #endif


    numLays++;
    return;
}

void net::propFW(const std::vector<float>& in) //Function to propogate data through the network
{
    assert(layers.size() > 1); //Take out for debug?

    for(unsigned s = 0; s < in.size(); ++s) //For every neuron in input layer, assign value of input to output of neuron. memcpy might be faster?
    {
        layers[0].outs[s] = in[s];

    }

    for(unsigned i = 1; i < numLays; ++i) //For the next layers in the network...
    {
        layers[i].propFW(layers[i - 1]); //Propogate FW based on previous layer's output
    }

}

layer& net::getOut() //Function returning the outputs of the network
{
    return layers.back();
}

void net::backProp(const std::vector<float>& expected) //Function to backpropogate and train the neural network
{
    #ifdef _DEBUG_
    logFile << 
    "--------------------BACK PROPOGATION--------------------" << std::endl;
    #endif

    #ifdef _DEBUG_
    logFile << "OUTPUT LAYER " << std::endl;
    #endif

    layers[layers.size() - 1].calcOutputGradients(expected); //Get the output gradients first

    for(size_t i = numLays - 2; i > 0; i--) //Start from first hidden layer from output layer work to the input layer
    {
        #ifdef _DEBUG_
        logFile << "LAYER " << i << std::endl;
        #endif
        layers[i].calcHiddenGradients(layers[i + 1]); //Get the gradient of this layer

        
    }       
    for(size_t i = numLays - 1; i > 0; --i) //From output layer to input, adjust weights based on calculated gradients
    {
        layers[i].updateWeights(layers[i - 1]);
    }

    #ifdef _DEBUG_
        logFile << 
    "--------------------END BACK PROPOGATION--------------------" << std::endl;
    #endif
}

void net::train(const set& in, int iterations) //Convenience function to train over dataset and get MSE
{
    size_t s = in.size(); //Get size once 
    MSE = 0.0f; //Reset MSE before training

    for(int j = 0; j < iterations; ++j) //Iterate as many times as given (default 1)
        for(unsigned i = 0; i < s; ++i) //Iterate through every piece of data given
        {
            propFW(in[i].first); //Propogate the data forwards
            backProp(in[i].second); //Train on given expected outputs
            
            for(unsigned k = 0; k < layers.back().size; ++k)
            {
                MSE += (float)pow(in[i].second[k] - layers.back().outs[k], 2); //Mean squared error
            }
        }

    MSE /= s * iterations; 
}

void layer::write(FILE* file) //Function to write layer to file using C style FILE for writing, might make x86 NN incompatible with x86_64 PC
{
    fwrite((char* )&LR, sizeof(float), 1, file); //Write neural network learning rate
    fwrite((char *)&size, sizeof(size_t), 1, file); //Write neural network size

    for(auto& out : outs)
    {
        fwrite((char *)&out, sizeof(float), 1, file);
    }

    for(auto& b : bias)
    {
        fwrite((char *)&b, sizeof(float), 1, file);
    }

    for(auto& grad : gradients)
    {
        fwrite((char *)&grad, sizeof(float), 1, file);
    }

    size_t inSize = weights[0].size();
    fwrite((char *)&inSize, sizeof(size_t), 1, file);

    for(unsigned i = 0; i < size; ++i) //For every weight...
    {
        for(auto& w : weights[i])
        {
            fwrite((char *)&w, sizeof(float), 1, file);
        }
    }
}

void net::write(std::string path) //Function to write entire NN to a file
{
    FILE* writer = fopen(path.c_str(), "wb");
    if(writer == NULL) //File failed to open
    {
        #ifdef _DEBUG_
        logFile << "Error: Failed to open NN file at " << path << std::endl;
        #endif
        return; //Don't exit program, but exit the function to write to file
    }
    #ifdef _DEBUG_
    logFile << "Opened NN file at " << path << std::endl;
    #endif
    
    unsigned i = 0; //Only used to give progress bar indication
    for(auto& lay : layers)
    {
        #ifdef _DEBUG_
        logFile << "Writing layer #" << i << " to file..."<< std::endl;
        #endif
        lay.write(writer);
        if(i != numLays - 1) fwrite("B", sizeof(char), 1, writer);     //Write layer separator character   
        i++;

        //writer.flush();
        #ifdef _DEBUG_
        logFile << "Wrote layer #" << i << " to file!"<< std::endl;
        #endif

    }
    fwrite("E", sizeof(char), 1, writer); //Write E to signify there are no more layers left
    fclose(writer);
}

layer::layer(FILE* file) //Function to reconstruct a layer from a FILE object
{
    size_t s; //Size temporary variable
    fread((char *)&LR, sizeof(float), 1, file);
    fread((char*)&size, sizeof(size_t), 1, file);

    outs.reserve(size); //Allocate enough memory to hold entire output vector
    for(unsigned i = 0; i < size; ++i)
    {
        float f;
        fread((char *)&f, sizeof(float), 1, file);
        outs.push_back(f); 
    }

    bias.reserve(size);
    for(unsigned i = 0; i < size; ++i)
    {
        float f;
        fread((char *)&f, sizeof(float), 1, file);
        bias.push_back(f);
    }

    gradients.reserve(size); 
    for(unsigned i = 0; i < size; ++i)
    {
        float f;
        fread((char *)&LR, sizeof(float), 1, file);
        gradients.push_back(f);
    }

    fread((char *)&s, sizeof(size_t), 1, file);
    weights.resize(size);

    for(unsigned i = 0; i < size; ++i) //For every weight...
    {
        weights[i].reserve(s);
        for(unsigned j = 0; j < s; ++j) //For every input...
        {
            float f;
            fread((char *)&f, sizeof(float), 1, file);
            weights[i].push_back(f);
        }
    }

    #ifdef _DEBUG_
    logFile << "Read layer into file!" << std::endl;
    logFile << "Output size: " << outs.size() << std::endl;
    logFile << "Bias size: " << bias.size() << std::endl;
    logFile << "Gradients size: " << gradients.size() << std::endl;
    logFile << "Input number: " << weights[0].size() << std::endl;
    #endif
}

net::net(std::string fName) //Constructor to load a NN from one file
{
    FILE* reader = fopen(fName.c_str(), "rb");
    if(reader == NULL)
    {
        #ifdef _DEBUG_
        logFile << "Failed to open NN file from " << fName << std::endl;
        #endif
        exit(-1);
    }
    size_t i = 0; //Count of layers loaded

    while(!feof(reader))
    {
        i++;

        #ifdef _DEBUG_
        logFile << "Reading layer from file: " << fName << std::endl;
        #endif

        if(feof(reader)) //Just in case we hit an unexpected EOF
        {
            break;
        }

        layers.push_back(layer(reader)); //Read the layer data from the file and add it to our layers
        #ifdef _DEBUG_
        logFile << "Layer #" << i << " loaded from file " << fName << std::endl;
        #endif

        char c;
        fread((char *)&c, sizeof(char), 1, reader);
        if(c == 'E') //End of file character
        {
            break;
        }

    }
    numLays = layers.size();
    fclose(reader);
}

void net::load(std::string fName) //Same as constructor, but to explicitly call without move semantics
{
    FILE* reader = fopen(fName.c_str(), "rb");
    if(reader == NULL)
    {
        #ifdef _DEBUG_
        logFile << "Failed to open NN file from " << fName << std::endl;
        #endif
        throw std::runtime_error("Failed to open neural network file!");
    }
    size_t i = 0; //Count of layers loaded

    while(!feof(reader))
    {
        i++;

        #ifdef _DEBUG_
        logFile << "Reading layer from file: " << fName << std::endl;
        #endif

        if(feof(reader)) //Just in case we hit an unexpected EOF
        {
            break;
        }

        layers.push_back(layer(reader)); //Read the layer data from the file and add it to our layers
        #ifdef _DEBUG_
        logFile << "Layer #" << i << " loaded from file " << fName << std::endl;
        #endif

        char c;
        fread((char *)&c, sizeof(char), 1, reader);
        if(c == 'E') //End of file character
        {
            break;
        }

    }
    numLays = layers.size();
    fclose(reader);
}

} //Namespace neural