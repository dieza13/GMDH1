#include "layerbody.h"



LayerBody::LayerBody(std::vector<PerceptronBody *> perceptrons)
{
    this->perceptrons = perceptrons;
    border = 40;
    bodyHeight = border;
    for (int i = 0; i < perceptrons.size(); i++) {
        PerceptronBody * netBody = perceptrons.at(i);
        netBody->setParentItem(this);
        netBody->setPos(border / 2, bodyHeight);
        bodyHeight += netBody->getHeight() + border;
        bodyWidht = netBody->getWidth();

    }



    bodyWidht += 40;
}

LayerBody::~LayerBody()
{
    for (int i = 0; i < perceptrons.size(); i++) {
        delete perceptrons.at(i);
    }

}

void LayerBody::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QBrush brush(Qt::white);

    QPen pen(Qt::green, 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    QRectF layerRec(0, 0, bodyWidht, bodyHeight);
    painter->fillRect(layerRec, brush);
    painter->drawRect(layerRec);



}
