#include "piece.h"

PieceItem::PieceItem(const QPixmap& pic)
{
    setPixmap(pic);
}

void PieceItem::setPixmap(const QPixmap& pic)
{
    QGraphicsPixmapItem::setPixmap(pic);
    
    setOffset((boardSquareSize - pic.width())  / 2.,
              (pic.height() - boardSquareSize) / 2.);    
}

