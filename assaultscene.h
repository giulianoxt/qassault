#ifndef ASSAULTSCENE_H
#define ASSAULTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "globals.h"

class AssaultScene : public QGraphicsScene
{
public:
    AssaultScene();

public slots:
    void startGame(const player_t&, const player_t&);

private:
    void layoutSquareItems();
    
    QGraphicsItem* squares[boardSize][boardSize];
    
};

#endif // ASSAULTSCENE_H
