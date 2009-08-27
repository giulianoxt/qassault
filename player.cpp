#include "player.h"
#include <QPoint>


Player::Player(const PlayerType& t, GameState* state, QObject* obj)
        : type(t), statusObj(obj), gameState(state),
          machine(new QtStateMachine(this))
{
    
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
    
    PieceClicked* t1(new PieceClicked(this));
    t1->setTargetState(selectDest);
    selectPiece->addTransition(t1);
    
    PieceClicked* t2(new PieceClicked(this));
    t2->setTargetState(selectDest);
    selectDest->addTransition(t2);
    
    OpenSquareClicked* t3(new OpenSquareClicked(this));
    t3->setTargetState(selectPiece);
    selectDest->addTransition(t3);
    
    DestSquareClicked* t4(new DestSquareClicked(this));
    t4->setTargetState(wait);
    selectDest->addTransition(t4);
    
    machine->addState(wait);
    machine->addState(end);
    machine->addState(play);
    play->setInitialState(selectPiece);
    
    if (t == Defense) {
        QtState* chooseStartPos1(new ChooseStartPos(this));
        chooseStartPos1->assignProperty(statusObj, "text", "Start pos (1/2)");
        QtState* chooseStartPos2(new ChooseStartPos(this));
        chooseStartPos2->assignProperty(statusObj, "text", "Start pos (2/2)");
        
        OpenSquareClicked* t1(new OpenSquareClicked(this));
        t1->setTargetState(chooseStartPos2);
        
        OpenSquareClicked* t2(new OpenSquareClicked(this));
        t2->setTargetState(wait);
        
        chooseStartPos1->addTransition(t1);
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
{ }

ChooseStartPos::ChooseStartPos(Player* p) : PlayerState(p)
{
    connect(this, SIGNAL(createPiece(int,int,PlayerType)), p,
            SIGNAL(createPiece(int,int,PlayerType)));
}

void ChooseStartPos::onExit()
{
   QPoint square = player->getStateData()->value("selectedSquare").toPoint();
   emit createPiece(square.x(), square.y(), player->getType());
}
