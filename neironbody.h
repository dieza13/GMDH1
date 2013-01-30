#ifndef NEIRONBODY_H
#define NEIRONBODY_H
#include <QtGui>
#include <itembody.h>

class NeironBody : public ItemBody
{
public:
    NeironBody(QString neironBodyText);
    ~NeironBody();
private:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    bool pressed;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // NEIRONBODY_H
