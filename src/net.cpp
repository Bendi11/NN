#include "include/net.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>

#define MAX(x, y) (x > y) ? x : y

float dot(const std::vector<float>& v1, const std::vector<float>& v2) //Function to take dot product of two vectors
{
    assert(v1.size() == v2.size()); //Crash program if the vectors are somehow incorrectly sized

    float out = 0.0f; //The return value of the function
    size_t len = v1.size(); //Get total size of the vectors
    for(unsigned i = 0; i < len; ++i) //Iterate through each element of vectors
    {
        out += v1[i] * v2[i]; //Multiply elements and add to total
    }
    return out;   
}

float sig(float x)
{
    return 1 / (1 + std::exp(-x));

    //return tanhf(x);
}

float sig_d(float x)
{
    return x * (1 - x);

    //return 1 - (x * x);
}

layer::layer(size_t numIn, size_t numOut) : size(numOut) //Function to construct a layer based on given size
{
    outs.resize(size, 100.0f); 
    gradients.resize(size, 100.0f);
    bias.resize(size, 0.5f);
    weights.resize(size);

    for(auto &w : weights) 
    {
        w.resize(numIn);
        for(auto &w2 : w)
        {
            w2 = ( ( (float)rand() / float(RAND_MAX) ) - 0.5f);
        }
    }
}

void layer::propFW(const layer& prev) //Function to calculate outputs based on layer inputs
{
    for(unsigned i = 0; i < size; ++i)
    {
        float cache = 0.0f;
        unsigned int psize = prev.outs.size(); //Get size once to avoid needing to get it every for loop iteration
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

    for(unsigned i = 0; i < size; ++i)
    {
        float delta = -(expected[i] - outs[i]);
       
        gradients[i] = delta * sig_d(outs[i]);

        #ifdef _DEBUG_ 
        std::cout << "Output neuron #" << i << " gradient: " << gradients[i] << " Delta: " << delta << std::endl;
        #endif
    }
}

void layer::calcHiddenGradients(const layer& next) //Function to calculate hidden layer gradients
{
    float sum = 0.0f;
    for(unsigned i = 0; i < size; ++i) //Iterate over all of our neurons
    {
        sum = 0.0;

        for(unsigned n = 0; n < next.size; ++n) //For each neuron this neuron feeds in the next layer
        {
            sum += next.weights[n][i] * next.gradients[n];
        }

        gradients[i] = sum * sig_d(outs[i]);
        #ifdef _DEBUG_ 
        std::cout << "Neuron # " << i << " Hidden gradient: " << gradients[i] << std::endl;
        #endif
    }
}

void layer::updateWeights(const layer& prev) //Function to update weights based on gradients
{
    for(unsigned i = 0; i < size; ++i) //For every neuron in this layer...
    {
        bias[i] -= gradients[i];

        #ifdef _DEBUG_
        std::cout << "Neuron #" << i << " bias: " << bias[i] << std::endl;
        #endif

        for(unsigned j = 0; j < prev.size; ++j) //For every neuron in the previous layer
        {
            float delta = LR * prev.outs[j] * gradients[i]; 
            weights[i][j] -= delta;

            #ifdef _DEBUG_
            //std::cout << "Neuron #" << i << " New weight: " << weights[i][j] << std::endl;
            #endif
        }
    }
}

void net::addLayer(unsigned int numOuts, unsigned int numIn) //Function to add a layer to the network
{
    if(layers.size() == 0)
    {
        layers.push_back(layer(numIn, numOuts));
        numLays = 1;
        return;
    }
    layers.push_back(layer(layers.back().size, numOuts)); //Add the layer to the network
    
    #ifdef _DEBUG_
    std::cout << "Layer added with " << layers.back().size << " inputs and " << numOuts << " outputs" << std::endl;
    #endif


    numLays++;
    return;
}

void net::propFW(const std::vector<float>& in) //Function to propogate data through the network
{
    assert(layers.size() > 1);

    for(unsigned s = 0; s < in.size(); ++s)
    {
        layers[0].outs[s] = in[s];

    }

    for(unsigned i = 1; i < numLays; ++i) //For the next layers in the network...
    {
        layers[i].propFW(layers[i - 1]);
    }

}

layer& net::getOut() //Function returning the outputs of the network
{
    return layers.back();
}

void net::backProp(const std::vector<float>& expected) //Function to backpropogate and train the neural network
{
    #ifdef _DEBUG_
    std::cout << 
    "--------------------BACK PROPOGATION--------------------" << std::endl;
    #endif

    #ifdef _DEBUG_
    std::cout << "OUTPUT LAYER " << std::endl;
    #endif

    layers[layers.size() - 1].calcOutputGradients(expected);

    for(unsigned int i = numLays - 2; i > 0; i--)
    {
        #ifdef _DEBUG_
        std::cout << "LAYER " << i << std::endl;
        #endif
        layers[i].calcHiddenGradients(layers[i + 1]);

        
    }       
    for(unsigned i = numLays - 1; i > 0; --i)
    {
        #ifdef _DEBUG_
        if(i == numLays - 1)
        {
            std::cout << "OUTPUT LAYER: " << std::endl;    
        }
        #endif

        layers[i].updateWeights(layers[i - 1]);

        #ifdef _DEBUG_
        if(i == numLays - 1)
        {
            std::cout << "END OUTPUT LAYER: " << std::endl;
        }
        #endif
    }

    #ifdef _DEBUG_
        std::cout << 
    "--------------------END BACK PROPOGATION--------------------" << std::endl;
    #endif
}

void net::train(const set& in)
{
    size_t s = in.size(); //Get size once 
    for(unsigned i = 0; i < s; ++i) //Iterate through every piece of data given
    {
        propFW(in[i].first); //Propogate the data forwards
        backProp(in[i].second); //Train on given expected outputs
    }
}

void layer::write(std::ofstream& fStream)
{
    size_t s; //Size temporary variable

    s = outs.size();
    fStream.write( (char *)&size, sizeof(size_t) ); 
    fStream.write( (char *)outs.data(), outs.size() ); //Write outputs to file

    s = bias.size();
    fStream.write( (char *)&size, sizeof(size_t) ); 
    fStream.write( (char *)bias.data(), bias.size() ); //Write bias to file
    
    s = gradients.size();
    fStream.write( (char *)&size, sizeof(size_t) ); 
    fStream.write( (char *)gradients.data(), gradients.size() ); //Write gradients to file

    s = weights.size();
    fStream.write((char *)&s, sizeof(size_t)); //Write size of weight matrix to file

    for(unsigned i = 0; i < size; ++i) //For every weight...
    {
        s = weights[i].size();
        fStream.write( (char *)&size, sizeof(size_t) ); 
        fStream.write( (char *)weights.data(), s);
    }

    fStream.write( (char *)&LR, sizeof(float));
    fStream.flush();
}

void net::write(const std::string path)
{
    std::ofstream writer(path, std::ios::binary); //Open the file in binary mode
    for(auto& lay : layers)
    {
        lay.write(writer);
    }
    writer.flush();
    writer.close();
}
