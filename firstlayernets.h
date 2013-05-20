#ifndef FIRSTLAYERNETS_H
#define FIRSTLAYERNETS_H
#include "netslayer.h"
#include <perceptronbody.h>
#include <layerbody.h>
#include <QVector>
class FirstLayerNets : public NetsLayer
{
public:
    FirstLayerNets();
    ~FirstLayerNets();
private:
    LayerBody * layerBody;
    std::vector<int> twoVectorIntersect(std::vector<int> v1, std::vector<int> v2);

public:
    void teachNets(Sample * resultSample);
    std::vector<int> getResultVectorIntersect(); //выбрать примеры, которые характерны каждой сети
    void setResultSample(Sample * sample);
    int getNetsCount();
    PerceptronBody * getNetVisualObj(int netNum);
    void setLayerBody();
    LayerBody * getLayerBody();
    QVector<double> calculateParams(QMap<int, double> example);
};

#endif // FIRSTLAYERNETS_H
