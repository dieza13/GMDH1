#include "firstlayernets.h"
#include "netslayer.h"
#include <iostream>

FirstLayerNets::FirstLayerNets()
{
}

std::vector<std::vector<double> > FirstLayerNets::teachNets(Sample *resultSample, QVBoxLayout * normalErrorPL)
{
    std::vector<std::vector<double> > netsErr;
    // обучение сетей первого уровня, получение количества входов второго уровня
    int neironsCount = 0;
    for (int i = 0; i < Nets.size(); i++) {
        Perceptron * net = Nets[i];
        net->teachPerceptron(false);
        neironsCount += net->getNeironsNum().size();
    }
//    показать веса нейронов i-ой сети
    showNeironsWeights(0);

    for (int i = 0; i < resultSample->getExamplesCount(); i++ ) {
        int num = resultSample->getExamplesNum()[i];

        int posInEnter = 0;
        int posInNeur = resultSample->enterCount;

        for (int j = 0; j < Nets.size(); j++) {
            Perceptron * net = Nets[j];
            double * exam = net->getExample(num);
            for (int k = 0; k < net->getNeironsNum().size(); k++) {
                double value = net->getFunctionValue(k, exam);
                if (net->teachExamples.neironsToNextLevel[k]) {
                    resultSample->setEnter(i, posInEnter, value);
                    posInEnter ++;
                }  else {
                    resultSample->setEnter(i, posInNeur, value);
                }
                posInNeur ++;
            }
        }
        posInEnter = 0;
    }

    for (int j = 0; j < Nets.size(); j++) {
        Perceptron * net = Nets[j];
        for (int j = 0; j < net->neirons.size(); j++) {
            std::vector<double> minMaxNorm;
            Neiron * neiron = net->neirons[j];
            minMaxNorm.push_back(neiron->normDownLim);
            minMaxNorm.push_back(neiron->normUpLim);
            resultSample->minMaxNormNeironsValue.push_back(minMaxNorm);
        }
    }

    int count = 0;
    for (int j = 0; j < Nets.size(); j++) {
        Perceptron * net = Nets[j];
        net->calcError("net №" + QString::number(j + 1), normalErrorPL, false, &netsErr, count, net->teachExamples);
        count += net->teachExamples.neironCount;
        std::cout << std::endl;
    }
    return netsErr;
}

void FirstLayerNets::setResultExamples(std::vector<Sample> * samples)
{
    Sample * resultSample = &(samples->at(samples->size() - 1));
    for (int i = 0; i < resultSample->getExamplesCount(); i++ ) {
        int num = resultSample->getExamplesNum()[i];

        int posInEnter = 0;
        int posInNeur = resultSample->enterCount;

        for (int j = 0; j < Nets.size(); j++) {
            Sample * sample = &samples->at(j);
            Perceptron * net = Nets[j];
            double * exam = sample->examples[num];
            for (int k = 0; k < net->getNeironsNum().size(); k++) {
                double value = net->getFunctionValue(k, exam);
                if (net->teachExamples.neironsToNextLevel[k]) {
                    resultSample->setEnter(i, posInEnter, value);
                    posInEnter ++;
                }  else {
                    resultSample->setEnter(i, posInNeur, value);
                }
                posInNeur ++;
            }
        }
        posInEnter = 0;
    }
}


