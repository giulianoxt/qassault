#ifndef ASSAULTSCENE_H
#define ASSAULTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "globals.h"
#include "piece.h"
#include "assaultitems.h"


class AssaultScene : public QGraphicsScene
{
public:
    AssaultScene();

public slots:
    void startGame(const player_t&, const player_t&);
    void characterChanged(const QPixmap*, const QPixmap*);

private:
    void layoutItems();
    
    const QPixmap *knightPixmap, *pawnPixmap;
    SquareItem* squares[boardSize][boardSize];
    PieceItem* pieces[boardSize][boardSize];
};

#endif // ASSAULTSCENE_H
