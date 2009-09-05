#ifndef ASSAULTSCENE_H
#define ASSAULTSCENE_H

#include <QPoint>
#include <QList>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "globals.h"
#include "game.h"
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
    void startGame();
    void insertPiece(int, int, PlayerType);
    void characterChanged(const QPixmap*, const QPixmap*);
    void highlightMoves(const QList<Move>&);
    void blankMoves(const QList<Move>&);
    void movePiece(const Move&);
    
private:
    void layoutItems();    
    
    const QPixmap *defensePixmap, *attackPixmap;
    SquareItem* squares[boardSize][boardSize];
    PieceItem* pieces[boardSize][boardSize];
};

#endif // ASSAULTSCENE_H
