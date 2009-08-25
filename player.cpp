#include "player.h"


Player::Player(const PlayerType& t, GameState* state, QObject* obj)
        : type(t), statusObj(obj), gameState(state),
          machine(new QtStateMachine(this))
{
    
}


HumanPlayer::HumanPlayer(const PlayerType& t, GameState* state, QObject* obj)
        : Player(t, state, obj)
{
    QtState* wait(new QtState);
    wait->assignProperty(statusObj, "text", "Waiting");
    QtState* end(new QtState);
    end->assignProperty(statusObj, "text", "");
    
    QtState* play(new QtState);
    
    QtState* selectPiece(new QtState(play));
    selectPiece->assignProperty(statusObj, "text", "Select piece");    
    QtState* selectDest(new QtState(play));
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
        QtState* chooseStartPos1(new QtState);
        chooseStartPos1->assignProperty(statusObj, "text", "Start pos (1/2)");
        QtState* chooseStartPos2(new QtState);
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
