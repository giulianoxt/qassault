#include "minimax.h"
#include <QList>
#include <string>

bool oneMoveWin(const GameState&, bool, int&);
double minimax_(const GameState&, bool, int, int&, int&, double, double);


int k;

void minimax(const GameState& st, bool max, int& movInd)
{
    k = 0;
    
    if (st.moves(max ? Attack : Defense).size() == 1) {
        movInd = 0; k = 1;
    }
    else if (oneMoveWin(st, max, movInd)) {
        return;
    }
    else {
        int steps = 0;
        
        double eval = minimax_(st, max, moveLookaheads, movInd, steps,
                               GameState::evalMin, GameState::evalMax);
        if (eval == GameState::evalMin || eval == GameState::evalMax)
            cout << (eval == GameState::evalMin ? "Defense" : "Attack") << " is going to win!" << endl;
    }
    
    cout << k << " nodes" << endl;
}


bool oneMoveWin(const GameState& st, bool max, int& movInd)
{
    if (!st.almostOver())
        return false;
    
    movInd = 0;
    GameState next;
    const QList<Move> moves = st.moves(max ? Attack : Defense);
    
    foreach (const Move& m, moves) {
        st.copyAndMove(m, next);
        if (next.gameOver())
            return true;
        ++movInd;
    }
    
    return false;
}

double minimax_(const GameState& st, bool max, int depth,
                int& movInd, int& stepsNeeded, double alpha, double beta)
{        
    if (!depth || st.gameOver()) {
        ++k;
        stepsNeeded = moveLookaheads - depth;
        return st.eval();
    }

    movInd = 0;
    int i = 0, d = 0, steps;
    int bestEvalSteps;
    GameState child;
    
    if (max) {
        bestEvalSteps = stepsNeeded; 
        const QList<Move> moves = st.moves(Attack);
        
        for (QList<Move>::const_iterator it = moves.begin(); it != moves.end(); ++it, ++i) {                 
            st.copyAndMove(*it, child);            
            double eval = minimax_(child, false, depth-1, d, steps, alpha, beta);
            
            if (eval > alpha) {
                alpha = eval;
                bestEvalSteps = steps;
                movInd = i;
            }
            else if (eval == alpha && steps < bestEvalSteps) {
                bestEvalSteps = steps;
                movInd = i;
            }
            
            if (alpha >= beta)
                break;
        }
        
        return alpha;
    }
    else {
        bestEvalSteps = stepsNeeded;
        const QList<Move> moves = st.moves(Defense);
        
        for (QList<Move>::const_iterator it = moves.begin(); it != moves.end(); ++it, ++i) {            
            st.copyAndMove(*it, child);
            double eval = minimax_(child, true, depth-1, d, steps, alpha, beta);
            
            if (eval < beta) {
                beta = eval;
                bestEvalSteps = steps;
                movInd = i;
            }
            else if (eval == beta && steps < bestEvalSteps) {
                bestEvalSteps = steps;
                movInd = i;
            }
            
            if (alpha >= beta)
                break;
        }
        
        return beta;
    }
}
