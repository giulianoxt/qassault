#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include "assaultitems.h"
#include "util.h"


SquareItem::SquareItem(int _i, int _j) : 
    QGraphicsRectItem(0, 0, boardSquareSize, boardSquareSize), i(_i), j(_j)
{
    
    setPen(QPen(Qt::NoPen));
    setBrush(Qt::transparent);
    setZValue(-1.0);
    setAcceptHoverEvents(true);
}

QBrush SquareItem::spotPic;


void SquareItem::setSpotPic(const QPixmap& pic)
{
    SquareItem::spotPic = QBrush(pic);
}

void SquareItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    e->accept();
    setBrush(SquareItem::spotPic);
    update();
}

void SquareItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    e->accept();
    setBrush(Qt::transparent);
    update();
}
