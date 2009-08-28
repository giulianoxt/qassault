#ifndef GAME_H
#define GAME_H

#include <QPoint>
#include <QVector>
#include "globals.h"


enum PlayerType { Attack, Defense };

bool isValidSquare(int i, int j);

bool isInsideFortress(int i, int j);

typedef char SquareT;
const SquareT AttackPiece = 'A';
const SquareT DefensePiece = 'D';
const SquareT Empty = 'E';

SquareT squareType(PlayerType);


class GameState
{   
   
public:

    GameState();
    
    void init();
    void clear();
    
    bool isOpen(int, int) const;
    SquareT get(int, int) const;
    void set(int, int, SquareT);
    
    void move(int, int, int, int);
    
    const QVector<QPoint> moves(int, int) const;
    
private:    
    SquareT board[boardSize][boardSize];
};



#endif // GAME_H
