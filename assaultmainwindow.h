#ifndef ASSAULTMAINWINDOW_H
#define ASSAULTMAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class AssaultMainWindow;
}

class AssaultMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AssaultMainWindow(QWidget *parent = 0);
    ~AssaultMainWindow();

private:
    Ui::AssaultMainWindow *ui;
};

#endif // ASSAULTMAINWINDOW_H