void FirstLayerNets::teachNets(Sample *resultSample)
{
    // обучение сетей первого уровня, получение количества входов второго уровня
    int neironsCount = 0;
    for (int i = 0; i < Nets.size(); i++) {
        Perceptron * net = Nets[i];
        net->teachPerceptron(false);
        neironsCount += net->getNeironsNum().size();
    }
////    показать веса нейронов i-ой сети
    for (int i = 0; i < resultSample->getExamplesCount(); i++ ) {
        int num = resultSample->getExamplesNum()[i];
        int posInEnter = 0;
        int posInNeur = resultSample->enterCount;
        for (int j = 0; j < Nets.size(); j++) {
            Perceptron * net = Nets[j];
            double * exam = net->getExample(num);
            for (int k = 0; k < net->getNeironsNum().size(); k++) {
                double value = net->getFunctionValue(k, exam);
                if (net->teachExamples.neironsToNextLevel[k]) {
                    resultSample->setEnter(i, posInEnter, value);
                    posInEnter ++;
                }  else {
                    resultSample->setEnter(i, posInNeur, value);
                }
                posInNeur ++;
            }
        }
        posInEnter = 0;
    }
}

 std::vector<int>  FirstLayerNets::getResultVectorIntersect(std::vector<Sample> samples)
{
     std::vector<int> resultExamplesNum = ((Sample)samples[0]).getExamplesNum();
     for (int i = 1; i < Nets.size(); i++) {
         Sample sample = samples[i];
         resultExamplesNum = twoVectorIntersect(resultExamplesNum, sample.getExamplesNum());
     }
    return resultExamplesNum;
 }

 std::vector<int> FirstLayerNets::twoVectorIntersect(std::vector<int> v1, std::vector<int> v2)
 {
     int iSize, jSize;
     if (v1.size() < v2.size()) {
         iSize = v1.size();
         jSize = v2.size();
     } else {
         jSize = v1.size();
         iSize = v2.size();
     }
     std::vector<int> resultIntersection;
     for (int i = 0; i < v1.size(); i ++) {
         for (int j = 0; j < v2.size(); j ++) {
             if (v1[i] == v2[j]) {
                 resultIntersection.push_back(v1[i]);
                 break;
             }
         }
     }
     return resultIntersection;
 }

void FirstLayerNets::setResultSample(Sample *sample)
 {
     Perceptron * net1 = Nets[0];
     std::vector<bool> neironsFromPrevLev;
     std::map<int, bool> neirons;
     for (int i = 0; i < sample->neironsNum.size(); i++)
         neironsFromPrevLev.push_back(true);
     for (int i = 0; i < Nets.size(); i++) {
         Perceptron * net = Nets[i];
         for (int j = 0; j < net->getNeironsNum().size(); j++) {
             int neironNum = net->getNeironsNum()[j];
             QString neironName = net->teachExamples.neironsName[j];
             std::cout << neironName.toStdString() << std::endl;
             if (net->teachExamples.neironsToNextLevel[j])
                 sample->entersNum.push_back(neironNum);
                 sample->entersName.push_back(neironName);
             if (!neirons[neironNum]) {
                 sample->neironsNum.push_back(neironNum);
                 sample->neironsName.push_back(neironName);
                 neirons[neironNum] = true;
                 if (net->teachExamples.neironsToNextLevel[j])
                     neironsFromPrevLev.push_back(true);
                 else
                     neironsFromPrevLev.push_back(false);
             }
         }
     }
     sample->neironCount = sample->neironsNum.size();
     sample->enterCount = sample->entersNum.size();

     sample->neironsToNextLevel = neironsFromPrevLev;
 }

 FirstLayerNets::~FirstLayerNets()
 {
 }

 int FirstLayerNets::getNetsCount()
 {
     return Nets.size();
 }

 PerceptronBody *FirstLayerNets::getNetVisualObj(int netNum)
 {
     Perceptron * perceptron = Nets.at(netNum);
     return perceptron->getPerceptronBody();
 }

 void FirstLayerNets::setLayerBody()
 {
     std::vector<PerceptronBody *>  perceptrons;
     for (int i = 0; i < Nets.size(); i++) {
         Perceptron * perceptron = Nets[i];
         perceptrons.push_back(perceptron->getPerceptronBody());
     }
     layerBody = new LayerBody(perceptrons);
 }

 LayerBody *FirstLayerNets::getLayerBody()
 {
     setLayerBody();
     return layerBody;
 }

 QVector<double> FirstLayerNets::calculateParams(QMap<int, double> example)
 {
     QVector<double> results;
     for (int i = 0; i < Nets.size(); i++) {
         Perceptron * net = Nets.at(i);
         int enterCount = net->getExamples()->getEnterCount();
         double * exam = new double[enterCount];
         for (int j = 0; j < enterCount; j++) {
             int enterNum = net->getExamples()->getEntersNum()->at(j);
             double value = example[enterNum];
             exam[j] = net->getExamples()->normalizEnterValue(value, enterNum);
         }
         for (int j = 0; j < net->getNeironsNum().size(); j++) {
             double value = net->getFunctionValue(j, exam);
             results.push_back(value);
         }
     }
     return results;
 }






