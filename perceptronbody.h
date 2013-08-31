#ifndef PERCEPTRONBODY_H
#define PERCEPTRONBODY_H
#include <QtGui>
#include <QVector>
#include <neironbody.h>
#include <itembody.h>

class PerceptronBody : public  ItemBody
{

public:

public:
    PerceptronBody(QString perceptronBodyText, std::vector<QGraphicsItem*> neirons, std::vector<int>  enters);
    ~PerceptronBody();

//    void addNeironBody(NeironBody * neironBody);

private:

    void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    bool pressed;
    std::vector<QGraphicsItem*> neirons;
    int enterSize;
    int neironSize;
    std::vector<int>  enters;
protected:
//    QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PERCEPTRONBODY_H
