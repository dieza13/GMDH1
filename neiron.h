#ifndef NEIRON_H
#define NEIRON_H
#include "QLabel"
#include <neironbody.h>

class Neiron
{
public:
    Neiron(int enterCount, double velocity, double a, int neironNum);
    ~Neiron();
public:
    void setVelocity(double v);
    double getVelocity();
    void setAlpha(double a);
    double getAlpha();
    double calculateY(double * X);
    void setMinMaxFunkValue(double min, double max);
    void setMinMaxNormValue(double min, double max);
    double calculateWeights(double *X, int yNum); //пересчитать веса
    void setNeironBody(int num);
    int getNeironNum();
    double getMaxValue();
    double getMinValue();
    void showWeights();
    NeironBody * getNeironBody();
    void  iniWeight(); //инициализация весов случайными маленькими значениями


public:
    double * weight;// массив весов
    int enterCount;//количество входов
    double alpha;// параметр сигмоидальной функции
    double velocity;//скорость обучения
    double maxVal;
    double minVal;
    double normUpLim = 1; // верхняя граница нормализации
    double normDownLim = 0; // нижняя граница нормализации
    int neironNum;    
private:


    NeironBody * neironBody;
private:        

};

#endif // NEIRON_H
