#ifndef MINIMAX_H
#define MINIMAX_H

#include <QCache>
#include "game.h"
#include "globals.h"

struct SearchResult;

class MinimaxAI
{
public:
    MinimaxAI(PlayerType, int); 
    
    Move searchTree(const GameState&);
    Move searchTreeCache(const GameState&);

protected:
    bool oneMoveWin(const GameState&,
                    const QList<Move>&, int&);
    
    double minimax(const GameState&, bool,
                   int, int&, double, double);
    
    double minimaxCache(const GameState&, bool,
                   int, int&, double, double);
    
protected:    
    bool isMax;
    int hitNodes;
    int numNodes;
    int lookahead;
    PlayerType player;
    QCache<GameState,SearchResult> cache;
};

enum ResultType { Exact, LowerBound, UpperBound };

struct SearchResult {
    int depth;
    int movInd;
    double eval;
    ResultType type;
    
    SearchResult();
    SearchResult(int, int, double, ResultType);
    
    GameState st;
    double alpha, beta;
};

uint qHash(const GameState&);

#endif // MINIMAX_H
