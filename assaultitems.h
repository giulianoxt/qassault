#ifndef ASSAULTITEMS_H
#define ASSAULTITEMS_H

#include <QBrush>
#include <QGraphicsRectItem>
#include "globals.h"


class SquareItem : public QGraphicsRectItem
{
public:
    SquareItem(int, int);
    
    static void setSpotPic(const QPixmap&);
    
private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    
    static QBrush spotPic;
    
    int i, j;
};

#endif // ASSAULTITEMS_H
