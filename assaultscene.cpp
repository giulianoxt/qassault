#include <QGraphicsRectItem>
#include "assaultscene.h"

AssaultScene::AssaultScene()
{
}


void AssaultScene::startGame(const player_t& knights, const player_t& pawns)
{    
    clear();    
    layoutSquareItems();
}

void AssaultScene::layoutSquareItems()
{
    int squareStep = boardSquareSize + boardSquareDistance;
    int offset = -3 * squareStep;
    
    for (int i = 0; i < boardSize; ++i) {
        int pos_i = i * squareStep + offset;
        
        for (int j = 0; j < boardSize; ++j) {
            int pos_j = j * squareStep + offset;
        
            if (!isValidSquare(i, j)) continue;
            
            QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, 
                                           boardSquareSize, boardSquareSize);
            
            item->setBrush(debugColor);
            item->setPos(pos_j, pos_i);
            item->setVisible(false);
            item->setZValue(-1.0);
            
            addItem(squares[i][j] = item);
        }
    }
}
