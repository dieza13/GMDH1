#ifndef PERCEPTRON_H
#define PERCEPTRON_H
#include "vector"
#include "neiron.h"
#include "sample.h"
#include <perceptronbody.h>

class Perceptron
{
public:
    Perceptron();
    Perceptron(Sample teachExams, int eraCount, double alpha, double velocity);
    ~Perceptron();
public:
    void teachPerceptron();
    std::vector<int>  getExamplesNum();
    std::vector<int> getNeironsNum();
    double getFunctionValue(int neironNum, int examNum);
    double getFunctionValue(int neironNum, double * exam);
    double * getExample(int num);
    Neiron * getNeiron(int i);
    Sample * getExamples();
    void setPerceptronBody(QString name);
    PerceptronBody * getPerceptronBody();
public:
    Sample teachExamples;

private:
    double velocity;
    int eraCount;
    std::vector<Neiron*> neirons;

    PerceptronBody * perceptronBody;

private:
    void setNeirons(int enterCount, double velocity, double alpha);
    void teachingNeirons(double velocity);
    std::vector<double*> randomExamps();
    void generateResultNet();



};

#endif // PERCEPTRON_H
