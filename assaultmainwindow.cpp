#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"
#include<iostream>
using namespace std;


AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AssaultMainWindow)
{
    ui->setupUi(this);
    
    cout << "Construtor" << endl;
    
    cout << ui->label->pixmap()->isNull() << endl;
    
}

AssaultMainWindow::~AssaultMainWindow()
{
    delete ui;
}
