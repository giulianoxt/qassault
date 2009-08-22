#include <QBrush>
#include "assaultitems.h"


SquareItem::SquareItem(int _i, int _j) : 
    QGraphicsRectItem(0, 0, boardSquareSize, boardSquareSize), i(_i), j(_j)
{
    setBrush(QBrush(debugColor));
    setZValue(-1.0);
    setVisible(false);
}
