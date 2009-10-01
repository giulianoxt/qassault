#ifndef PLAYER_H
#define PLAYER_H

#include <QTimer>
#include <QObject>
#include <QtState>
#include <QVariant>
#include <QtStateMachine>
#include "game.h"
#include "animations.h"
#include "transitions.h"
#include "aiplayer.h"
#include "util.h"
#include "globals.h"

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
    void move(const Move&);
    
signals:
    // Input 
    void gameEnded(PlayerType);
    void opponentPlayed();
    void pieceClicked(int, int);
    void squareClicked(int, int);
    
    // Output
    void played();
    void createPiece(int, int, PlayerType);
    void highlightMoves(const QList<Move>&);
    void blankMoves(const QList<Move>&);
    void movePiece(const Move&);
    
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
    // Output
    void highlightMoves(const QList<Move>&);
    void blankMoves(const QList<Move>&);
    
protected:
    QList<Move> destMoves;
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


class ComputerPlayer : public Player
{ Q_OBJECT

public:
    ComputerPlayer(const PlayerType&, GameState*,
                   QObject*, QObject*, AIPlayer*);

signals:
    void choose();

public:
    AIPlayer* ai;
    QObject* waitBar;
};


class AIChoosePosState : public PlayerState
{ Q_OBJECT
    
public:
    AIChoosePosState(Player*);

signals:
    void createPiece(int, int, PlayerType);
    void doneChoosing();

protected:
    virtual void onEntry();
    
};

class AIWaitState : public PlayerState
{
public:
    AIWaitState(Player*);
};


class AIPlayState : public PlayerState
{ Q_OBJECT
    
public:
    AIPlayState(Player*);
    
private slots:
    void terminateAI();
    
protected:
    virtual void onEntry();
    virtual void onExit();
    AIPlayer* getAIPlayer();
    
protected:
    QTimer aiTimer;
};


class PieceMovingState : public PlayerState
{ Q_OBJECT
    
public:
    PieceMovingState(Player*);
    void addFinishedTransition(QtState*);

signals:
    // Output
    void highlightMoves(const QList<Move>&);
    void blankMoves(const QList<Move>&);
    
protected:
    virtual void onEntry();
    virtual void onExit();
    
    QList<Move> possibleMoves();
    
protected:
    Move m;
    PieceMovingAnimation animation;
};

#endif // PLAYER_H
