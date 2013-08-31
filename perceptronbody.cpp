#include "perceptronbody.h"
#include <iostream>


PerceptronBody::PerceptronBody(QString perceptronBodyText, std::vector<QGraphicsItem*> neirons, std::vector<int> enters)
{
    this->enters = enters; //
    enterSize = 30;
    neironSize = 50;
    penColor = new QColor(Qt::black);
    penWidht = 3;
    bodyWidht = 150;
    this->neirons = neirons;
    pressed = false;
//    setFlag(ItemIsMovable);
    bodyText = perceptronBodyText;
    bodyHeight = neirons.size() * neironSize + neironSize / 2 * (neirons.size() + 1);

    int enterAreaSize = enters.size() * enterSize + enterSize / 2 * (enters.size() + 1);
    if (bodyHeight < enterAreaSize)
        bodyHeight = enterAreaSize;
}

PerceptronBody::~PerceptronBody()
{
    for (int i = 0; i < neirons.size(); i++) {
        delete neirons.at(i);
    }
}

void PerceptronBody::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    pressed = false;
//    update();
//    QGraphicsItem::mouseReleaseEvent(event);
}

void PerceptronBody::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    pressed = true;
//    update();
//    QGraphicsItem::mousePressEvent(event);
}





void PerceptronBody::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int neironCount = neirons.size();





    QRectF rec = boundingRect();
    QBrush brush(Qt::white);

    painter->fillRect(rec, brush);


    int x = enterSize / 2 + enterSize + 10;
    int y,y1;
    int var;

    QFont font2("Courier", 8, QFont::Bold, true);
    painter->setFont(font2);
    painter->setPen(QPen(*penColor, 2));
    int size2 = bodyHeight / enters.size();
    for (int i = 0; i < enters.size(); i++) {
        int enterPosY = size2 * i + (size2 - enterSize) / 2 ;
        QRectF enterRec(enterSize / 2, enterPosY, enterSize, enterSize);
        painter->drawRect(enterRec);
        QString enterStr = QString::number(enters[i]);
        painter->drawText(enterSize / 2 + (enterSize - enterStr.size() * 7) / 2, enterPosY + enterSize / 2, enterStr);
        var = enterPosY + enterSize / 2;
        painter->drawLine(x - 10, var, x, var);
        if (i == 0) y = var;
    }
    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(x, y, x, var);


    QFont font1("Helvetica", 10, QFont::Bold);
    painter->setFont(font1);
    painter->setPen(QPen(Qt::red, 2));
    painter->drawText((bodyWidht - bodyText.size() * 8) * 2 / 3, neironSize / 4, bodyText);
    painter->setPen(QPen(*penColor, penWidht));
    painter->drawRect(rec);

    int size1 = bodyHeight / neironCount;
    var = bodyWidht * 1 / 2;
    for (int i = 0; i < neirons.size(); i++) {
        neirons.at(i)->setParentItem(this);
        y = size1 * i + (size1 - neironSize) / 2;
        neirons.at(i)->setPos(var, y);
        painter->drawLine(x, y + neironSize / 2, var, y + neironSize / 2);
        painter->drawLine(x + neironSize, y + neironSize / 2, var + 1.3 * neironSize, y + neironSize / 2);
        if (i == 0) y1 = y;
    }
    painter->drawLine(x, y1 + neironSize / 2, x, y + neironSize / 2);


}



