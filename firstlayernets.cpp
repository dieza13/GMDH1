#include "firstlayernets.h"
#include "netslayer.h"
#include <iostream>

FirstLayerNets::FirstLayerNets()
{

}





void FirstLayerNets::teachNets(Sample *resultSample)
{
    // свормировать лист с номерами примеров, которые будут подаваться на сеть второго звена
    std::vector<int> resultExamplesNum = getResultVectorIntersect();

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
            for (int k = 0; k < net->getNeironsNum().size(); k++) {

                double value = net->getFunctionValue(k, num);


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
        net->calcError("net №" + QString::number(j + 1));
        std::cout << std::endl;
    }

}




 std::vector<int>  FirstLayerNets::getResultVectorIntersect()
{
     std::vector<int> resultExamplesNum = ((Perceptron*)Nets[0])->getExamplesNum();
     for (int i = 1; i < Nets.size(); i++) {
         Perceptron * net = Nets[i];
         resultExamplesNum = twoVectorIntersect(resultExamplesNum, net->getExamplesNum());
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
 //TODO: можно ускорить!!!
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
             if (net->teachExamples.neironsToNextLevel[j])
                 sample->entersNum.push_back(neironNum);
             if (!neirons[neironNum]) {
                 sample->neironsNum.push_back(neironNum);
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
//     for (int i = 0; i < net1->getNeironsNum().size(); i++) {
//         if (net1->teachExamples.neironsToNextLevel[i])
//             sample->entersNum.push_back(net1->getNeironsNum()[i]);
//         sample->neironsNum.push_back(net1->getNeironsNum()[i]);
//     }

//     for (int i = 1; i < Nets.size(); i++) {
//         Perceptron * net = Nets[i];
//         std::vector<int> nums = net->getNeironsNum();
//         neironsNum.insert(neironsNum.end(), nums.begin(), nums.end());
//     }
//     return neironsNum;
 }

 FirstLayerNets::~FirstLayerNets()
 {
//     delete layerBody;
//     for (int i = 0; i < Nets.size(); i++) {
//         Perceptron * perceptron= Nets.at(i);
//         delete perceptron;
//     }


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
     QList<PerceptronBody *>  perceptrons;
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




