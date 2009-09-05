#include "game.h"
#include "util.h"
#include "globals.h"
#include <cstring>
#include <cstdlib>
#include <algorithm>
using std::sort;


bool isValidSquare(int i, int j)
{
    return  (i >= 0 && i < boardSize &&
             j >= 0 && j < boardSize) &&
            !((i <= 1 || i >= boardSize-2) &&
             (j <= 1 || j >= boardSize-2));
}

bool isInsideFortress(int i, int j)
{
    return (i >= 2 && i <= 4 && j >= 2 && j <= 4);
}

bool isDestinySquare(int i, int j, const QList<Move>& l)
{
    QPoint p(i, j);
    
    foreach (const Move& m, l)
        if (m.destiny() == p)
            return true;
    
    return false;
}

SquareT squareType(PlayerType t)
{
    if (t == Attack)
        return AttackPiece;
    else
        return DefensePiece;
}


GameState::GameState()
{ }

GameState::GameState(const GameState& st)
{
    attackSz = st.attackSz;
    defenseSz = st.defenseSz;
    has_def = st.has_def;
    defA = st.defA;
    defB = st.defB;
    movA = st.movA;
    movB = st.movB;
    attackOnFort = st.attackOnFort;
    memcpy(board, st.board, sizeof board);
}

void GameState::init()
{
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            board[i][j] = isInsideFortress(i, j) ? Empty : AttackPiece;
    
    attackSz = 24;
    defenseSz = 0;
    has_def = false;
    defA = defB = QPoint(0, 0);
    movA.clear();
    movB.clear();
    attackOnFort = 0;
}

bool GameState::gameOver() const
{
    PlayerType p;
    return gameOver(p);
}

bool GameState::gameOver(PlayerType& p) const
{
    if (attackOnFort == 9 || (movA.empty() && movB.empty())) {
        p = Attack;
        return true;
    }
    else if (attackSz <= 8) {
        p = Defense;
        return true;
    }
    else {
        return false;
    }
}

bool GameState::isOpen(int i, int j) const
{
    return board[i][j] == Empty;
}

SquareT GameState::get(int i, int j) const
{
    return board[i][j];
}

SquareT GameState::get(const QPoint& p) const
{
    return board[p.x()][p.y()];
}

void GameState::insertDefensePiece(int i, int j)
{
    board[i][j] = DefensePiece;
    
    if (has_def) throw "GameState::insertDefensePiece";
    
    if (defA.isNull())
        defA = QPoint(i, j);
    else if (defB.isNull()) {
        defB = QPoint(i, j);
        has_def = true;
        initRound();
    }
    
    ++defenseSz;
}

void GameState::set(int i, int j, SquareT t)
{
    if (board[i][j] == AttackPiece && isInsideFortress(i,j))
        --attackOnFort;
    
    board[i][j] = t;
    
    if (t == AttackPiece && isInsideFortress(i, j))
        ++attackOnFort;
}

void GameState::set(const QPoint& p, SquareT t)
{
    set(p.x(), p.y(), t);
}

void GameState::move(const Move& m)
{    
    if (!m.isDiagonalKill()) {
        set(m.destiny(), get(m.origin()));
        set(m.origin(), Empty);
    }
    
    foreach (const QPoint& p, m.kills()) set(p, Empty);
    
    attackSz -= m.killSize();

    if (!m.isDiagonalKill()) {
        if (m.origin() == defA)
            defA = m.destiny();
        else if (m.origin() == defB)
            defB = m.destiny();
    }
        
    initRound();
    
    cout << *this << endl;
}

GameState* GameState::copyAndMove(const Move& m) {
    GameState* st = new GameState(*this);
    st->move(m);
    return st;
}

const QList<Move> GameState::moves(int i, int j) const
{    
    if (board[i][j] == AttackPiece)
        return attackMoves(i, j);
    else if (board[i][j] == DefensePiece)
        return defenseMoves(i, j);
    else {
        throw "GameState::moves";
        return QList<Move>();
    }
}
    
