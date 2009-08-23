#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"
#include "assaultitems.h"
#include "util.h"


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
    
    origKnightPic.load(":/resources/resources/knight_example.png");
    origPawnPic.load(":/resources/resources/knight_example_2.png");

    SquareItem::setSpotPic(QPixmap(":/resources/resources/spot.png"));
    
    updateKnightPic();
    updatePawnPic();
}

void AssaultMainWindow::setupConnections()
{
    connect(ui->startGameButton, SIGNAL(clicked()), SLOT(startGame()));
    
    connect(ui->knightsColorSlider, SIGNAL(valueChanged(int)), SLOT(updateKnightPic()));
    connect(ui->knightsSizeSlider, SIGNAL(valueChanged(int)), SLOT(updateKnightPic()));
    connect(ui->pawnsColorSlider, SIGNAL(valueChanged(int)), SLOT(updatePawnPic()));
    connect(ui->pawnsSizeSlider, SIGNAL(valueChanged(int)), SLOT(updatePawnPic()));
}

void AssaultMainWindow::startGame()
{
    player_t knights, pawns;
    
    knights = ui->knightsHumanRadioButton->isChecked() ? Human : PC;
    pawns = ui->pawnsHumanRadioButton->isChecked() ? Human : PC;
    
    scene->characterChanged(ui->knightsPicture->pixmap(), ui->pawnsPicture->pixmap());
    scene->startGame(knights, pawns);
}

void AssaultMainWindow::updateKnightPic()
{
    int newHeight = int(characterHeight * sliderRealValue(*ui->knightsSizeSlider));    
    QPixmap newPic = origKnightPic.scaledToHeight(newHeight, Qt::SmoothTransformation);
    
    changeHue(newPic, ui->knightsColorSlider->value());
    
    ui->knightsPicture->setPixmap(newPic);
}

void AssaultMainWindow::updatePawnPic()
{    
    int newHeight = int(characterHeight * sliderRealValue(*ui->pawnsSizeSlider));
    QPixmap newPic = origPawnPic.scaledToHeight(newHeight, Qt::SmoothTransformation);
    
    changeHue(newPic, ui->pawnsColorSlider->value());
    
    ui->pawnsPicture->setPixmap(newPic);
}
