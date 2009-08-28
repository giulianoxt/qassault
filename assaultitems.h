#ifndef ASSAULTITEMS_H
#define ASSAULTITEMS_H

#include <QBrush>
#include <QGraphicsRectItem>
#include "globals.h"


class SquareItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
       
public:
    enum SquareState {
        Highlight, Destination, Kill, Blank
    };
    
    SquareItem(int, int);
    
    static void loadImages();
    void changeState(SquareState);
    
signals:
    void clicked(int, int);
    
private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    
    int i, j;
    SquareState state;
    
    static QBrush highlightPic, destPic, killPic, blankPic;
};

#endif // ASSAULTITEMS_H
