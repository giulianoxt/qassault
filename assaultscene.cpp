#include <QGraphicsRectItem>
#include "assaultscene.h"

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

void AssaultScene::layoutItems()
{    
    for (int i = 0; i < boardSize; ++i) {    
        for (int j = 0; j < boardSize; ++j) {
            if (!isValidSquare(i, j)) continue;
            
            SquareItem *item = new SquareItem(i, j);
            
            connect(item, SIGNAL(clicked(int,int)), SIGNAL(squareClicked(int,int)));
            
            item->setPos(boardToScene(i, j) + QPoint(0, 6));
            addItem(squares[i][j] = item);
            
            if (!isInsideFortress(i, j)) {
                PieceItem* item = new PieceItem(*attackPixmap, i, j);
                
                connect(item, SIGNAL(clicked(int,int)), SIGNAL(pieceClicked(int,int)));
                
                item->setPos(boardToScene(i, j));
                addItem(pieces[i][j] = item);
            }
        }
    }
}
