#include "itembody.h"

ItemBody::ItemBody()
{
}

ItemBody::~ItemBody()///////////////////////////////
{
//    i
//    delete penColor;
}

QRectF ItemBody::boundingRect() const
{
    return QRectF(0, 0, bodyWidht, bodyHeight);
}

int ItemBody::getHeight()
{
    return bodyHeight;
}

int ItemBody::getWidth()
{
    return bodyWidht;
}




