#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <QThread>
#include <QAtomicInt>
#include "util.h"
#include "game.h"
#include "minimax.h"


class AIPlayer : public QThread
{
public:
    AIPlayer(PlayerType);
    
    Move getMove();
    void reset(GameState*);

public slots:
    void timeout();

protected:
    virtual void run() = 0;

protected:    
    Move chosen_move;
    GameState* state;
    PlayerType type;
    QAtomicInt done;
};


class DummyPlayer : public AIPlayer
{
public:
    DummyPlayer(PlayerType);

protected:
    virtual void run();
};


class GreedyPlayer : public AIPlayer
{
public:
    GreedyPlayer(PlayerType);

protected:
    virtual void run();
};


class MinimaxPlayer : public AIPlayer
{
public:
    MinimaxPlayer(PlayerType, int);

protected:
    virtual void run();
    
    MinimaxAI minimaxAI;
};

#endif // AIPLAYER_H
