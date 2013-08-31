#include "perceptroncontext.h"
#include <iostream>
#include <stdio.h>

std::string CALCULATING_RANGE_LABEL = "Расчет диапазона...";
std::string CALCULATED_RANGE_LABEL = "Диапазон рассчитан.";

PerceptronContext::PerceptronContext()
{
    firstLayerNets = new FirstLayerNets();
    resultPerceptron = NULL;
}

PerceptronContext::~PerceptronContext()
{
    delete firstLayerNets;
    delete resultPerceptron;
}

void PerceptronContext::addNewNet(Perceptron * perceptron, int neironNetType)
{
    switch (neironNetType){
    case 1:
        firstLayerNets->addNewNet(perceptron);
        break;
    case 2:
        perceptron->setPerceptronBody("result_net");
        resultPerceptron = perceptron;
        break;
    }
}

void PerceptronContext::teachNets(QVBoxLayout * normalErrorPL)
{    
    std::vector<std::vector<double> > netsErr = firstLayerNets->teachNets(resultPerceptron->getExamples(), normalErrorPL);
    resultPerceptron->teachPerceptron(true);
    Sample * sample = resultPerceptron->getExamples();
    resultPerceptron->calcError("result net", normalErrorPL,true, &netsErr, 0, resultPerceptron->teachExamples);
}

std::vector<int>  PerceptronContext::getExamplesIntersect(std::vector<Sample> samples)
{
    return firstLayerNets->getResultVectorIntersect(samples);
}

FirstLayerNets * PerceptronContext::getFirstLayerNets()
{
    return firstLayerNets;
}

QGraphicsScene * PerceptronContext::getStructureVisualObj()
{
    QGraphicsScene * scene = new QGraphicsScene();
    LayerBody * firstLayerBody = firstLayerNets->getLayerBody();
    PerceptronBody * resultNetBody = resultPerceptron->getPerceptronBody();
    scene->addItem(resultNetBody);
    scene->addItem(firstLayerBody);
    if (resultNetBody->getHeight() < firstLayerBody->getHeight()) {
        resultNetBody->setPos( firstLayerBody->getWidth() + 50, (firstLayerBody->getHeight() - resultNetBody->getHeight()) / 2 );
    } else {
        resultNetBody->setPos( firstLayerBody->getWidth() + 50, 0);
        firstLayerBody->setPos(0, (resultNetBody->getHeight() - firstLayerBody->getHeight()) / 2 );
    }
    return scene;
}

void PerceptronContext::addEnters(QMap<QString, int> enters)
{
    QMapIterator<QString, int> i(enters);
    while (i.hasNext()) {
         i.next();
         if (!this->enters.contains(i.key()))
             this->enters.insert(i.key(), i.value());
    }
}

QList<QString> PerceptronContext::getEntersName()
{
    return enters.keys();
}

QMap<QString, int> PerceptronContext::getEnters()
{
    return enters;
}

QMap<QString, double> PerceptronContext::getResults(QMap<int, double> example)
{
    QVector<double> resultExam = firstLayerNets->calculateParams(example);
    int entersCount = resultPerceptron->getExamples()->getEnterCount();
    double * resultExample = new double[entersCount];
    int i = 0;
    for (i = 0; i < resultExam.size(); i++) {
        resultExample[i] = resultExam[i];
    }
    for (int j = 0; j < entersCount - resultExam.size(); j++) {
        int enterNum = resultPerceptron->getExamples()->getEntersNum()->at(j);
        double value = example[enterNum];
        double v = resultPerceptron->getExamples()->normalizEnterValue(value, enterNum);
        resultExample[j + i] = v;
    }
    for (int i = 0; i < entersCount; i++) {
        std::cout << resultExample[i] << " ";
    }
    std::cout << std::endl;
    QVector<double> resultValues;
    QMap<QString, double> denormalizResultValues;
    for (int i = 0; i < resultPerceptron->getNeironsNum().size(); i++) {
        double value;
        if(resultPerceptron->teachExamples.neironsToNextLevel[i]) {
            value = resultPerceptron->getFunctionValue(i, resultExample);
        } else {
            value = resultExam[i];
        }
        resultValues.push_back(value);
        Neiron * neiron = resultPerceptron->neirons[i];
        double maxVal = neiron->maxVal;//resultPerceptron->getExamples()->getNeironMaxValue(i);
        double minVal = neiron->minVal;//resultPerceptron->getExamples()->getNeironMinValue(i);
        double denormValue = value * (maxVal - minVal) + minVal;
        QString neironName;
        QMapIterator<QString, int> iter(neirons);
        while (iter.hasNext()) {
             iter.next();
             if (iter.value() == resultPerceptron->getNeironsNum().at(i))
                 neironName = iter.key();
        }
        denormalizResultValues.insert(neironName, denormValue);
    }
    return denormalizResultValues;
}

