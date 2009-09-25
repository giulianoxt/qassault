#include "aiplayer.h"


AIPlayer::AIPlayer(PlayerType p) : type(p)
{ }


DummyAI::DummyAI(PlayerType p) : AIPlayer(p)
{ } 

const Move DummyAI::play(const GameState& st)
{
   const QList<Move> moves = st.moves(type);
   
   int i = randInt(0, moves.size()-1);
   return moves.at(i);
}
