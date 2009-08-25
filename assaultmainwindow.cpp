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
    
    origDefensePic.load(":/resources/resources/mage.png");
    origAttackPic.load(":/resources/resources/knight_example_2.png");

    SquareItem::setSpotPic(QPixmap(":/resources/resources/spot.png"));
    
    updateDefensePic();
    updateAttackPic();
}

void AssaultMainWindow::setupConnections()
{
    connect(ui->startGameButton, SIGNAL(clicked()), SLOT(startGame()));
    
    connect(ui->defenseSaturationSlider, SIGNAL(valueChanged(int)), SLOT(updateDefensePic()));
    connect(ui->defenseColorSlider, SIGNAL(valueChanged(int)), SLOT(updateDefensePic()));
    connect(ui->defenseSizeSlider, SIGNAL(valueChanged(int)), SLOT(updateDefensePic()));
    
    connect(ui->attackSaturationSlider, SIGNAL(valueChanged(int)), SLOT(updateAttackPic()));
    connect(ui->attackColorSlider, SIGNAL(valueChanged(int)), SLOT(updateAttackPic()));
    connect(ui->attackSizeSlider, SIGNAL(valueChanged(int)), SLOT(updateAttackPic()));
}

void AssaultMainWindow::startGame()
{    
    scene = new AssaultScene;
    
    ui->boardGraphicsView->setScene(scene);
    scene->characterChanged(ui->defensePicture->pixmap(), ui->attackPicture->pixmap());
    
    scene->startGame();
    
}

void AssaultMainWindow::updateDefensePic()
{
    int newHeight = int(characterHeight * sliderRealValue(*ui->defenseSizeSlider));    
    QPixmap newPic = origDefensePic.scaledToHeight(newHeight, Qt::SmoothTransformation);
    
    changeColors(newPic, ui->defenseColorSlider->value(), ui->defenseSaturationSlider->value());
    
    ui->defensePicture->setPixmap(newPic);
}

void AssaultMainWindow::updateAttackPic()
{    
    int newHeight = int(characterHeight * sliderRealValue(*ui->attackSizeSlider));
    QPixmap newPic = origAttackPic.scaledToHeight(newHeight, Qt::SmoothTransformation);
    
    changeColors(newPic, ui->attackColorSlider->value(), ui->attackSaturationSlider->value());
    
    ui->attackPicture->setPixmap(newPic);
}
