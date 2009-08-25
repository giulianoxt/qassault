#ifndef ASSAULTMAINWINDOW_H
#define ASSAULTMAINWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include "assaultscene.h"
#include "player.h"

namespace Ui
{
    class AssaultMainWindow;
}

class AssaultMainWindow : public QWidget
{
    Q_OBJECT

public:
    AssaultMainWindow(QWidget *parent = 0);
    ~AssaultMainWindow();
    
private slots:
    void startGame();
    void updateAttackPic();
    void updateDefensePic();
    
private:
    void setupUi();
    void setupConnections();
    void setupSceneToPlayer(AssaultScene*, Player*);
    
    Ui::AssaultMainWindow *ui;
    QPixmap origAttackPic, origDefensePic;
    
private:
    AssaultScene *scene;
    Player *attack, *defense;
};

#endif // ASSAULTMAINWINDOW_H
