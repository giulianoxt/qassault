#include "game.h"
#include "util.h"
#include "globals.h"
#include <cstring>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;


bool isValidSquare(int i, int j)
{
    return  (i >= 0 && i < boardSize &&
             j >= 0 && j < boardSize) &&
            !((i <= 1 || i >= boardSize-2) &&
             (j <= 1 || j >= boardSize-2));
}

bool isInsideFortress(int i, int j)
{
    const int minI = fortressBounds[0][0];
    const int maxI = fortressBounds[0][1];
    const int minJ = fortressBounds[1][0];
    const int maxJ = fortressBounds[1][1];
    
    return (i >= minI && i <= maxI && j >= minJ && j <= maxJ);
}

bool isDestinySquare(int i, int j, const QList<Move>& l)
{
    const QPoint p(i, j);
    
    foreach (const Move& m, l)
        if (m.destiny() == p)
            return true;
    
    return false;
}

QString playerTypeString(PlayerType t) {
    if (t == Attack)
        return "Attack";
    else
        return "Defense";
}

SquareT squareType(PlayerType t)
{
    if (t == Attack)
        return AttackPiece;
    else
        return DefensePiece;
}

void GameState::initZobrist()
{
    static const SquareT types[] = {
        AttackPiece, DefensePiece, Empty
    };
    
    foreach_validSquare(i, j) for_(t, 0, 3)
        GameState::zobrist[i][j][(int)types[t]] = rand();
    
    GameState::zobrist[0][0][true] = rand();
    GameState::zobrist[0][0][false] = rand();
}


const double GameState::evalMin = double(INT_MIN);
const double GameState::evalMax = double(INT_MAX);

const int GameState::defenseMovesAll[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
    {0, 1}, {1, -1}, {1, 0}, {1, 1}
};

const int GameState::attackMovesAll[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
    {0, 1}, {1, -1}, {1, 0}, {1, 1}
};

const int GameState::attackMovesForward[5][2] = {
    {-1,0}, {-1,-1}, {-1, 1}, {0,-1}, {0,1}
};

uint GameState::zobrist[boardSize][boardSize][256];


GameState::GameState()
{ }

GameState::GameState(const GameState& st)
{
    copy(st);
}

void GameState::init()
{
    attackSz = 0;
    
    memset(board, Empty, sizeof board);
    
    foreach_validSquare(i, j) if (!isInsideFortress(i,j)) {
        board[i][j] = AttackPiece;
        ++attackSz;
    }
    
    defenseSz = 0;
    defA = defB = QPoint(0, 0);
    movA.clear();
    movB.clear();
    attackOnFort = 0;
}

void GameState::copy(const GameState& st)
{
    max = st.max;
    _hash = st._hash;
    attackSz = st.attackSz;
    defenseSz = st.defenseSz;
    attackOnFort = st.attackOnFort;
    defA = st.defA;
    defB = st.defB;
    movA = st.movA;
    movB = st.movB;
    memcpy(board, st.board, sizeof board);
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
    else if (attackSz < 9) {
        p = Defense;
        return true;
    }
    else {
        return false;
    }
}

bool GameState::almostOver() const
{
    return attackOnFort == 8 || attackSz == 9;
}

bool GameState::isMax() const
{
    return max;
}

uint GameState::attackSize() const
{
    return attackSz;
}

uint GameState::defenseSize() const
{
    return defenseSz;
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
    
    if (defA.isNull())
        defA = QPoint(i, j);
    else if (defB.isNull()) {
        defB = QPoint(i, j);
        max = true;
        initHash();
        initRound();
    }
    
    ++defenseSz;
}

void GameState::set(int i, int j, SquareT newP)
{
    SquareT oldP = board[i][j];
    board[i][j] = newP;
    
    if (oldP == AttackPiece && isInsideFortress(i,j))
        --attackOnFort;
    
    if (newP == AttackPiece && isInsideFortress(i, j))
        ++attackOnFort;
    
    _hash ^= zobrist[i][j][(int)oldP];
    _hash ^= zobrist[i][j][(int)newP];
}

void GameState::set(const QPoint& p, SquareT t)
{
    set(p.x(), p.y(), t);
}

void GameState::move(const Move& m)
{            
    set(m.destiny(), get(m.origin()));
    set(m.origin(), Empty);
    
    foreach (const QPoint& p, m.kills())
        set(p, Empty);
    
    attackSz -= m.killSize();

    if (m.origin() == defA)
        defA = m.destiny();
    else if (m.origin() == defB)
        defB = m.destiny();    
    
    _hash ^= zobrist[0][0][max];
    max = !max;
    _hash ^= zobrist[0][0][max];
    
    initRound();
}

GameState* GameState::copyAndMove(const Move& m) const
{
    GameState* st = new GameState(*this);
    st->move(m);
    return st;
}

void GameState::copyAndMove(const Move& m, GameState& st) const
{
    st.copy(*this);
    st.move(m);
}

const QList<Move> GameState::moves(int i, int j) const
{    
    if (board[i][j] == AttackPiece)
        return attackMoves(i, j);
    else
        return defenseMoves(i, j);
}
 
