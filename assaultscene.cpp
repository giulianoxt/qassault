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
    int squareStep = boardSquareSize + boardSquareDistance;
    int offset = -3 * squareStep;
    
    for (int i = 0; i < boardSize; ++i) {
        int pos_i = i * squareStep + offset;
    
        for (int j = 0; j < boardSize; ++j) {           
            int pos_j = j * squareStep + offset;
        
            if (!isValidSquare(i, j)) continue;
            
            SquareItem *item = new SquareItem(i, j);
            item->setPos(pos_j, pos_i);            
            addItem(squares[i][j] = item);
            
            if (!isInsideFortress(i, j)) {
                PieceItem* item = new PieceItem(*pawnPixmap);
                item->setPos(pos_j, pos_i);
                addItem(pieces[i][j] = item);
            }
        }
    }
}
