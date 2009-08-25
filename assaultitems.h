#ifndef ASSAULTITEMS_H
#define ASSAULTITEMS_H

#include <QBrush>
#include <QGraphicsRectItem>
#include "globals.h"


class SquareItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    
public:
    SquareItem(int, int);
    
    static void setSpotPic(const QPixmap&);
    
signals:
    void clicked(int, int);
    
private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    
    static QBrush spotPic;
    
    int i, j;
};

#endif // ASSAULTITEMS_H
