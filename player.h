#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtState>
#include <QtStateMachine>
#include "game.h"
#include "transitions.h"

class OpenSquareClicked;
class PieceClicked;
class DestSquareClicked;


class Player : public QObject
{
    Q_OBJECT
    
public:
    Player(const PlayerType&, GameState*);

signals:
    void gameEnded();
    void opponentPlayed();
    void pieceClicked(int, int);
    void squareClicked(int, int);
    
protected:
    PlayerType type;
    GameState* gameState;
    QtStateMachine* machine;
};


class HumanPlayer : public Player
{

public:
    HumanPlayer(const PlayerType&, GameState*);
};

#endif // PLAYER_H
