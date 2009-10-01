#include <QMessageBox>
#include "assaultmainwindow.h"
#include "ui_assaultmainwindow.h"
#include "assaultitems.h"
#include "util.h"
#include "game.h"


AssaultMainWindow::AssaultMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::AssaultMainWindow),
      scene(NULL), state(NULL), attack(NULL), defense(NULL)
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
    
    scoreBoard = ui->scoreBoard;
    scoreBoard->setPlayAgainButton(ui->playAgainButton);
    scoreBoard->setAttackLabels(
      ui->scoreBoardTypeP2,
      ui->scoreBoardWinsP2,
      ui->scoreBoardPiecesP2);
    scoreBoard->setDefenseLabels(
      ui->scoreBoardTypeP1,
      ui->scoreBoardWinsP1,
      ui->scoreBoardPiecesP1);
    
    ui->loadingGif->setMovie(":/resources/resources/loading.gif");
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
    
    connect(this, SIGNAL(gameEnded(PlayerType)),
            scoreBoard, SLOT(gameEnded(PlayerType)));
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

void AssaultMainWindow::setupPlayers()
{
    connect(attack, SIGNAL(played()), this, SLOT(attackTurnEnded()));
    connect(defense, SIGNAL(played()), this, SLOT(defenseTurnEnded()));
    
    connect(this, SIGNAL(attackPlayed()), defense, SIGNAL(opponentPlayed()));
    connect(this, SIGNAL(defensePlayed()), attack, SIGNAL(opponentPlayed()));
    
    connect(attack, SIGNAL(played()), scoreBoard, SLOT(turnEnded()));
    connect(defense, SIGNAL(played()), scoreBoard, SLOT(turnEnded()));
    
    connect(this, SIGNAL(gameEnded(PlayerType)),
            attack, SIGNAL(gameEnded(PlayerType)));
    connect(this, SIGNAL(gameEnded(PlayerType)),
            defense, SIGNAL(gameEnded(PlayerType)));
}

void AssaultMainWindow::startGame()
{    
    if (state) delete state;
    if (attack) delete attack;
    if (defense) delete defense;
    
    scene = new AssaultScene;
    state = new GameState;
    state->init();
    
    if (ui->attackHumanRadioButton->isChecked()) {
        attack = new HumanPlayer(Attack, state,
           ui->scoreBoardStatusP2);
    }
    else {
        attack = new ComputerPlayer(Attack, state,
           ui->scoreBoardStatusP2, ui->loadingGif,
           new MinimaxAI(Attack));
    }
    
    if (ui->defenseHumanRadioButton->isChecked()) {
        defense = new HumanPlayer(Defense, state,
           ui->scoreBoardStatusP1);
    }
    else {
        defense = new ComputerPlayer(Defense, state,
           ui->scoreBoardStatusP1, ui->loadingGif,
           new GreedyAI(Defense));
    }  

    setupPlayers();    
    setupSceneToPlayer(scene, attack);
    setupSceneToPlayer(scene, defense); 

    ui->scoreBoardPictureP1->setPixmap(*ui->defensePicture->pixmap());    
    ui->scoreBoardPictureP2->setPixmap(*ui->attackPicture->pixmap());
    
    ui->boardGraphicsView->setScene(scene);
    scene->characterChanged(ui->defensePicture->pixmap(), ui->attackPicture->pixmap());
    scene->startGame();
    
    scoreBoard->startNewGame(state, attack, defense);
}

void AssaultMainWindow::attackTurnEnded()
{
    turnEnded(Attack);
}

void AssaultMainWindow::defenseTurnEnded()
{
    turnEnded(Defense);
}

void AssaultMainWindow::turnEnded(PlayerType p)
{        
    PlayerType winner;
    if (state->gameOver(winner)) {
        gameOver(winner);
    }
    else {
        if (p == Attack)
            emit attackPlayed();
        else
            emit defensePlayed();
    }
}

void AssaultMainWindow::gameOver(PlayerType p)
{
    emit gameEnded(p);
    
    delete state;    
    attack->deleteLater();
    defense->deleteLater();
    
    state = NULL;
    attack = defense = NULL;
    
    QMessageBox* box(new QMessageBox(this));
    box->setWindowTitle("Round ended");
    box->setText(playerTypeString(p) + " wins!");
    box->setParent(this);
    box->show();
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

Player* AssaultMainWindow::getPlayer(PlayerType p)
{
    return (p == Attack ? attack : defense);
}

Player* AssaultMainWindow::getOponnentPlayer(PlayerType p)
{
    return (p == Defense ? attack : defense);
}
