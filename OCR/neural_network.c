#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_network.h"
#include <math.h>

double my_random()
{
    double r = rand();
    int sign = rand()%2;
    sign = sign == 1 ? 1 : -1;
    while (r > 1) {
        r /= 10;
    }
    r *= rand();
    while(r > 1)
        r /= 10;
    return sign*r;
}

struct neural_network *createNetwork(unsigned layerCount, unsigned inputCount, unsigned neuronCount)
{
    // allocs
    struct neuron **layers = malloc(sizeof(struct neuron*) * layerCount);
    layers[0] = malloc(sizeof(struct neuron) * inputCount);
    for (unsigned i = 1; i < layerCount - 1; i++) {
            layers[i] = malloc(sizeof(struct neuron) * neuronCount);
    }
    layers[layerCount - 1] = malloc(sizeof(struct neuron));
    // inits
    // input layer
    for (unsigned i = 0; i < inputCount; i++) {
        layers[0][i].input = 0;
        layers[0][i].type = input;
        layers[0][i].connectionsCount = neuronCount;
        layers[0][i].delta = 0;
        layers[0][i].w = malloc(sizeof(double) * neuronCount);
        for (unsigned k = 0; k < neuronCount; k++) {
            layers[0][i].w[k] = my_random();
        }
    }
    // hidden layers
    for (unsigned i = 1; i < layerCount - 2; i++) {
        for (unsigned j = 0; j < neuronCount; j++) {
            layers[i][j].input = 0;
            layers[i][j].type = hidden;
            layers[i][j].connectionsCount = neuronCount;
            layers[i][j].delta = 0;
            layers[i][j].w = malloc(sizeof(double) * neuronCount);
            for (unsigned k = 0; k < neuronCount; k++) {
                layers[i][j].w[k] = my_random();
            }
        }
    }

    // last hidden layer
    for (unsigned i = 0; i < neuronCount; i++) {
        layers[layerCount - 2][i].input = 0;
        layers[layerCount - 2][i].type = hidden;
        layers[layerCount - 2][i].connectionsCount = 1;
        layers[layerCount - 2][i].delta = 0;
        layers[layerCount - 2][i].w = malloc(sizeof(double));
        layers[layerCount - 2][i].w[0] = my_random();
    }

    // output layer
    layers[layerCount - 1][1].input = 0;
    layers[layerCount - 1][1].type = output;
    layers[layerCount - 1][1].connectionsCount = 0;
    layers[layerCount - 2][1].delta = 0;
    layers[layerCount - 1][1].w = NULL;
    
    struct neural_network *new_network = malloc(sizeof(struct neural_network));
    new_network->neurons = layers;
    new_network->inputCount = inputCount;
    new_network->layerCount = layerCount;
    new_network->neuronPerLayer = neuronCount;

    return new_network;
}

double getOutput(struct neural_network *network)
{
    struct neuron **neurons = network->neurons;
    for (unsigned i = 0; i < network->inputCount; i++) {
        for (unsigned j = 0; j < network->neuronPerLayer; j++) {
            neurons[1][j].input += neurons[0][i].w[j] * neurons[0][i].input;
        }
    }
    // hidden layers
    for (unsigned k = 1; k < network->layerCount - 2; k++) {
        for (unsigned i = 0; i < network->neuronPerLayer; i++) {
            for (unsigned j = 0; j < network->neuronPerLayer; j++) {
                neurons[k+1][j].input += neurons[k][i].w[j] * sigmoide(neurons[k][i].input);
            }
        }
    }
    // last hidden layer
    unsigned n = network->layerCount;
        for (unsigned i = 0; i < network->neuronPerLayer; i++) {
            neurons[n - 1][0].input += neurons[n - 2][i].w[0] * sigmoide(neurons[n - 2][i].input);
        }
    
    return sigmoide(network->neurons[n - 1][0].input);
}

void freeNetwork(struct neural_network *network)
{
    for(unsigned i = 0; i < network->inputCount; i++)
    {
       free(network->neurons[0][i].w);
    }
   for(unsigned i = 1; i < network->layerCount-1;i++)
   {
        for (unsigned j = 0; j < network->neuronPerLayer;j++)
        {
           free((network->neurons[i][j]).w); 
        }
   }
    for (unsigned i = 0 ; i < network->layerCount;i++)
   {
        free(network->neurons[i]);
   }
   free(network->neurons);
   free(network);
}

double sigmoide(double s)
{
    double res = 0;
    res = 1 / (1 + exp(-s));
    return res;
}

void learn(struct neural_network *network, double res)
{
    //Output
    unsigned n = network->layerCount;
    network->neurons[n-1][0].delta = res -sigmoide(network->neurons[n - 1][0].input);

    //Last hidden Layers
    
}
