#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "game.h"
#include "util.h"
#include <QThread>
#include <QAtomicInt>


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


class DummyAI : public AIPlayer
{
public:
    DummyAI(PlayerType);

protected:
    virtual void run();
};


#endif // AIPLAYER_H
