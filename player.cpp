#include <QPoint>
#include <QVector>
#include "player.h"
#include "util.h"


Player::Player(const PlayerType& t, GameState* state, QObject* obj)
        : type(t), statusObj(obj), gameState(state),
          machine(new QtStateMachine(this))
{
    connect(this, SIGNAL(movePiece(int,int,int,int)), SIGNAL(played()));
}

PlayerType Player::getType()
{
    return type;
}

GameState* Player::getGameState()
{
    return gameState;
}

QVariantHash* Player::getStateData()
{
    return &stateData;
}

void Player::move(int i, int j, int ni, int nj)
{
    gameState->move(i, j, ni, nj);
    emit movePiece(i, j, ni, nj);
}


HumanPlayer::HumanPlayer(const PlayerType& t, GameState* state, QObject* obj)
        : Player(t, state, obj)
{
    QtState* wait(new WaitState(this));
    wait->assignProperty(statusObj, "text", "Waiting");
    QtState* end(new EndState(this));
    end->assignProperty(statusObj, "text", "");
    
    QtState* play(new PlayState(this));
    
    QtState* selectPiece(new SelectPieceState(this, play));
    selectPiece->assignProperty(statusObj, "text", "Select piece");    
    QtState* selectDest(new SelectDestState(this, play));
    selectDest->assignProperty(statusObj, "text", "Play");
    
    wait->addTransition(this, SIGNAL(opponentPlayed()), play);
    wait->addTransition(this, SIGNAL(gameEnded()), end);
    play->addTransition(this, SIGNAL(gameEnded()), end);
    
    // selectPiece -> PieceClicked -> selectDest
    PieceClicked* t1(new PieceClicked(this));
    t1->setTargetState(selectDest);
    selectPiece->addTransition(t1);
    
    // selectDest -> PieceClicked -> selectDest
    PieceClicked* t2(new PieceClicked(this));
    t2->setTargetState(selectDest);
    selectDest->addTransition(t2);
    
    // selectDest -> OpenSquareClicked -> selectPiece
    OpenSquareClicked* t3(new OpenSquareClicked(this));
    t3->setTargetState(selectPiece);
    selectDest->addTransition(t3);
    
    // selectDest -> DestSquareClicked -> wait
    DestSquareClicked* t4(new DestSquareClicked(this));
    t4->setTargetState(wait);
    selectDest->addTransition(t4);
    
    machine->addState(wait);
    machine->addState(end);
    machine->addState(play);
    play->setInitialState(selectPiece);
    
    if (t == Defense) {
        QtState* chooseStartPos1(new ChooseStartPos(this, 1));
        chooseStartPos1->assignProperty(statusObj, "text", "Start pos (1/2)");
        QtState* chooseStartPos2(new ChooseStartPos(this, 2));
        chooseStartPos2->assignProperty(statusObj, "text", "Start pos (2/2)");
        
        // chooseStartPos1 -> OpenSquareClicked -> chooseStartPos2
        OpenSquareClicked* t1(new OpenSquareClicked(this));
        t1->setTargetState(chooseStartPos2);
        chooseStartPos1->addTransition(t1);
        
        // chooseStartPos2 -> OpenSquareClicked -> wait
        OpenSquareClicked* t2(new OpenSquareClicked(this));
        t2->setTargetState(wait);
        chooseStartPos2->addTransition(t2);
        
        machine->addState(chooseStartPos1);
        machine->addState(chooseStartPos2);
        machine->setInitialState(chooseStartPos1);
    }
    else {
        machine->setInitialState(wait);
    }
    
    machine->start();
}


PlayerState::PlayerState(Player* p, QtState* st = 0)
        : QtState(st), player(p)
{ }


WaitState::WaitState(Player* p) : PlayerState(p)
{ }


EndState::EndState(Player* p) : PlayerState(p)
{ }


PlayState::PlayState(Player* p) : PlayerState(p)
{ }


SelectPieceState::SelectPieceState(Player* p, QtState* st)
        : PlayerState(p, st)
{ }


SelectDestState::SelectDestState(Player* p, QtState* st)
        : PlayerState(p, st)
{
    connect(this, SIGNAL(highlightSquares(const QVector<QPoint>)),
            p, SIGNAL(highlightSquares(const QVector<QPoint>)));
    connect(this, SIGNAL(blankSquares(const QVector<QPoint>)),
            p, SIGNAL(blankSquares(const QVector<QPoint>)));
    connect(this, SIGNAL(move(int,int,int,int)),
            p, SLOT(move(int,int,int,int)));
}

void SelectDestState::onEntry()
{
    QPoint piecePos = player->getStateData()->value("selectedPiece").toPoint();
    int i = piecePos.x(), j = piecePos.y();
    
    chosen = player->getGameState()->moves(i, j);
    
    QList<QVariant> l = toVariantList<QVector<QPoint>, QPoint>(chosen);
    
    player->getStateData()->insert("destSquares", l);
    
    emit highlightSquares(chosen);
}

void SelectDestState::onExit()
{
    emit blankSquares(chosen);
    chosen.clear();
    
    QVariantHash* data = player->getStateData();
    
    data->remove("destSquares");
    
    bool destSelected = data->value("destSquareClicked", false).toBool();
    data->remove("destSquareClicked");
    
    if (destSelected) {
        QPoint p1 = data->value("selectedPiece").toPoint();
        QPoint p2 = data->value("selectedSquare").toPoint();
        
        emit move(p1.x(), p1.y(), p2.x(), p2.y());
    }
}

        
ChooseStartPos::ChooseStartPos(Player* p, int _n)
        : PlayerState(p), n(_n)
{
    connect(this, SIGNAL(createPiece(int,int,PlayerType)), p,
            SIGNAL(createPiece(int,int,PlayerType)));
    connect(this, SIGNAL(doneChoosing()), p, SIGNAL(played()));
}

void ChooseStartPos::onExit()
{
   QPoint square = player->getStateData()->value("selectedSquare").toPoint();
   
   int i = square.x(), j = square.y();
   PlayerType type = player->getType();
   
   emit createPiece(i, j, type);
   player->getGameState()->set(i, j, squareType(type));
   
   if (n == 2) emit doneChoosing();
}
