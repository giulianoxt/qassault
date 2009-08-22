#include "globals.h"


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
