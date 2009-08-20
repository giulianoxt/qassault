#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"
#include<iostream>
using namespace std;


AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AssaultMainWindow)
{
    ui->setupUi(this);   
}

AssaultMainWindow::~AssaultMainWindow()
{
    delete ui;
}
