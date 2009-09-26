#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "game.h"
#include "util.h"

class AIPlayer
{
public:
    AIPlayer(PlayerType);
    virtual const Move play(const GameState&) = 0;
    
protected:
    PlayerType type;
};


class DummyAI : public AIPlayer
{
public:
    DummyAI(PlayerType);
    virtual const Move play(const GameState&);
};


#endif // AIPLAYER_H
