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
        net->teachPerceptron();
        neironsCount += net->getNeironsNum().size();
    }

//    показать веса нейронов i-ой сети
    showNeironsWeights(0);

    for (int i = 0; i < resultSample->getExamplesCount(); i++ ) {
        int num = resultSample->getExamplesNum()[i];
        int posInExam = 0;
        for (int j = 0; j < Nets.size(); j++) {
            Perceptron * net = Nets[j];
            for (int k = 0; k < net->getNeironsNum().size(); k++) {
                double value = net->getFunctionValue(k, num);//getNeiron(k)->calculateY();
                resultSample->setEnter(i, posInExam, value);
                posInExam ++;
            }
        }
        posInExam = 0;
    }


//    std::vector<std::vector<double> > resultExams;

//    for (int i = 0; i < resultExamplesNum.size(); i++) {
//        int num = resultExamplesNum[i];
//        std::vector<double> exam;
//        for (int j = 0; j < Nets.size(); j++) {
//            Perceptron * net = Nets[j];
//            std::vector<double> exit = net->getFunctionValue(net->getExample(num));
//            exam.insert(exam.end(), exit.begin(), exit.end());
//        }
//        double * example = &exam[0];
//        resultExams.push_back(exam);

//    }

//    for (int i = 0; i < resultExams.size(); i++) {
//        std::vector<double> result = resultExams[i];
//        for (int j = 0; j < result.size(); j++) {
//            std::cout << /*example[0]*/ resultExams[i][j] * (Nets[0]->getNeiron(j)->getMaxValue() - Nets[0]->getNeiron(j)->getMinValue()) + Nets[0]->getNeiron(j)->getMinValue() << " ";

//        }
//        std::cout << std::endl;
//    }
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
     for (int i = 0; i < iSize; i ++) {
         for (int j = 0; j < jSize; j ++) {
             if (v1[i] == v2[j]) {
                 resultIntersection.push_back(v1[i]);
                 break;
             }
         }
     }
     return resultIntersection;
 }


 std::vector<int> FirstLayerNets::getNeironNum()
 {
     Perceptron * net1 = Nets[0];
     std::vector<int> neironsNum;
     for (int i = 0; i < net1->getNeironsNum().size(); i++) neironsNum.push_back(net1->getNeironsNum()[i]);
     for (int i = 1; i < Nets.size(); i++) {
         Perceptron * net = Nets[i];
         std::vector<int> nums = net->getNeironsNum();
         neironsNum.insert(neironsNum.end(), nums.begin(), nums.end());
     }
     return neironsNum;
 }

 FirstLayerNets::~FirstLayerNets()
 {
     for (int i = 0; i < Nets.size(); i++) delete Nets.at(i);
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
             results.push_back(net->getFunctionValue(j, exam));
         }
     }
     return results;
 }




