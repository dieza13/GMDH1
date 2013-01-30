#ifndef BODY_H
#define BODY_H
#include <QGraphicsItem>
#include <QPainter>

class Body : public QGraphicsItem
{
public:
    Body();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // BODY_H
