#include "perceptroncontext.h"
#include <iostream>
#include <stdio.h>

PerceptronContext::PerceptronContext()
{
    firstLayerNets = new FirstLayerNets();
    resultPerceptron = NULL;
//    secondTypeNets = new NetsLayer();
}

PerceptronContext::~PerceptronContext()
{
    delete firstLayerNets;
    delete resultPerceptron;
//    for (int i = 0; i < firstTypeNets.size(); i++) delete firstTypeNets[i];
//    for (int i = 0; i < secondTypeNets.size(); i++) delete secondTypeNets[i];

//    delete resultSample;
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

void PerceptronContext::teachNets()
{    
    firstLayerNets->teachNets(resultPerceptron->getExamples());
//    for (int i = 0; i < resultPerceptron->getExamplesNum().size(); i++) {
//        double * exam = resultPerceptron->getExample(i);
//        for (int j = 0; j < resultPerceptron->getExamples()->getEnterCount() + resultPerceptron->getExamples()->getNeironCount(); j++)
//            std::cout << exam[j] << ' ';
//        std::cout << std::endl;
//    }

    std::cout << "asdasd" << std::endl;
    resultPerceptron->teachPerceptron();

    Sample * sample = resultPerceptron->getExamples();

    std::cout << "|||||||||||" << std::endl;

    ////////////////////s
    int neironsCount = resultPerceptron->getNeironsNum().size();
    double * d = new double[neironsCount] ;
    for (int i = 0; i < neironsCount; i++) {
        d[i] = 0;
    }
    /////////////////////////////////f


    for (int i = 0; i < sample->getExamples().size(); i++) {
        double  exam[2];
        exam[0] = sample->getExamples()[i][2];
        exam[1] = sample->getExamples()[i][3];
        for (int j = 0; j < resultPerceptron->getNeironsNum().size(); j++) {
            double value = resultPerceptron->getFunctionValue(j, exam);

            //////////////////////////////s
            double deNorm = value * (resultPerceptron->getExamples()->getNeironMaxValue(j) - resultPerceptron->getExamples()->getNeironMinValue(j)) + resultPerceptron->getExamples()->getNeironMinValue(j);
            double celevoe = resultPerceptron->getExamples()->getExamples()[i][resultPerceptron->getExamples()->getEnterCount() + j] * (resultPerceptron->getExamples()->getNeironMaxValue(j) - resultPerceptron->getExamples()->getNeironMinValue(j)) + resultPerceptron->getExamples()->getNeironMinValue(j);
            d[j] += (deNorm - celevoe) * (deNorm - celevoe);
            /////////////////////////f


            std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
//            std::cout << resultPerceptron->getFunctionValue(j, exam) << " ";
        }
        std::cout << std::endl;
    }

    ////////////////////////////s
    int n = resultPerceptron->getExamples()->getExamplesCount();

    std::cout << "Ошибка по второй сети: ";

    for (int i = 0; i < neironsCount; i++) {
        d[i] = sqrt(d[i] / n);
        std::cout << d[i] << " ";
    }

    std::cout << std::endl;
    //////////////////////////////f

//    for (int i = 0; i < sample->getExamplesNum().size(); i++) {
//        double * exam = sample->getExamples()[i];
//        for (int j = 0; j < (sample->getEnterCount() + sample->getNeironCount()); j++) {
//            if (j < sample->getEnterCount())
//            /*std::cout << */printf("%4.5f ", exam[j]) /*<< ' '*/;
//            else
//                printf("%4.5f ", exam[j] * (sample->getNeironMaxValue(j - sample->getEnterCount()) - sample->getNeironMinValue(j - sample->getEnterCount())) + sample->getNeironMinValue(j - sample->getEnterCount()));

//        }
////        std::cout << std::endl;
//    }
}














std::vector<int>  PerceptronContext::getExamplesIntersect()
{
    return firstLayerNets->getResultVectorIntersect();
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
//    resultNetBody->setParentItem(scene);
//    firstLayerBody->setParentItem(scene);

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
    for (i = 0; i < entersCount - resultExam.size(); i++) {
        int enterNum = resultPerceptron->getExamples()->getEntersNum()->at(i);
        double value = example[enterNum];
        resultExample[i] = resultPerceptron->getExamples()->normalizEnterValue(value, enterNum);
    }
    for (int j = 0; j < resultExam.size(); j++) {
        resultExample[j + i] = resultExam[j];
    }
    QVector<double> resultValues;
    QMap<QString, double> denormalizResultValues;
    for (int i = 0; i < resultPerceptron->getNeironsNum().size(); i++) {
        double value = resultPerceptron->getFunctionValue(i, resultExample);
        resultValues.push_back(value);
        double maxVal = resultPerceptron->getExamples()->getNeironMaxValue(i);
        double minVal = resultPerceptron->getExamples()->getNeironMinValue(i);
        double denormValue = value * (maxVal - minVal) + minVal;


        QString neironName;
        QMapIterator<QString, int> iter(neirons);
        while (iter.hasNext()) {
             iter.next();
             if (iter.value() == resultPerceptron->getNeironsNum().at(i))
                 neironName = iter.key();

        }


//        std::cout << neironName.toStdString() << std::endl;
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
