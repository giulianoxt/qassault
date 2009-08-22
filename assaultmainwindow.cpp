#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"


AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AssaultMainWindow)
{
    setupUi();
    setupConnections();
}

AssaultMainWindow::~AssaultMainWindow()
{
    delete ui;
}

void AssaultMainWindow::setupUi()
{    
    ui->setupUi(this);    
    
    ui->boardGraphicsView->setStyleSheet("background: transparent");
    ui->boardGraphicsView->setScene(scene = new AssaultScene);
}

void AssaultMainWindow::setupConnections()
{
    connect(ui->startGameButton, SIGNAL(clicked()), SLOT(startGame()));
}

void AssaultMainWindow::startGame()
{
    player_t knights, pawns;
    
    knights = ui->knightsHumanRadioButton->isChecked() ? Human : PC;
    pawns = ui->pawnsHumanRadioButton->isChecked() ? Human : PC;
    
    scene->startGame(knights, pawns);    
}
