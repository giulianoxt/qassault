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
        

enum PlayerType { Attack, Defense };

QString playerTypeString(PlayerType);

typedef char SquareT;
const SquareT AttackPiece = 'A';
const SquareT DefensePiece = 'D';
const SquareT Empty = 'E';

SquareT squareType(PlayerType);


class GameState
{   
public:

    GameState();
    GameState(const GameState&);
    
    void init();
    
    bool gameOver() const;
    bool gameOver(PlayerType&) const;
    
    uint attackSize() const;
    uint defenseSize() const;
    
    bool isOpen(int, int) const;
    SquareT get(int, int) const;
    SquareT get(const QPoint&) const;
    
    void insertDefensePiece(int, int);
    
    void move(const Move&);
    GameState* copyAndMove(const Move&);
    
    const QList<Move> moves(int, int) const;
    const QList<Move> moves(const PlayerType&) const;
    
    bool operator==(const GameState&) const;
    
private:    
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
    
friend ostream& operator<<(ostream&, const GameState&);
};

ostream& operator<<(ostream&, const GameState&);

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
