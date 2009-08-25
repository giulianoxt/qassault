#include "piece.h"
#include "globals.h"
#include<iostream>
using namespace std;

PieceItem::PieceItem(const QPixmap& pic, int _i, int _j) : i(_i), j(_j)
{
    setPixmap(pic);
}

void PieceItem::setPixmap(const QPixmap& pic)
{
    QGraphicsPixmapItem::setPixmap(pic);
    
    setOffset((boardSquareSize - pic.width())  / 2.,
              (pic.height() - boardSquareSize) / 2.);    
}

void PieceItem::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    emit clicked(i, j);
}