const QList<Move> GameState::moves(const PlayerType& p) const {    
    QList<Move> moves;
    
    if (p == Defense) {
        moves.append(movA);
        moves.append(movB);
    }
    else {       
        foreach_attackPiece(i, j) {            
            QPoint p(i, j);
            
            if (isInsideFortress(i, j)) for_(m, 0, 8) {
                int ni = i + attackMovesAll[m][0];
                int nj = j + attackMovesAll[m][1];
                
                if (isInsideFortress(ni, nj) && isOpen(ni, nj))
                    moves.push_back(Move(p, QPoint(ni, nj)));
            }
            else for_(m, 0, 5) {
                int ni = i + attackMovesForward[m][0];
                int nj = j + attackMovesForward[m][1];
                
                if (isValidSquare(ni, nj) && isOpen(ni, nj))
                    moves.push_back(Move(p, QPoint(ni, nj)));
            }
        }
    }
    
    return moves;
}

const QList<Move> GameState::attackMoves(int i, int j) const
{   
    QList<Move> moves;
    const QPoint p(i, j);
            
    if (isInsideFortress(i, j)) for_(m, 0, 8) {
        int ni = i + attackMovesAll[m][0];
        int nj = j + attackMovesAll[m][1];
                
        if (isInsideFortress(ni, nj) && isOpen(ni, nj))
            moves.push_back(Move(p, QPoint(ni, nj)));
    }
    else for_(m, 0, 5) {
        int ni = i + attackMovesForward[m][0];
        int nj = j + attackMovesForward[m][1];
                
        if (isValidSquare(ni, nj) && isOpen(ni, nj))
            moves.push_back(Move(p, QPoint(ni, nj)));
    }
    
    return moves;
}

const QList<Move> GameState::defenseMoves(int i, int j) const
{
    const QPoint p(i, j);
    
    if (p == defA)
        return movA;
    else if (p == defB)
        return movB;
    else
        throw "GameState::defenseMoves";
}

void GameState::initHash()
{
    _hash = zobrist[0][0][max];
    
    foreach_validSquare(i, j)
        _hash ^= zobrist[i][j][(int)board[i][j]];
}

void GameState::initRound()
{    
    for (int k = 0; k < 2; ++k) {
        QPoint& p = !k ? defA : defB;
        
        QList<Move> moves;
        
        int i = p.x(), j = p.y();
        
        for_(m, 0, 8) {
            int di = defenseMovesAll[m][0];
            int dj = defenseMovesAll[m][1];

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
            for_(m, 0, 8) {
                int ni = i + defenseMovesAll[m][0];
                int nj = j + defenseMovesAll[m][1];
                
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

double GameState::eval() const
{
    PlayerType p;
    if (gameOver(p))
        return (p == Attack ? evalMax : evalMin);

    // max heuristics [0,1]
    double attackOnFortH = attackOnFort / 9.0l;
    double attackSizeH = attackSz / 24.0l;
    
    double numShieldedH = 0.0l;
    foreach_fortressSquare(i, j) if (board[i][j] == AttackPiece) {
        bool ok = true;
        for_(m, 0, 8) {
            int niA = i + defenseMovesAll[m][0];
            int niB = i - defenseMovesAll[m][0];
            int njA = j + defenseMovesAll[m][1];
            int njB = j - defenseMovesAll[m][1];
            if (isValidSquare(niA, njA) &&
                isValidSquare(niB, njB) &&
                (isOpen(niA, njA) && isOpen(niB, njB) ||
                 (get(niA,njA) == DefensePiece && isOpen(niB,njB) ||
                  get(niB,njB) == DefensePiece && isOpen(niA,njA)))) {
                ok = false; break;
            }
        }
        if (ok) ++numShieldedH;
    }
    numShieldedH /= 9;
    
    // min heuristics [0,1]
    double defenseBlockedH = (!movA.empty() + !movB.empty()) / 2.0l;
    double defenseFortressDistH = 0.0l;
    for_(k, 0, 2) {
        QPoint p = !k ? defA : defB;
        int i = p.x(), j = p.y();
        int di = std::max(0,i-2), dj;
        
        if (j < 2) dj = 2 - j;
        else if (j > 4) dj = j - 4;
        else dj = 0;
        
        defenseFortressDistH += di + dj;
    }
    defenseFortressDistH /= 12;
    
    // scalarization (INT_MIN, INT_MAX)
    return 40 * attackOnFortH   + 30 * attackSizeH + 29 * numShieldedH +
           28 * defenseBlockedH + 20 * defenseFortressDistH;
}

uint GameState::hash() const
{
    return _hash;
}

bool GameState::operator==(const GameState& st) const
{
    if (_hash != st._hash               ||
        max != st.max                   || 
        attackSz != st.attackSz         ||
        attackOnFort != st.attackOnFort ||
        defA != st.defA                 ||
        defB != st.defB)
        return false;
    
    return !memcmp(board, st.board, sizeof board);
}


ostream& operator<<(ostream& out, const GameState& st)
{
    out << "GameState<" << endl
        << " attackSz = " << st.attackSz << endl
        << " defenseSz = " << st.defenseSz << endl
        << " attackOnFort = " << st.attackOnFort << endl
        << " movA = " << st.movA << endl
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

istream& operator>>(istream& in, GameState& st)
{    
    st.movA.clear();
    st.movB.clear();
    st.attackOnFort = 0;
    st.attackSz = 0;
    st.defenseSz = 0;
    st.defA = st.defB = QPoint(0, 0);
    memset(st.board, Empty, sizeof st.board);
    
    foreach_validSquare(i, j) {
        SquareT x; in >> x;
        switch (x) {
            case AttackPiece:
                ++st.attackSz;
                st.set(i, j, AttackPiece);
                break;
            case DefensePiece:
                st.insertDefensePiece(i, j);
                break;
        }
    }
    
    st.initHash();
    st.initRound();
    return in;
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

bool Move::operator==(const Move& m) const
{
    return orig == m.orig && dest == m.dest && kill == m.kill;
}
