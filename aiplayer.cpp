#include "aiplayer.h"
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;


AIPlayer::AIPlayer(PlayerType p) : type(p)
{
    reset(NULL);
}

void AIPlayer::reset(GameState* st)
{
    state = st;
    done = false;
}

void AIPlayer::timeout()
{
    done = true;
}

Move AIPlayer::getMove()
{
    return chosen_move;
}


DummyAI::DummyAI(PlayerType p) : AIPlayer(p)
{ }

void DummyAI::run()
{
    const QList<Move> moves = state->moves(type);
    QList<Move> copy(moves);
    random_shuffle(copy.begin(), copy.end());
    chosen_move = copy.back();
}
