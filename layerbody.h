#ifndef LAYERBODY_H
#define LAYERBODY_H
#include <QList>
#include <QtGui>
#include <itembody.h>
#include <perceptronbody.h>

class LayerBody  : public  ItemBody
{

public:
    LayerBody(std::vector<PerceptronBody*> perceptrons);
    ~LayerBody();

private:
    int border;
    std::vector<PerceptronBody*> perceptrons;

private:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // LAYERBODY_H
