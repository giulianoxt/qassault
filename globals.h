#ifndef GLOBALS_H
#define GLOBALS_H

#include <QColor>


// Game logic

enum player_t { Human, PC };

const int boardSize = 7;

bool isValidSquare(int i, int j);

bool isInsideFortress(int i, int j);


// Game GUI

const QColor debugColor = QColor(0, 255, 0, 180);

const int boardSquareSize = 58;

const int boardSquareDistance = 18;

const int characterHeight = 82;


#endif // GLOBALS_H
