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


DummyPlayer::DummyPlayer(PlayerType p) : AIPlayer(p)
{ }

void DummyPlayer::run()
{
    const QList<Move> moves = state->moves(type);
    QList<Move> copy(moves);
    random_shuffle(copy.begin(), copy.end());
    chosen_move = copy.back();
}


GreedyPlayer::GreedyPlayer(PlayerType p) : AIPlayer(p)
{ }

void GreedyPlayer::run()
{
    const QList<Move> moves = state->moves(type);
    
    GameState next;
    double bestEval;
    
    if (type == Attack) bestEval = GameState::evalMin;
    else bestEval = GameState::evalMax;
    
    foreach (const Move& m, moves) {
        state->copyAndMove(m, next);
        double eval = next.eval();
        
        if ((type == Attack  && eval > bestEval) ||
            (type == Defense && eval < bestEval)) {
            bestEval = eval;
            chosen_move = m;
        }
    }
}


MinimaxPlayer::MinimaxPlayer(PlayerType p, int l = moveLookaheads)
        : AIPlayer(p), minimaxAI(p, l)
{ }

void MinimaxPlayer::run()
{
    chosen_move = minimaxAI.searchTreeCache(*state);
    
    cout << chosen_move << endl;
}
