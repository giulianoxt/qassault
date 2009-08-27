#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtState>
#include <QVariant>
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
    Player(const PlayerType&, GameState*, QObject*);

    PlayerType getType();    
    GameState* getGameState();
    QVariantHash* getStateData();
    
signals:
    // Input 
    void gameEnded();
    void opponentPlayed();
    void pieceClicked(int, int);
    void squareClicked(int, int);
    
    // Output
    void createPiece(int, int, PlayerType);
    
protected:
    PlayerType type;
    QVariantHash stateData;
    
    QObject* statusObj;
    GameState* gameState;
    QtStateMachine* machine;
};


class HumanPlayer : public Player
{

public:
    HumanPlayer(const PlayerType&, GameState*, QObject*);
};


class ComputerPlayer : public Player
{

public:
    ComputerPlayer(const PlayerType&, GameState*, QObject*);
};


class PlayerState : public QtState
{
public:
    PlayerState(Player*, QtState*);
    
protected:
    Player* player;
};


class WaitState : public PlayerState
{
public:
    WaitState(Player*);
};

class EndState : public PlayerState
{
public:
    EndState(Player*);
};

class PlayState : public PlayerState
{
public:
    PlayState(Player*);
};

class SelectPieceState : public PlayerState
{
public:
    SelectPieceState(Player*, QtState*);
};

class SelectDestState : public PlayerState
{
public:
    SelectDestState(Player*, QtState*);
};

class ChooseStartPos : public PlayerState
{ Q_OBJECT
    
public:
    ChooseStartPos(Player*);

signals:
    void createPiece(int, int, PlayerType);
    
protected:
    virtual void onExit();
};

#endif // PLAYER_H