const QList<Move> GameState::attackMoves(int i, int j) const
{   
    static const int moves[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    QList<Move> movesL;
    QPoint p(i, j);
    
    for (int m = 0; m < 8; ++m) {
        int ni = i + moves[m][0], nj = j + moves[m][1];
        
        if (isValidSquare(ni, nj) && isOpen(ni, nj))
            movesL.push_back(Move(p, QPoint(ni, nj)));
    }
    
    return movesL;
}

const QList<Move> GameState::defenseMoves(int i, int j) const
{
    const QPoint p = QPoint(i, j);
    
    if (p == defA)
        return movA;
    else if (p == defB)
        return movB;
    else
        throw "GameState::defenseMoves";
}

void GameState::initRound()
{
    if (!has_def) throw "GameState::initRound";
    
    static const int diagM[4][2] = {
        {-1, -1},{-1, 1},{1, 1},{1, -1}
    };
    
    static const int normM[4][2] = {
        {-1, 0},{0, -1},{0, 1},{1, 0}
    };
    
    static const int movesM[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
        {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    
    for (int k = 0; k < 2; ++k) {
        QPoint& p = !k ? defA : defB;
        
        QList<Move> moves;
        
        int i = p.x(), j = p.y();
        
        for (int d = 0; d < 4; ++d) {
            int ni = i + diagM[d][0], nj = j + diagM[d][1];
            
            if (!isValidSquare(ni, nj)) continue;
            if (get(ni, nj) != AttackPiece) continue;
            
            Move m(p, QPoint(ni,nj));
            m.addKill(QPoint(ni, nj));
            moves.push_back(m);
        }
        
        for (int n = 0; n < 4; ++n) {
            int di = normM[n][0], dj = normM[n][1];

            Move m(p, p);
            
            for (int l = 1; ; l += 2) {
                int atI = i + l*di, atJ = j + l*dj;
                int eI = atI + di, eJ = atJ + dj;
                
                if (!isValidSquare(atI,atJ) || !isValidSquare(eI,eJ))
                    break;
                
                if (get(atI,atJ) != AttackPiece || !isOpen(eI, eJ))
                    break;
                
                m.addKill(QPoint(atI,atJ));
            }
            
            if (m.isKill()) {
                int sz = m.killSize();
                m.setDestiny(QPoint(i + 2*sz*di, j + 2*sz*dj));
                moves.push_back(m);
            }
        }
        
        if (!moves.empty()) {
            sort(moves.begin(), moves.end());
            
            while (moves.front() < moves.back())
                moves.pop_front();
        }
        else {
            for (int m = 0; m < 8; ++m) {
                int ni = i + movesM[m][0], nj = j + movesM[m][1];
                
                if (isValidSquare(ni,nj) && isOpen(ni,nj))
                    moves.push_back(Move(p, QPoint(ni, nj)));
            }
        }
        
        if (!k) movA = moves;
        else    movB = moves;
    }
    
    if (movA.empty() || movB.empty()) return;
    
    bool aKill = movA.front().isKill();
    bool bKill = movB.front().isKill();
    
    if (aKill && !bKill)
        movB.clear();
    else if (!aKill && bKill)
        movA.clear();
    else if (aKill && bKill) {        
        if (movA.front() < movB.front())
            movA.clear();
        else if (movB.front() < movA.front())
            movB.clear();
    }
}


ostream& operator<<(ostream& out, const GameState& st)
{
    out << "GameState<" << endl
        << " attackSz = " << st.attackSz << endl
        << " defenseSz = " << st.defenseSz << endl
        << " attacOnFort = " << st.attackOnFort << endl
        << " has_def = " << st.has_def << endl
        << " defA = " << st.defA << endl
        << " movA = " << st.movA << endl
        << " defB = " << st.defB << endl
        << " movB = " << st.movB << endl;
    
    for (int i = 0; i < boardSize; ++i) {
        out << ' ';
        for (int j = 0; j < boardSize; ++j) {
            if (j) out << ' ';
            
            if (!isValidSquare(i, j)) {
                out << ' ';
                continue;
            }
            
            SquareT s = st.board[i][j];
            
            if (s == AttackPiece)
                out << 'A';
            else if (s == DefensePiece)
                out << 'D';
            else if (s == Empty)
                out << 'E';
            else
                out << 'X';
        }
        out << endl;
    }
    
    return out << '>';
}

ostream& operator<<(ostream& out, const Move& m)
{
    return out << "Move<" << m.origin() << " -> " << m.destiny()
               << ", " << m.kills() << ">";
}


Move::Move() { }

Move::Move(const Move& m) : orig(m.orig), dest(m.dest), kill(m.kill)
{ }

Move::Move(const QPoint& a, const QPoint& b) : orig(a), dest(b)
{ }

void Move::addKill(const QPoint& p)
{
    kill.push_back(p);
}

const QPoint& Move::origin() const
{
    return orig;
}

void Move::setOrigin(const QPoint& p)
{
    orig = p;
}

const QPoint& Move::destiny() const
{
    return dest;
}

bool Move::isDiagonalKill() const
{
    int di = dest.x()-orig.x(), dj = dest.y()- orig.y();
    
    return kill.size() && di && dj;
}

void Move::setDestiny(const QPoint& p)
{
    dest = p;
}

bool Move::isKill() const
{
    return !kill.empty();
}

int Move::killSize() const
{
    return kill.size();
}

const QList<QPoint>& Move::kills() const
{
    return kill;
}

bool Move::operator<(const Move& m) const
{
    return kill.size() < m.kill.size();
}
