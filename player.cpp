#include "player.h"


Player::Player(const PlayerType& t, GameState* state)
        : type(t), gameState(state), machine(new QtStateMachine(this))
{
    
}


HumanPlayer::HumanPlayer(const PlayerType& t, GameState* state)
        : Player(t, state)
{
    QtState* wait(new QtState);
    QtState* end(new QtState);
    QtState* play(new QtState);
    QtState* selectPiece(new QtState(play));
    QtState* selectDest(new QtState(play));
    
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
        QtState* chooseStartPos2(new QtState);
        
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
}
