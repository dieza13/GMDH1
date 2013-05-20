#include "netslayer.h"
#include <iostream>

void NetsLayer::addNewNet(Perceptron * perceptron)
{
    QString num = QString::number(Nets.size());
    perceptron->setPerceptronBody(num);
    Nets.push_back(perceptron);
}

NetsLayer::~NetsLayer()
{
    for (int i = 0; i < Nets.size(); i++)
        delete Nets[i];
}

void NetsLayer::showNeironsWeights(int i)
{
    Perceptron * net = Nets[i];
    for (int j = 0; j < net->getNeironsNum().size(); j++) {
        net->getNeiron(j)->showWeights();
    }

}
