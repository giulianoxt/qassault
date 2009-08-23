#include <QGraphicsRectItem>
#include "assaultscene.h"

AssaultScene::AssaultScene()
{
    characterChanged(NULL, NULL);
}


void AssaultScene::startGame(const player_t& knights, const player_t& pawns)
{    
    clear();    
    layoutItems(); 
}

void AssaultScene::characterChanged(const QPixmap *knight, const QPixmap *pawn)
{
    knightPixmap = knight;
    pawnPixmap = pawn;
}

void AssaultScene::layoutItems()
{    
    for (int i = 0; i < boardSize; ++i) {    
        for (int j = 0; j < boardSize; ++j) {
            if (!isValidSquare(i, j)) continue;
            
            SquareItem *item = new SquareItem(i, j);
            
            item->setPos(boardToScene(i, j) + QPoint(0, 6));
            addItem(squares[i][j] = item);
            
            if (!isInsideFortress(i, j)) {
                PieceItem* item = new PieceItem(*pawnPixmap, i, j);
                item->setPos(boardToScene(i, j));
                addItem(pieces[i][j] = item);
            }
        }
    }
}
