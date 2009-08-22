#ifndef GLOBALS_H
#define GLOBALS_H

#include <QColor>


// Game logic

enum player_t { Human, PC };

const int boardSize = 7;

bool isValidSquare(uint i, uint j);

bool isInsideFortress(uint i, uint j);


// Game GUI

const QColor debugColor = QColor(0, 255, 0, 180);

const int boardSquareSize = 58;

const int boardSquareDistance = 18;


#endif // GLOBALS_H
