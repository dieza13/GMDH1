#ifndef PERCEPTRONCONTEXT_H
#define PERCEPTRONCONTEXT_H
#include "vector"
#include "perceptron.h"
#include "filereader.h"
#include "sample.h"
#include "netslayer.h"
#include "firstlayernets.h"
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QVector>
#include <QMap>

class PerceptronContext
{
public:
    PerceptronContext();
    ~PerceptronContext();
public:
    void addNewNet(Perceptron * perceptron, int neironNetType);
    void teachNets();
    std::vector<int> getExamplesIntersect();
    FirstLayerNets * getFirstLayerNets();
    QGraphicsScene * getStructureVisualObj();
    QList<QString> getEntersName();
    QList<QString> getNeironsName();
    void addEnters(QMap<QString, int> enters);
    void addNeirons(QMap<QString, int> neirons);
    QMap<QString, int> getEnters();
    QMap<QString, double>  getResults(QMap<int, double> example);


private:
    FirstLayerNets * firstLayerNets;
    Perceptron * resultPerceptron;
    QMap<QString, int> neirons;
    QMap<QString, int> enters;
    Sample * resultSample;
private:
    std::vector<int> getResultVectorIntersect(std::vector<int> v1, std::vector<int> v2);
    std::vector<int> getResultNeironNumUnion(std::vector<int> v1, std::vector<int> v2);
    Sample * setResultNet();


    void setNeirons(QMap<QString, int> enters);

};

#endif // PERCEPTRONCONTEXT_H
