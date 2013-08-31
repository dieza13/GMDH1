#include "neiron.h"
#include <stdlib.h>
#include <math.h>
#include "QPainter"
#include "iostream"


void Neiron::iniWeight()
{
    for (int i = 0; i < enterCount + 1; i++){
        weight[i] = 0.1 + 0.1 * 2 * (float)rand() / (float)RAND_MAX;
    }
}

Neiron::~Neiron()
{
    if (weight != NULL || weight != 0)
        delete weight;
}

Neiron::Neiron(int enterCount, double velocity, double alpha, int neironNum)
{
    this->enterCount = enterCount;
    this->velocity = velocity;
    this->alpha = alpha;    
    weight = new double[this->enterCount + 1];
    this->neironNum = neironNum;
    neironBody = new NeironBody(QString::number(neironNum));
    iniWeight();
}

double Neiron::getVelocity()
{
    return velocity;
}

void Neiron::setAlpha(double a)
{
    alpha = a;
}

double Neiron::getAlpha()
{
    return alpha;
}

void Neiron::setVelocity(double v)
{
    velocity = v;
}

double Neiron::calculateWeights(double *X, int yNum)
{
    double y = calculateY(X);
    double Y = X[enterCount + yNum];
    double d = Y  - y;
    double v = velocity;
    weight[0] +=  v * d * 1;
    for (int i = 0; i < enterCount; i ++) {
        weight[i + 1] += v * d * X[i];
    }
    return y;
}

double Neiron::calculateY(double *X)
{
    double S = 0,y,d;
    for (int i = 0; i < enterCount;i ++) {
        double w = weight[i + 1];        
        double x = X[i];        
        S += x * w;
    }
    S += weight[0];
    double a = alpha;
    y = 1 / (1 + exp (- S * a));
    return y;
}

void Neiron::setMinMaxFunkValue(double min, double max)
{
    this->maxVal = max;
    this->minVal = min;
}

void Neiron::setMinMaxNormValue(double min, double max)
{
    this->normDownLim = min;
    this->normUpLim = max;
}

int Neiron::getNeironNum()
{
    return neironNum;
}

double Neiron::getMaxValue()
{
    return maxVal;
}

double Neiron::getMinValue()
{
    return minVal;
}

void Neiron::showWeights()
{
    std::cout << "neiron weight:" << std::endl;
    for (int i = 0; i < enterCount + 1; i++) std::cout << weight[i] << std::endl;
}

NeironBody *Neiron::getNeironBody()
{
    return neironBody;
}


