#include <QGraphicsRectItem>
#include "assaultscene.h"
#include "util.h"


AssaultScene::AssaultScene()
{
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

void AssaultScene::movePiece(int i, int j, int ni, int nj)
{
    pieces[ni][nj] = pieces[i][j];
    pieces[ni][nj]->move(ni, nj);
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

void AssaultScene::highlightSquares(const QVector<QPoint> v)
{
    foreach(const QPoint& p, v) {
        int i = p.x(), j = p.y();
        squares[i][j]->changeState(SquareItem::Destination);
    }
}

void AssaultScene::blankSquares(const QVector<QPoint> v)
{
    foreach(const QPoint& p, v) {
        int i = p.x(), j = p.y();
        squares[i][j]->changeState(SquareItem::Blank);
    }
}
