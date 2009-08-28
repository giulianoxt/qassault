#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include "assaultitems.h"
#include "util.h"


QBrush SquareItem::destPic;
QBrush SquareItem::killPic;
QBrush SquareItem::blankPic;
QBrush SquareItem::highlightPic;

void SquareItem::loadImages()
{
    SquareItem::highlightPic = QPixmap(":/resources/resources/spot.png");
    SquareItem::destPic = QPixmap(":/resources/resources/spot_2.png");
    SquareItem::killPic = QPixmap(":/resources/resources/spot_3.png");
    SquareItem::blankPic = Qt::transparent;
}

SquareItem::SquareItem(int _i, int _j) : 
    QGraphicsRectItem(0, 0, boardSquareSize, boardSquareSize), i(_i), j(_j)
{
    
    setPen(QPen(Qt::NoPen));
    setZValue(-1.0);
    setAcceptHoverEvents(true);
    
    changeState(Blank);
}


void SquareItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    e->accept();
    
    if (state == Blank)
        changeState(Highlight);
}

void SquareItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    e->accept();
    
    if (state == Highlight)
        changeState(Blank);
}

void SquareItem::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    emit clicked(i, j);
}

void SquareItem::changeState(SquareState st) {
    state = st;
    
    switch (st) {
        case Highlight:
          setBrush(SquareItem::highlightPic); break;
        case Destination:
          setBrush(SquareItem::destPic); break;
        case Kill:
          setBrush(SquareItem::killPic); break;
        case Blank:
          setBrush(SquareItem::blankPic); break;
    }

    update();
}
