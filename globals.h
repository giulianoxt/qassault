#ifndef GLOBALS_H
#define GLOBALS_H

#include <QColor>


// consts

const QColor debugColor = QColor(0, 255, 0, 180);

const int boardSize = 7;

const int boardSquareSize = 58;

const int boardSquareDistance = 18;

const int characterHeight = 82;

const int fortressBounds[2][2] = {{0,2},{2,4}};

// variables

extern int aiWaitTime;

extern int movingAnimationDuration;

extern int moveLookaheads;

#endif // GLOBALS_H
