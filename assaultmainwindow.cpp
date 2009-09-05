#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"
#include "assaultitems.h"
#include "util.h"
#include "game.h"


AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AssaultMainWindow),
      scene(NULL), attack(NULL), defense(NULL)
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
    
    updateDefensePic();
    updateAttackPic();
    
    SquareItem::loadImages();
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

void AssaultMainWindow::setupSceneToPlayer(AssaultScene* scene, Player* p)
{
    connect(scene, SIGNAL(pieceClicked(int,int)),
            p, SIGNAL(pieceClicked(int,int)));
    connect(scene, SIGNAL(squareClicked(int,int)),
            p, SIGNAL(squareClicked(int,int)));
    connect(p, SIGNAL(createPiece(int,int,PlayerType)),
            scene, SLOT(insertPiece(int,int,PlayerType)));
    connect(p, SIGNAL(highlightMoves(const QList<Move>&)),
            scene, SLOT(highlightMoves(const QList<Move>&)));
    connect(p, SIGNAL(blankMoves(const QList<Move>&)),
            scene, SLOT(blankMoves(const QList<Move>&)));
    connect(p, SIGNAL(movePiece(const Move&)),
            scene, SLOT(movePiece(const Move&)));
}

void AssaultMainWindow::setupPlayerToPlayer(Player* attack, Player* defense)
{
    connect(attack, SIGNAL(played()), defense, SIGNAL(opponentPlayed()));
    connect(defense, SIGNAL(played()), attack, SIGNAL(opponentPlayed()));
}

void AssaultMainWindow::startGame()
{
    if (scene) {
        delete scene;
    }
    
    if (attack && defense) {
        delete attack;
        delete defense;
    }
    
    scene = new AssaultScene;
    GameState* state = new GameState;
    state->init();
    
    attack = new HumanPlayer(Attack, state, ui->scoreBoardStatusP1);
    defense = new HumanPlayer(Defense, state, ui->scoreBoardStatusP2);
    
    setupSceneToPlayer(scene, attack);
    setupSceneToPlayer(scene, defense); 
    setupPlayerToPlayer(attack, defense);
    
    ui->scoreBoardPictureP1->setPixmap(*ui->attackPicture->pixmap());
    ui->scoreBoardPictureP2->setPixmap(*ui->defensePicture->pixmap());
    
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
