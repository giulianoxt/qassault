#ifndef ASSAULTMAINWINDOW_H
#define ASSAULTMAINWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include "assaultscene.h"
#include "scoreboard.h"
#include "player.h"


namespace Ui { class AssaultMainWindow; }


class AssaultMainWindow : public QWidget
{
    Q_OBJECT

public:
    AssaultMainWindow(QWidget *parent = 0);
    ~AssaultMainWindow();

signals:
    // Output
    void gameEnded(PlayerType);

private slots:
    void startGame();
    void turnEnded();
    void updateAttackPic();
    void updateDefensePic();
    
private:
    void setupUi();
    void setupConnections();
    void setupPlayers();
    void setupSceneToPlayer(AssaultScene*, Player*);
    void gameOver(PlayerType);
    
    Ui::AssaultMainWindow *ui;
    QPixmap origAttackPic, origDefensePic;
    
private:
    AssaultScene* scene;
    ScoreBoard* scoreBoard;
    
    GameState* state;
    Player *attack, *defense;
};

#endif // ASSAULTMAINWINDOW_H
