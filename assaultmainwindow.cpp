#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"


AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AssaultMainWindow)
{
    setupUi();    
}

AssaultMainWindow::~AssaultMainWindow()
{
    delete ui;
}

void AssaultMainWindow::setupUi()
{    
    ui->setupUi(this);    
    ui->graphicsView->setScene(new AssaultScene);
}
