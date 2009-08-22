#include "globals.h"


bool isValidSquare(uint i, uint j)
{
    return !((i <= 1 || i >= boardSize-2) &&
             (j <= 1 || j >= boardSize-2));
}

bool isInsideFortress(uint i, uint j)
{
    return (i >= 2 && i <= 4 && j >= 2 && j <= 4);
}
