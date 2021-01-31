#include "include/net.hpp"
#include <assert.h>
#include <string>

#define MAX(x, y) (x > y) ? x : y
float layer::LR = 0.005f; //Default 0.005f


//#define _DEBUG_

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
    outs.resize(size, 1.0f); 
    gradients.resize(size, 1.0f);
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

    for(unsigned i = 0; i < size; ++i)
    {
        float delta = -(expected[i] - outs[i]);
       
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
        sum = 0.0;

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
        bias[i] -= gradients[i];

        for(unsigned j = 0; j < prev.size; ++j) //For every neuron in the previous layer
        {
            float delta = LR * prev.outs[j] * gradients[i]; 
            weights[i][j] -= delta;
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
    logFile << "Layer added with " << layers.back().size << " inputs and " << numOuts << " outputs" << std::endl;
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
    logFile << 
    "--------------------BACK PROPOGATION--------------------" << std::endl;
    #endif

    #ifdef _DEBUG_
    logFile << "OUTPUT LAYER " << std::endl;
    #endif

    layers[layers.size() - 1].calcOutputGradients(expected);

    for(size_t i = numLays - 2; i > 0; i--)
    {
        #ifdef _DEBUG_
        logFile << "LAYER " << i << std::endl;
        #endif
        layers[i].calcHiddenGradients(layers[i + 1]);

        
    }       
    for(size_t i = numLays - 1; i > 0; --i)
    {
        #ifdef _DEBUG_
        if(i == numLays - 1)
        {
            logFile << "OUTPUT LAYER: " << std::endl;    
        }
        #endif

        layers[i].updateWeights(layers[i - 1]);

        #ifdef _DEBUG_
        if(i == numLays - 1)
        {
            logFile << "END OUTPUT LAYER: " << std::endl;
        }
        #endif
    }

    #ifdef _DEBUG_
        logFile << 
    "--------------------END BACK PROPOGATION--------------------" << std::endl;
    #endif
}

void net::train(const set& in, int iterations)
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
                MSE += pow(in[i].second[k] - layers.back().outs[k], 2);
            }
        }

    MSE /= s * iterations; 
}

void layer::write(std::ofstream& fStream)
{
    fStream << LR << ' ' << size << ' ';; //Write metadata about layer

    for(auto& out : outs)
    {
        fStream << out << ' ';
    }

    for(auto& b : bias)
    {
        fStream << b << ' ';
    }

    for(auto& grad : gradients)
    {
        fStream << grad << ' ';
    }

    fStream << weights[0].size() << ' '; //Write total size of weights matrix and size of each vector inside each vector

    for(unsigned i = 0; i < size; ++i) //For every weight...
    {
        for(auto& w : weights[i])
        {
            fStream << w << ' ';
        }
    }
}

void net::write(std::string path)
{
    std::ofstream writer; //Open the file in binary mode
    writer.open(path);
    if(!writer.is_open()) //File failed to open
    {
        logFile << "Error: Failed to open NN file at " << path << std::endl;
        return; //Don't exit program, but exit the function to write to file
    }
    
    unsigned i = 0; //Only used to give progress bar indication
    for(auto& lay : layers)
    {
        lay.write(writer);
        if(i != numLays - 1) writer << " B ";     //Write layer separator character   
        i++;

    }
    writer << " E"; //Write E to signify there are no more layers left
    writer.close();
}

layer::layer(std::ifstream& file)
{
    size_t s; //Size temporary variable
    file >> LR;
    file >> size;

    outs.reserve(size); //Allocate enough memory to hold entire output vector
    for(unsigned i = 0; i < size; ++i)
    {
        float f;
        file >> f;
        outs.push_back(f); 
    }

    bias.reserve(size);
    for(unsigned i = 0; i < size; ++i)
    {
        float f;
        file >> f;
        bias.push_back(f);
    }

    gradients.reserve(size); 
    for(unsigned i = 0; i < size; ++i)
    {
        float f;
        file >> f;
        gradients.push_back(f);
    }

    file >> s; //Get number of inputs to network
    weights.resize(size);

    for(unsigned i = 0; i < size; ++i) //For every weight...
    {
        for(unsigned j = 0; j < s; ++j) //For every input...
        {
            float f;
            file >> f;
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
    std::ifstream reader(fName); //Reader file object for reading all neural network layers
    if(!reader.is_open())
    {
        logFile << "Failed to open NN file from " << fName << std::endl;
        exit(-1);
    }
    size_t i = 0; //Count of layers loaded

    while(!reader.eof() )
    {
        i++;

        #ifdef _DEBUG_
        logFile << "Reading layer from file: " << fNames + std::to_string(i) + ".NN" << std::endl;
        #endif

        if(!reader.is_open())
        {
            break;
        }
        layers.push_back(layer(reader)); //Read the layer data from the file and add it to our layers
        logFile << "Layer #" << i << " loaded from file " << fName << std::endl;
        char c;
        reader >> c;
        if(c == 'E') //End of file
        {
            break;
        }

    }
    numLays = layers.size();
    reader.close();
}

void net::load(net* in, std::string fName) //Same as constructor, but taking argument instead
{
    std::ifstream reader(fName); //Reader file object for reading all neural network layers
    if(!reader.is_open())
    {
        logFile << "Failed to open NN file from " << fName << std::endl;
        return;
    }
    size_t i = 0; //Count of layers loaded

    while(!reader.eof() )
    {
        i++;

        #ifdef _DEBUG_
        logFile << "Reading layer from file: " << fNames + std::to_string(i) + ".NN" << std::endl;
        #endif

        if(!reader.is_open())
        {
            break;
        }
        in->layers.push_back(layer(reader)); //Read the layer data from the file and add it to our layers
        logFile << "Layer #" << i << " loaded from file " << fName << std::endl;
        char c;
        reader >> c;
        if(c == 'E') //End of file
        {
            break;
        }

    }
    in->numLays = in->layers.size();
    logFile << in->numLays << std::endl;
    reader.close();
}

void net::save(net* in, std::string fName) //
{
    std::ofstream writer; //Open the file in binary mode
    writer.open(fName);
    if(!writer.is_open()) //File failed to open
    {
        logFile << "Error: Failed to open NN file at " << fName << std::endl;
        return; //Don't exit program, but exit the function to write to file
    }
    
    unsigned i = 0; //Only used to give progress bar indication
    for(auto& lay : in->layers)
    {
        lay.write(writer);
        if(i != in->numLays - 1) writer << " B ";     //Write layer separator character   
        i++;

    }
    writer << " E"; //Write E to signify there are no more layers left
    writer.close();
}
