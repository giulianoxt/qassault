#ifndef GAME_H
#define GAME_H

#include <QPoint>
#include <QList>
#include <QVector>
#include <QMetaType>
#include "globals.h"
#include "util.h"
#include <iostream>
using std::ostream;
using std::cout;


class Move;

bool isValidSquare(int, int);
bool isInsideFortress(int, int);
bool isDestinySquare(int, int, const QList<Move>&);
bool isDiagonalKill(int, int, const QList<Move>&);
        

enum PlayerType { Attack, Defense };

QString playerTypeString(PlayerType);

typedef char SquareT;
const SquareT AttackPiece = 'A';
const SquareT DefensePiece = 'D';
const SquareT Empty = 'E';

SquareT squareType(PlayerType);


#define foreach_validSquare(i,j)\
   for_(i, 0, boardSize) for_(j, 0, boardSize)\
    if (isValidSquare(i, j))

#define foreach_fortressSquare(i,j)\
   for_(i, fortressBounds[0][0], fortressBounds[0][1]+1)\
   for_(j, fortressBounds[1][0], fortressBounds[1][1]+1)

#define foreach_attackPiece(i,j)\
   foreach_validSquare(i, j) if (board[i][j] == AttackPiece)

       
class GameState
{   
public:

    GameState();
    GameState(const GameState&);
    
    void init();
    void copy(const GameState&);
    bool gameOver() const;
    bool gameOver(PlayerType&) const;
    
    bool operator==(const GameState&) const;
    
    uint attackSize() const;
    uint defenseSize() const;
    
    bool isOpen(int, int) const;
    SquareT get(int, int) const;
    SquareT get(const QPoint&) const;
    
    void insertDefensePiece(int, int);
    
    void move(const Move&);
    GameState* copyAndMove(const Move&) const;
    void copyAndMove(const Move&, GameState&) const;
    
    double eval() const;
    static const double evalMax; // Attack win
    static const double evalMin; // Defense win
    
    const QList<Move> moves(int, int) const;
    const QList<Move> moves(const PlayerType&) const;
    
protected:    
    void initRound();
    void set(int, int, SquareT);
    void set(const QPoint&, SquareT);
    const QList<Move> attackMoves(int, int) const;
    const QList<Move> defenseMoves(int, int) const;
        
    bool has_def;
    uint attackOnFort;
    QPoint defA, defB;
    QList<Move> movA, movB;
    
    uint attackSz, defenseSz;
    SquareT board[boardSize][boardSize];
    
protected:    
    static const int defenseMovesAll[8][2];
    static const int attackMovesAll[8][2];
    static const int attackMovesForward[5][2];    
    
friend ostream& operator<<(ostream&, const GameState&);
friend istream& operator>>(istream&, GameState&);
};

ostream& operator<<(ostream&, const GameState&);

istream& operator>>(istream&, GameState&);

ostream& operator<<(ostream&, const Move&);

class Move
{
public:
    Move();
    Move(const Move&);
    Move(const QPoint&, const QPoint&);
    
    bool isDiagonalKill() const;
    const QPoint& origin() const;    
    const QPoint& destiny() const;
    
    void setOrigin(const QPoint&);
    void setDestiny(const QPoint&);
    
    bool isKill() const;
    int killSize() const;
    void addKill(const QPoint&);
    const QList<QPoint>& kills() const;

    bool operator<(const Move&) const;
    bool operator==(const Move&) const;
    
private:
    QPoint orig, dest;
    QList<QPoint> kill;
};

Q_DECLARE_METATYPE(Move);

#endif // GAME_H
