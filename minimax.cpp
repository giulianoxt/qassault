#include "minimax.h"
#include <QList>

double minimax_(const GameState&, bool, int, int&, double, double);


int k;

void minimax(const GameState& st, bool max, int& movInd)
{
    k = 0;
    
    if (st.moves(max ? Attack : Defense).size() == 1) {
        movInd = 0;
        k = 1;
    }
    else {
        double alpha = GameState::evalMin, beta = GameState::evalMax;
        minimax_(st, max, moveLookaheads, movInd, alpha, beta);
    }
    
    cout << k << " nodes" << endl;
}

double minimax_(const GameState& st, bool max, int depth, int& movInd, double alpha, double beta)
{
    if (!depth || st.gameOver()) {
        ++k;
        return st.eval();
    }

    movInd = 0;
    int i = 0, d = 0;
    double bestEval;
    GameState child;
    
    if (max) {
        bestEval = GameState::evalMin;
        const QList<Move> moves = st.moves(Attack);
    
        for (QList<Move>::const_iterator it = moves.begin(); it != moves.end(); ++it, ++i) {     
            if (alpha >= beta) break;
            
            st.copyAndMove(*it, child);            
            double eval = minimax_(child, false, depth-1, d, alpha, beta);
            if (eval > bestEval) {
                bestEval = eval;
                movInd = i;
            }
            
            if (eval > alpha) alpha = eval;
        }
    }
    else {
        bestEval = GameState::evalMax;
        const QList<Move> moves = st.moves(Defense);
        
        for (QList<Move>::const_iterator it = moves.begin(); it != moves.end(); ++it, ++i) {
            if (alpha >= beta) break;
            
            st.copyAndMove(*it, child);
            double eval = minimax_(child, true, depth-1, d, alpha, beta);
            if (eval < bestEval) {
                bestEval = eval;
                movInd = i;
            }
            
            if (eval < beta) beta = eval;
        }
    }
    
    return bestEval;
}
