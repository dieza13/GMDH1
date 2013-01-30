#include "body.h"
//визуализация
Body::Body()
{
    QGraphicsTextItem *text = new QGraphicsTextItem(this);

    text->setTextInteractionFlags(Qt::TextEditorInteraction);
    text->setPos(10,10);
    text->setPlainText("Test");
}

QRectF Body::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void Body::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
}
