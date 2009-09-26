#include "aiplayer.h"
#include <iostream>
using namespace std;


AIPlayer::AIPlayer(PlayerType p) : type(p)
{ }


DummyAI::DummyAI(PlayerType p) : AIPlayer(p)
{ } 

const Move DummyAI::play(const GameState& st)
{
   const QList<Move> moves = st.moves(type);   
   
   int i = randInt(0, moves.size()-1);
   
   Move m = moves.at(i);
   return m;
}
