#include "scoreboard.h"
#include <QString>


ScoreBoard::ScoreBoard(QWidget* p = 0)
        : QFrame(p), attackWins(0), defenseWins(0)
{ }

void ScoreBoard::setPlayAgainButton(QPushButton* b)
{
    playAgainButton = b;
}

void ScoreBoard::setAttackLabels(QLabel* a, QLabel* b, QLabel* c)
{
    attackLabels[0] = a;
    attackLabels[1] = b;
    attackLabels[2] = c;
}

void ScoreBoard::setDefenseLabels(QLabel* a, QLabel* b, QLabel* c)
{
    defenseLabels[0] = a;
    defenseLabels[1] = b;
    defenseLabels[2] = c;
}

QString playerString(Player* p) {
    if (dynamic_cast<HumanPlayer*>(p))
        return "Human";
    else
        return "Computer";
}

void ScoreBoard::startNewGame(GameState* st, Player* at, Player* def)
{
    state = st;
    attack = at;
    defense = def;
    
    defenseLabels[0]->setText(playerString(def));
    attackLabels[0]->setText(playerString(at));
    
    playAgainButton->setEnabled(false);
    
    updateStats();
}

void ScoreBoard::turnEnded()
{
    updateStats();
}

void ScoreBoard::gameEnded(PlayerType p)
{    
    if (p == Attack)
        ++attackWins;
    else
        ++defenseWins;

    playAgainButton->setEnabled(true);    
    updateStats();
}

void ScoreBoard::updateStats()
{
    defenseLabels[1]->setText(QString("%1").arg(defenseWins));
    attackLabels[1]->setText(QString("%1").arg(attackWins));    
    defenseLabels[2]->setText(QString("2/2"));
    attackLabels[2]->setText(QString("%1/24").arg(state->attackSize()));
}
