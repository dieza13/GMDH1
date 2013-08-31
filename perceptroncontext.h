#ifndef PERCEPTRONCONTEXT_H
#define PERCEPTRONCONTEXT_H
#include "vector"
#include "perceptron.h"
#include "sample.h"
#include "netslayer.h"
#include "firstlayernets.h"
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QVector>
#include <QMap>
#include <EnterRange.h>

class PerceptronContext
{
public:
    PerceptronContext();
    ~PerceptronContext();
public:
    void addNewNet(Perceptron * perceptron, int neironNetType);
    void teachNets(QVBoxLayout * normalErrorPL);
    std::vector<int> getExamplesIntersect(std::vector<Sample> samples);
    FirstLayerNets * getFirstLayerNets();
    QGraphicsScene * getStructureVisualObj();
    QList<QString> getEntersName();
    QList<QString> getNeironsName();
    void addEnters(QMap<QString, int> enters);
    void addNeirons(QMap<QString, int> neirons);
    QMap<QString, int> getEnters();
    QMap<QString, double>  getResults(QMap<int, double> example);

public:
    FirstLayerNets * firstLayerNets;
    Perceptron * resultPerceptron;
public:
    QMap<QString, int> neirons;
    QMap<QString, int> enters;
public:
    std::vector<int> getResultVectorIntersect(std::vector<int> v1, std::vector<int> v2);
    std::vector<int> getResultNeironNumUnion(std::vector<int> v1, std::vector<int> v2);
    Sample * setResultNet();
    std::vector<double> optimizeAlpha(double startAlpha, double endAlpha, double step, int optimizeParam, int netNum);


    void setNeirons(QMap<QString, int> enters);
    std::vector<std::vector<double> > calcRange(std::vector<EnterRange> range, std::vector<QString> *names, QLabel * rangePrBInfoLab, QProgressBar *saveRangePrB);

};

#endif // PERCEPTRONCONTEXT_H
