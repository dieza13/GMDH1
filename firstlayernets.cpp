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

    ////////////////////s
    double * d = new double[neironsCount] ;
    for (int i = 0; i < neironsCount; i++) {
        d[i] = 0;
    }
/////////////////////////////////f


//    показать веса нейронов i-ой сети
    showNeironsWeights(0);

    for (int i = 0; i < resultSample->getExamplesCount(); i++ ) {
        int num = resultSample->getExamplesNum()[i];
        std::cout << resultSample->getExamplesNum()[i] << '\t';
        int posInExam = 0;
        for (int j = 0; j < Nets.size(); j++) {
            Perceptron * net = Nets[j];
            for (int k = 0; k < net->getNeironsNum().size(); k++) {
                double value = net->getFunctionValue(k, num);
                resultSample->setEnter(i, posInExam, value);
//////////////////////////////s
                double deNorm = value * (resultSample->getNeironMaxValue(k) - resultSample->getNeironMinValue(k)) + resultSample->getNeironMinValue(k);
                double celevoe = resultSample->getExamples()[i][resultSample->getEnterCount() + k] * (resultSample->getNeironMaxValue(k) - resultSample->getNeironMinValue(k)) + resultSample->getNeironMinValue(k);
                d[k] += (deNorm - celevoe) * (deNorm - celevoe);
/////////////////////////f
                std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
                posInExam ++;
            }
        }
        std::cout << std::endl;
        posInExam = 0;
    }

////////////////////////////s
    int n = resultSample->getExamplesCount();

    std::cout << "Ошибкапо первой сети: ";

    for (int i = 0; i < neironsCount; i++) {
        d[i] = sqrt(d[i] / n);
        std::cout << d[i] << " ";
    }

    std::cout << std::endl;
//////////////////////////////f

    std::cout << "/////////////////" << std::endl;

//    for (int i = 0; i < resultSample->getExamples().size(); i++) {
//        for (int j = 0; j < 4; j++) {
//            std::cout <<  resultSample->getExamples()[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }

}





//double * d = new double[neironsCount] ;

//for (int i = 0; i < neironsCount; i++) {
//    d[i] = 0;
//}




////    показать веса нейронов i-ой сети
//showNeironsWeights(0);

//for (int i = 0; i < resultSample->getExamplesCount(); i++ ) {
//    int num = resultSample->getExamplesNum()[i];
//    std::cout << resultSample->getExamplesNum()[i] << '\t';
//    int posInExam = 0;
//    for (int j = 0; j < Nets.size(); j++) {
//        Perceptron * net = Nets[j];
//        for (int k = 0; k < net->getNeironsNum().size(); k++) {
//            double value = net->getFunctionValue(k, num);
//            resultSample->setEnter(i, posInExam, value);

//            ///////////
//            double deNorm = value * (resultSample->getNeironMaxValue(k) - resultSample->getNeironMinValue(k)) + resultSample->getNeironMinValue(k);
//            double celevoe = resultSample->getExamples()[i][resultSample->getEnterCount() + k] * (resultSample->getNeironMaxValue(k) - resultSample->getNeironMinValue(k)) + resultSample->getNeironMinValue(k);
//            d[k] += (deNorm - celevoe) * (deNorm - celevoe);
//            ///////////

//            std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
//            posInExam ++;
//        }
//    }
//    std::cout << std::endl;
//    posInExam = 0;
//}

//int n = resultSample->getExamplesCount();

//std::cout << "Ошибка: ";

//for (int i = 0; i < neironsCount; i++) {
//    d[i] = sqrt(d[i] / n);
//    std::cout << d[i] << " ";
//}

//std::cout << std::endl;


//std::cout << "/////////////////" << std::endl;

//for (int i = 0; i < resultSample->getExamples().size(); i++) {
//    for (int j = 0; j < 4; j++) {
//        std::cout <<  resultSample->getExamples()[i][j] << " ";
//    }
//    std::cout << std::endl;
//}
















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

     for (int i = 0; i < net1->getNeironsNum().size(); i++)
//         if (net1->teachExamples.neironsToNextLevel[i])
             neironsNum.push_back(net1->getNeironsNum()[i]);
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
             double value = net->getFunctionValue(j, exam);
             results.push_back(value);
         }
     }
     return results;
 }




