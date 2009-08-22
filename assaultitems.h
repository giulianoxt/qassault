#ifndef ASSAULTITEMS_H
#define ASSAULTITEMS_H

#include <QGraphicsRectItem>
#include "globals.h"

class SquareItem : public QGraphicsRectItem
{
public:
    SquareItem(int, int);
    
private:
    int i, j;
};

#endif // ASSAULTITEMS_H
