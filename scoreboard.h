#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include "game.h"
#include "player.h"


class ScoreBoard : public QFrame
{
    Q_OBJECT
    
public:
    ScoreBoard(QWidget*);    
    
    void setPlayAgainButton(QPushButton*);
    void setAttackLabels(QLabel*, QLabel*, QLabel*);
    void setDefenseLabels(QLabel*, QLabel*, QLabel*);

public slots:
    void startNewGame(GameState*, Player*, Player*);
    void turnEnded();
    void gameEnded(PlayerType);
    
private:
    void updateStats();
    
    uint attackWins, defenseWins;
    
    QLabel* attackLabels[3];
    QLabel* defenseLabels[3];
    QPushButton* playAgainButton;
    
    GameState* state;
    Player *attack, *defense;
};

#endif // SCOREBOARD_H
