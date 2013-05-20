#ifndef ITEMBODY_H
#define ITEMBODY_H
#include <QtGui>

class ItemBody : public QGraphicsItem
{
public:
    ItemBody();
    ~ItemBody();//////////////////
public:
    int getHeight();
    int getWidth();
public:
    QString bodyText;
    int bodyWidht;
    int bodyHeight;
    QColor * penColor;
    int penWidht;

protected:
    QRectF boundingRect() const;
};

#endif // ITEMBODY_H
