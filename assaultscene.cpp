#include <QGraphicsRectItem>
#include "assaultscene.h"
#include "util.h"


AssaultScene* AssaultScene::instance = NULL;

AssaultScene::AssaultScene()
{
    instance = this;
    characterChanged(NULL, NULL);
}


void AssaultScene::startGame()
{    
    clear();    
    layoutItems(); 
}

void AssaultScene::characterChanged(const QPixmap *defense, const QPixmap *attack)
{
    defensePixmap = defense;
    attackPixmap = attack;
}

void AssaultScene::insertPiece(int i, int j, PlayerType p) {
    PieceItem* item;
    const QPixmap* pic = (p == Attack ? attackPixmap : defensePixmap);
    
    item = new PieceItem(*pic, i, j);
    
    connect(item, SIGNAL(clicked(int, int)), SIGNAL(pieceClicked(int,int)));
    addItem(pieces[i][j] = item);
}

void AssaultScene::movePiece(const Move& m)
{
    if (!m.isDiagonalKill()) {
        int i = m.origin().x(), j = m.origin().y();
        int ni = m.destiny().x(), nj = m.destiny().y();
    
        pieces[ni][nj] = pieces[i][j];
        pieces[ni][nj]->move(ni, nj);
    }
    
    foreach (const QPoint& p, m.kills()) {
        int i = p.x(), j = p.y();
        removeItem(pieces[i][j]);
        delete pieces[i][j];
    }
}

PieceItem* AssaultScene::getPiece(int i, int j)
{
    return pieces[i][j];
}

PieceItem* AssaultScene::getPiece(const QPoint& p)
{
    return getPiece(p.x(), p.y());
}

void AssaultScene::layoutItems()
{    
    for (int i = 0; i < boardSize; ++i) {    
        for (int j = 0; j < boardSize; ++j) {
            if (!isValidSquare(i, j)) continue;
            
            SquareItem *item = new SquareItem(i, j);
            
            connect(item, SIGNAL(clicked(int,int)), SIGNAL(squareClicked(int,int)));
            
            item->setPos(boardToScene(i, j) + QPoint(0, 6));
            addItem(squares[i][j] = item);            
            
            if (!isInsideFortress(i, j))
                insertPiece(i, j, Attack);
        }
    }
}

void AssaultScene::highlightMoves(const QList<Move>& l)
{
    foreach(const Move& m, l) {
        int di = m.destiny().x(), dj = m.destiny().y();
        squares[di][dj]->changeState(SquareItem::Destination);
        
        foreach(const QPoint& p, m.kills()) {
            int ki = p.x(), kj = p.y();
            squares[ki][kj]->changeState(SquareItem::Kill);
        }
    }
}

void AssaultScene::blankMoves(const QList<Move>& l)
{
    foreach(const Move& m, l) {
        int di = m.destiny().x(), dj = m.destiny().y();
        squares[di][dj]->changeState(SquareItem::Blank);
        
        foreach (const QPoint& p, m.kills()) {
            int ki = p.x(), kj = p.y();
            squares[ki][kj]->changeState(SquareItem::Blank);
        }
    }
}

AssaultScene* AssaultScene::getInstance()
{
    return instance;
}
