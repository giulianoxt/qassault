#ifndef ASSAULTSCENE_H
#define ASSAULTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "globals.h"
#include "util.h"
#include "piece.h"
#include "assaultitems.h"


class AssaultScene : public QGraphicsScene
{    
    Q_OBJECT
    
public:
    AssaultScene();

signals:
    void pieceClicked(int, int);    
    void squareClicked(int, int);        
    
public slots:
    void startGame(const player_t&, const player_t&);
    void characterChanged(const QPixmap*, const QPixmap*);
    
private:
    void layoutItems();    
    
    const QPixmap *defensePixmap, *attackPixmap;
    SquareItem* squares[boardSize][boardSize];
    PieceItem* pieces[boardSize][boardSize];
};

#endif // ASSAULTSCENE_H
