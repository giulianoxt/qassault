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

public slots:
    void move(int, int, int, int);
    
signals:
    // Input 
    void gameEnded();
    void opponentPlayed();
    void pieceClicked(int, int);
    void squareClicked(int, int);
    
    // Output
    void played();
    void createPiece(int, int, PlayerType);
    void highlightSquares(const QVector<QPoint>);
    void blankSquares(const QVector<QPoint>);
    void movePiece(int, int, int, int);
    
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
{ Q_OBJECT
    
public:
    SelectDestState(Player*, QtState*);

signals:
    void highlightSquares(const QVector<QPoint>);
    void blankSquares(const QVector<QPoint>);
    void move(int, int, int, int);
    
protected:
    QVector<QPoint> chosen;
    virtual void onEntry();
    virtual void onExit();
};

class ChooseStartPos : public PlayerState
{ Q_OBJECT
    
public:
    ChooseStartPos(Player*, int);
    
signals:
    void createPiece(int, int, PlayerType);
    void doneChoosing();
    
protected:
    virtual void onExit();

private:
    int n;
};

#endif // PLAYER_H
