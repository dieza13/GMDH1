#include "neironbody.h"

NeironBody::NeironBody(QString neironBodyText)
{
    penColor = new QColor(Qt::black);
    pressed = false;
//    setFlag(ItemIsMovable);
    bodyWidht = 50;
    bodyHeight = 50;
    bodyText = neironBodyText;
    penWidht = 3;
}

void NeironBody::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    pressed = true;
//    update();
//    QGraphicsItem::mousePressEvent(event);
}

void NeironBody::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    pressed = false;
//    update();
//    QGraphicsItem::mouseReleaseEvent(event);
}



void NeironBody::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::white);

    painter->fillRect(rec, brush);
    painter->setPen(QPen(*penColor, penWidht));

    painter->drawText((bodyWidht - bodyText.size() * 8) / 2, rec.height() / 2 + 5, bodyText);
    painter->drawRect(rec);

}

NeironBody::~NeironBody()
{
    delete penColor;
}
