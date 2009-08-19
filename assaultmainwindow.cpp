#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"

AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AssaultMainWindow)
{
    ui->setupUi(this);
}

AssaultMainWindow::~AssaultMainWindow()
{
    delete ui;
}
