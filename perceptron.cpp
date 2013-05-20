#include "perceptron.h"
#include <stdlib.h>
#include <algorithm>
#include "iostream"


Perceptron::~Perceptron()
{
    for (int i = neirons.size() - 1; i >=  0; i--) {
        Neiron * neiron = neirons.at(i);
        delete neiron;
    }
}

//установка параметров сети
Perceptron::Perceptron(Sample teachExams, int eraCount, double alpha,  double velocity)
{
    this->eraCount = eraCount;
    this->velocity = velocity;
    this->alpha = alpha;
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
void Perceptron::teachPerceptron(bool isResult) {

    for (int i = 0; i < neirons.size(); i++) {
        Neiron * neiron = neirons.at(i);
        neiron->iniWeight();
    }


    for (int i = 0; i < eraCount; i++) {
        double _velocity = this->velocity - this->velocity * i / eraCount;
        teachingNeirons(isResult, _velocity);
    }
}
//обучение нейронов
void Perceptron::teachingNeirons(bool isResult, double velocity){
    std::vector<double*> examples = teachExamples.getExamples();
    for (int i = 0; i < examples.size(); i ++) {
        int numb = (int)(examples.size()*(double)rand()/RAND_MAX);
        double * example = examples[numb];
        examples.erase(examples.begin() + numb);
        for (int j = 0; j < teachExamples.neironCount; j++) {
            if (isResult) {
                if (teachExamples.neironsToNextLevel[j]) {
                    Neiron * neiron = neirons[j];
                    neiron->setVelocity(velocity);
                    neiron->calculateWeights(example, j);
                }
            } else {
                Neiron * neiron = neirons[j];
                neiron->setVelocity(velocity);
                neiron->calculateWeights(example, j);
            }
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

//    for (int i = 0; i < teachExamples.getEnterCount(); i++) {
//        std::cout<< exam[i] <<  ' ';
//    }
//    std::cout << std::endl;
//    neiron->showWeights();


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

void Perceptron::calcError(QString netName)
{
    int neironsCount = teachExamples.getNeironCount();

    double * denorm = new double[neironsCount] ;
    double * norm = new double[neironsCount] ;
    for (int i = 0; i < neironsCount; i++) {
        denorm[i] = 0;
        norm[i] = 0;
    }

    int n = teachExamples.getExamplesCount();

    for (int i = 0; i < teachExamples.getExamplesCount(); i++) {
        std::cout << teachExamples.getExamplesNum()[i] + 1 << '\t';
        for (int k = 0; k < getNeironsNum().size(); k++) {
            double value = getFunctionValue(k, i);
            double deNorm = value * (teachExamples.getNeironMaxValue(k) - teachExamples.getNeironMinValue(k)) + teachExamples.getNeironMinValue(k);
            double v = teachExamples.getExamples()[i][teachExamples.getEnterCount() + k];

            double celevoe = v * (teachExamples.getNeironMaxValue(k) - teachExamples.getNeironMinValue(k)) + teachExamples.getNeironMinValue(k);
            denorm[k] += (deNorm - celevoe) * (deNorm - celevoe);
            norm[k] += (value - v) * (value - v);
            std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
        }
        std::cout << std::endl;
    }

    std::cout << "Ошибка по " + netName.toStdString() + " сети: ";

    QString errorD = "Denorm: ";
    QString errorN = "Norm: ";
    for (int i = 0; i < teachExamples.neironCount; i++) {
        denorm[i] = sqrt(denorm[i] / n);
        errorD.append(QString::number(denorm[i]) + " ");
        norm[i] = sqrt(norm[i] / n);
        errorN.append(QString::number(norm[i]) + " ");

    }
    std::cout << errorD.toStdString() << errorN.toStdString() << " " << std::endl;
}

double Perceptron::getFunctionValue(int neironNum, double *exam)
{
//    std::cout << std::endl << std::endl;
    Neiron * neiron = neirons[neironNum];
//    for (int i = 0; i < teachExamples.getEnterCount(); i++) {
//        std::cout<< exam[i] <<  std::endl;
//    }
//    std::cout << std::endl;
//    neiron->showWeights();
    return neiron->calculateY(exam);
}



