#ifndef GAME_H
#define GAME_H

#include "globals.h"


enum PlayerType { Attack, Defense };

bool isValidSquare(int i, int j);

bool isInsideFortress(int i, int j);


typedef char SquareT;
const SquareT AttackPiece = 'A';
const SquareT DefensePiece = 'D';
const SquareT Empty = 'E';

class GameState
{   
   
public:
    GameState();
    
    void clear();
    bool isOpen(int, int) const;
    
private:
    SquareT board[boardSize][boardSize];
};



#endif // GAME_H