void PerceptronContext::addNeirons(QMap<QString, int> neirons)
{
    QMapIterator<QString, int> i(neirons);
    while (i.hasNext()) {
         i.next();
         if (!this->neirons.contains(i.key()))
             this->neirons.insert(i.key(), i.value());
    }
}

QList<QString> PerceptronContext::getNeironsName()
{
    return neirons.keys();
}

std::vector<double> PerceptronContext::optimizeAlpha(double startAlpha, double endAlpha, double step, int optimizeParam, int netNum)
{
    Perceptron * net;
    bool isResult = false;
    if (netNum < firstLayerNets->Nets.size()) {
        net = firstLayerNets->Nets[netNum];
    } else {
        isResult = true;
        net = resultPerceptron;
        firstLayerNets->teachNets(&(net->teachExamples));
    }
    std::vector<double> alphas;
    for (int i = 0; i < net->neirons.size(); i++) {
        Neiron * neiron = net->neirons[i];
        double resultError = 0xffffffff;
        double resultAlpha = startAlpha;
        for (double alpha = startAlpha; alpha <= endAlpha; alpha += step) {
            neiron->alpha = alpha;
            double error = 0xffffffff;
            for (int j = 0; j < optimizeParam; j++) {
                net->teachPerceptron(isResult);
                double e = net->getError();
                if (e < error)
                    error = e;
            }
            if (resultError > error) {
                std::cout << std::endl << "new error:" << error << std::endl;
                resultError = error;
                resultAlpha = neiron->alpha;
            }
        }
        neiron->alpha = resultAlpha;
        alphas.push_back(resultAlpha);
    }
    return alphas;
}


std::vector<std::vector<double> > PerceptronContext::calcRange(std::vector<EnterRange> range, std::vector<QString> * names, QLabel * rangePrBInfoLab, QProgressBar * saveRangePrB)
{
    rangePrBInfoLab->setText(QString::fromStdString(CALCULATING_RANGE_LABEL));

    QMap<QString, double> res;
    std::vector<std::vector<double> > results;
    QMap<int, double> example;
    int count = 1;
    QMap<QString, EnterRange*> outputs;
    QMap<int, QString> entersName;
    for (int i = 0; i < range.size(); i++) {
         EnterRange * struc = &range[i];
         int index = struc->index;
         int num = (struc->endValue - struc->startValue + 1) / struc->step;
         count *= (num > 0) ? num : 1;
         if (index == -1) {
             outputs[struc->paramTitle] = struc;
         } else {
             example[index] = struc->startValue;
             entersName[index] = struc->paramTitle;
         }
    }
    saveRangePrB->setMaximum(count);
    int num = 0;
    while (num < count) {
        res = getResults(example);
        std::vector<double> result;
        QMapIterator<int, double> iter1(example);
        while (iter1.hasNext()) {
            double value = iter1.next().value();
            result.push_back(value);
        }
        QMapIterator<QString, double> iter2(res);
        bool isAdding = true;
        while (iter2.hasNext()) {
            double value = iter2.next().value();
            EnterRange * param = outputs[iter2.key()];
            if (value <= param->endValue && value >= param->startValue) {
                isAdding =true;
                result.push_back(value);
            } else {
                isAdding = false;
                break;
            }
        }
        if(isAdding)
            results.push_back(result);
        saveRangePrB->setValue(num);
        for (int i = 0; i < range.size(); i++) {
            EnterRange * struc = &range[i];
            int index = struc->index;
            if (index < 0) break;
            example[index] += struc->step;
            if (example[index] <= struc->endValue)
                break;
            else
                example[index] = struc->startValue;
        }
        num++;
    }

    QMapIterator<int, double> iter1(example);
    while (iter1.hasNext()) {
        int value = iter1.next().key();
        names->push_back(entersName[value]);
    }
    rangePrBInfoLab->setText(QString::fromStdString(CALCULATED_RANGE_LABEL));
    return results;
}
