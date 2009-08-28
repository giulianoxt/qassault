#include "game.h"
#include "globals.h"
#include <cstring>
#include <iostream>
using namespace std;


bool isValidSquare(int i, int j)
{
    return  (i >= 0 && i < boardSize &&
             j >= 0 && j < boardSize) &&
            !((i <= 1 || i >= boardSize-2) &&
             (j <= 1 || j >= boardSize-2));
}

bool isInsideFortress(int i, int j)
{
    return (i >= 2 && i <= 4 && j >= 2 && j <= 4);
}

SquareT squareType(PlayerType t)
{
    if (t == Attack)
        return AttackPiece;
    else
        return DefensePiece;
}


GameState::GameState()
{ }

void GameState::clear()
{
    memset(board, Empty, sizeof board);
}

void GameState::init()
{
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            board[i][j] = isInsideFortress(i, j) ? Empty : AttackPiece;
}

bool GameState::isOpen(int i, int j) const
{
    return board[i][j] == Empty;
}

SquareT GameState::get(int i, int j) const
{
    return board[i][j];
}

void GameState::set(int i, int j, SquareT type)
{
    board[i][j] = type;
}

void GameState::move(int i, int j, int ni, int nj)
{
    board[ni][nj] = board[i][j];
    board[i][j] = Empty;
}

const QVector<QPoint> GameState::moves(int i, int j) const
{    
    static const int attackMoves[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    static const int defenseMoves[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };
    
    QVector<QPoint> movesV;
    movesV.reserve(10);
    
    switch (board[i][j]) {
        case AttackPiece:        
          for (int m = 0; m < 4; ++m) {
              int ni = i + attackMoves[m][0], nj = j + attackMoves[m][1];
            
              if (isValidSquare(ni, nj) && isOpen(ni, nj))
                  movesV.push_back(QPoint(ni, nj));
          }
          break;
        
        case DefensePiece:
          for (int m = 0; m < 8; ++m) {
              int ni = i + defenseMoves[m][0], nj = j + defenseMoves[m][1];
              
              if (isValidSquare(ni, nj) && isOpen(ni, nj))
                  movesV.push_back(QPoint(ni, nj));
          }
          break;
    };
    
    return movesV;
}
