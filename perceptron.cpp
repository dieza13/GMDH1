#include "perceptron.h"
#include <stdlib.h>
#include <algorithm>


Perceptron::~Perceptron()
{
    for (int i = 0; i < neirons.size(); i++) delete neirons.at(i);
}

//установка параметров сети
Perceptron::Perceptron(Sample teachExams, int eraCount, double alpha,  double velocity)
{
    this->eraCount = eraCount;
    this->velocity = velocity;
    teachExamples = teachExams;    
    setNeirons(teachExams.getEnterCount(), velocity, alpha);

}

Perceptron::Perceptron()
{

}

//создание нейронов и установка их параметров: количество входов, скорость обучения, параметр сигмоидальной функции alpha
void Perceptron::setNeirons(int enterCount, double velocity, double alpha)
{
    for (int i = 0; i < teachExamples.getNeironsNum().size(); i++) {
        Neiron * neiron = new Neiron(enterCount, velocity, alpha, teachExamples.getNeironsNum().at(i));
        neiron->setMinMaxFunkValue(teachExamples.getNeironMinValue(i), teachExamples.getNeironMaxValue(i));
        neirons.push_back(neiron);
    }
}

//обучение сети
void Perceptron::teachPerceptron() {
    for (int i = 0; i < eraCount; i++) {
        double _velocity = this->velocity - this->velocity * i / eraCount;
        teachingNeirons(_velocity);
    }
}
//обучение нейронов
void Perceptron::teachingNeirons(double velocity){
    std::vector<double*> examples = teachExamples.getExamples();
    for (int i = 0; i < examples.size(); i ++) {
        int numb = (int)(examples.size()*(double)rand()/RAND_MAX);
        double * example = examples[numb];
        examples.erase(examples.begin() + numb);
        for (int j = 0; j < teachExamples.getNeironsNum().size(); j++) {
            Neiron * neiron = neirons[j];
            neiron->setVelocity(velocity);
            neiron->calculateWeights(example, j);
        }
    }
}

std::vector<int> Perceptron::getExamplesNum()
{
    return teachExamples.getExamplesNum();
}

std::vector<int> Perceptron::getNeironsNum()
{
    return teachExamples.getNeironsNum();
}

void Perceptron::generateResultNet()
{
}

double Perceptron::getFunctionValue(int neironNum, int examNum)
{
    Neiron * neiron = neirons[neironNum];
    double * exam;
    for (int i = 0; i < getExamplesNum().size(); i++) {
        int num = getExamplesNum()[i];
        if (examNum == num) {
            exam = getExample(examNum);
            break;
        }
    }
    return neiron->calculateY(exam);
}

double * Perceptron::getExample(int num)
{
    int examNum;
    for (int i = 0; i < teachExamples.getExamplesNum().size(); i++) {
        if (num == teachExamples.getExamplesNum().at(i)) examNum = i;
    }
    return teachExamples.getExamples()[examNum];
}

Neiron * Perceptron::getNeiron(int i)
{
    return neirons[i];
}

Sample * Perceptron::getExamples()
{
    return &teachExamples;
}

//установка параметров выводимого на экран объекта - "Perceptron"
void Perceptron::setPerceptronBody(QString name)
{
    QList<QGraphicsItem*> neironsBody;
    for (int i = 0; i < neirons.size(); i++) {
        Neiron * neiron = neirons[i];
        neironsBody.push_back(neiron->getNeironBody());
    }
//    std::vector<int> nums;
    teachExamples.getEntersNum();
    perceptronBody = new PerceptronBody(name, neironsBody, *teachExamples.getEntersNum());
}

PerceptronBody *Perceptron::getPerceptronBody()
{
    return perceptronBody;
}

double Perceptron::getFunctionValue(int neironNum, double *exam)
{
    Neiron * neiron = neirons[neironNum];
    return neiron->calculateY(exam);
}



