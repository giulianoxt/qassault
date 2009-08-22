#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsPixmapItem>
#include "globals.h"

class PieceItem : public QGraphicsPixmapItem
{
public:
    PieceItem(const QPixmap&);

    void setPixmap(const QPixmap&);
};

#endif // PIECE_H
