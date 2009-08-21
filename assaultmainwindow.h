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
    
private:
    void setupUi();
    
    Ui::AssaultMainWindow *ui;
};

#endif // ASSAULTMAINWINDOW_H
