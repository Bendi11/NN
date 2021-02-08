# NN
A neural network framework made from scratch in C++. It is pretty incomplete and doesn't offer many features, especially compared to any good neural network library
like Tensorflow, OpenNN, etc.

## Features
- CLI based neural network management
- Saving / loading neural networks to a file
- Easy image loading using a manifest file in a data folder

## Missing
- Easily configurable NN properties like activation functions
- GPU support
- Multithreading

## Using As a Library
This can be used standalone in a project, just as a library. Copy the folder src/neural to your project 
and in a cmakelists.txt file, use add_subdirectory(neural) and link_libraries(libNeural)
