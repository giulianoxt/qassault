#include "piece.h"
#include "globals.h"
#include "util.h"


PieceItem::PieceItem(const QPixmap& pic, int i, int j)
{
    setPixmap(pic);
    move(i, j);
}

void PieceItem::move(int ni, int nj)
{
    i = ni;
    j = nj;
    
    setPos(boardToScene(i, j));
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
