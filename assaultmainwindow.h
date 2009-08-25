#ifndef ASSAULTMAINWINDOW_H
#define ASSAULTMAINWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include "assaultscene.h"

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
    
    Ui::AssaultMainWindow *ui;
    AssaultScene *scene;
    
    QPixmap origAttackPic, origDefensePic;
};

#endif // ASSAULTMAINWINDOW_H
