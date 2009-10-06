#include "minimax.h"
#include <QList>
#include <string>
#include <cassert>


MinimaxAI::MinimaxAI(PlayerType p, int depth)
        : isMax(p == Attack),
          lookahead(depth),
          player(p),
          cache(cacheSize)
{ }


Move MinimaxAI::searchTree(const GameState& st)
{    
    numNodes = 0;
    int movInd = -1;
    const QList<Move> moves = st.moves(player);
    
    if (moves.size() == 1) {
        movInd = 0;
        numNodes = 1;
    }
    else if (oneMoveWin(st, moves, movInd)) {
        numNodes = movInd + 1;
    }
    else {
        double eval = minimax(
            st, isMax, lookahead, movInd,
            GameState::evalMin, GameState::evalMax
        );
        
        if (eval == GameState::evalMin)
            cout << "Defense is going to win!" << endl;
        else if (eval == GameState::evalMax)
            cout << "Attack is going to win!" << endl;
    }
    
    //cout << numNodes << " nodes" << endl;
    return moves.at(movInd);
}

bool MinimaxAI::oneMoveWin(const GameState& st,
                           const QList<Move>& moves,
                           int& movInd)
{
    if (!st.almostOver()) return false;
    
    movInd = 0;
    GameState next;
    
    foreach (const Move& m, moves) {
        st.copyAndMove(m, next);
        if (next.gameOver()) return true;
        ++movInd;
    }
    
    return false;
}

double MinimaxAI::minimax(const GameState& st, bool max,
                          int depth, int& movInd,
                          double alpha, double beta)
{   
    ++numNodes;
    
    if (!depth || st.gameOver()) {
        return st.eval();
    }

    double nodeEval;
    ResultType type = Exact;
    
    movInd = 0;
    int i = 0, d;
    
    GameState child;
    QList<Move>::const_iterator it;
    const QList<Move> moves = st.moves(max ? Attack : Defense);
    
    if (max) {        
        for (it = moves.begin(); it != moves.end(); ++it, ++i) {                 
            st.copyAndMove(*it, child);            
            double eval = minimax(child, false, depth-1, d, alpha, beta);
            
            if (eval > alpha) {
                alpha = eval;
                movInd = i;
            }
            
            if (alpha >= beta) {
                type = LowerBound;
                break;
            }
        }
        
        nodeEval = alpha;
    }
    else {        
        for (it = moves.begin(); it != moves.end(); ++it, ++i) {            
            st.copyAndMove(*it, child);
            double eval = minimax(child, true, depth-1, d, alpha, beta);
            
            if (eval < beta) {
                beta = eval;
                movInd = i;
            }
            
            if (alpha >= beta) {
                type = UpperBound;
                break;
            }
        }
        
        nodeEval = beta;
    }
    
    return nodeEval;
}















Move MinimaxAI::searchTreeCache(const GameState& st)
{    
    hitNodes = 0;
    numNodes = 0;
    int movInd = -1;
    const QList<Move> moves = st.moves(player);
    
    if (moves.size() == 1) {
        movInd = 0;
        numNodes = 1;
    }
    else if (oneMoveWin(st, moves, movInd)) {
        numNodes = movInd + 1;
    }
    else {
        double eval = minimaxCache(
            st, isMax, lookahead, movInd,
            GameState::evalMin-100, GameState::evalMax+100
        );
        
        if (eval == GameState::evalMin)
            cout << "Defense is going to win!" << endl;
        else if (eval == GameState::evalMax)
            cout << "Attack is going to win!" << endl;
    }
    
    cout << numNodes << " nodes" << endl;
    cout << "hit = " << hitNodes / double(numNodes) << endl;
    cout << "cacheSize = " << cache.size() << endl;
    return moves.at(movInd);
}


double MinimaxAI::minimaxCache(const GameState& st, bool max,
                               int depth, int& movInd,
                               double alpha, double beta)
{   
    ++numNodes;
    
    SearchResult* result = cache.object(st);
    bool validCache = result && (result->depth == depth);
    
    /*if (validCache && result->type == Exact) {
        ++hitNodes;
        movInd = result->movInd;
        return result->eval;
    }*/
    
    if (!depth || st.gameOver()) {
        return st.eval();
    }

    double nodeEval;
    ResultType nodeType = Exact;
    
    movInd = 0;
    int i = 0, d;
    
    GameState child;
    QList<Move>::const_iterator it;
    const QList<Move> moves = st.moves(max ? Attack : Defense);
    
    if (max) {
        /*if (validCache && result->type == LowerBound && result->eval >= beta) {
            movInd = result->movInd;
            return result->eval;
        }*/
        
        for (it = moves.begin(); it != moves.end(); ++it, ++i) {                 
            st.copyAndMove(*it, child);            
            double eval = minimaxCache(child, false, depth-1, d, alpha, beta);
            
            if (eval > alpha) {
                alpha = eval;
                movInd = i;
            }
            
            if (alpha >= beta) {
                nodeType = LowerBound;
                movInd = i;
                break;
            }
        }
        
        nodeEval = alpha;
    }
    else {        
        /*if (validCache && result->type == UpperBound && result->eval <= alpha) {
            movInd = result->movInd;
            return result->eval;
        }*/
        
        for (it = moves.begin(); it != moves.end(); ++it, ++i) {            
            st.copyAndMove(*it, child);
            double eval = minimaxCache(child, true, depth-1, d, alpha, beta);
            
            if (eval < beta) {
                beta = eval;
                movInd = i;
            }
            
            if (alpha >= beta) {
                nodeType = UpperBound;
                movInd = i;
                break;
            }
        }
        
        nodeEval = beta;
    }
    
    if (nodeType == Exact && depth >= cacheDepth) {
        SearchResult* result = cache.object(st);
        
        if (!result ||
            depth > result->depth ||
            nodeType == Exact && result->type != Exact)
        {
            SearchResult* res(new SearchResult(
                depth, movInd, nodeEval, nodeType
            ));
            
            res->st.copy(st);
            res->alpha = alpha;
            res->beta = beta;
            
            cache.insert(st, res);
        }
    }
    
    if (validCache &&
        result->type == Exact &&
        nodeType == Exact &&
        nodeEval != result->eval)
    {
        if (alpha == result->alpha &&
            beta  == result->beta)
            cout << "#### opa ####" << endl;
    }
    
    return nodeEval;
}




























SearchResult::SearchResult() { }

SearchResult::SearchResult(int _d, int _i, double _e, ResultType _t)
        : depth(_d), movInd(_i), eval(_e), type(_t)
{ }


uint qHash(const GameState& st)
{
    return st.hash();
}
