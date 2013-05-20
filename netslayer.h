#ifndef NETSLAYER_H
#define NETSLAYER_H
#include "vector"
#include "perceptron.h"

class NetsLayer
{
public:
    ~NetsLayer();
protected:

public:
    std::vector<Perceptron*> Nets;

public:
    void addNewNet(Perceptron * perceptron); // добавление сети
    void showNeironsWeights(int i); // вывести в консоль веса сетей



};

#endif // NETSLAYER_H
