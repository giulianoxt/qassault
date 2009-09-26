#include <QPoint>
#include <QVector>
#include <QTimer>
#include <QList>
#include "player.h"
#include "util.h"


Player::Player(const PlayerType& t, GameState* state, QObject* obj)
        : type(t), statusObj(obj), gameState(state),
          machine(new QtStateMachine(this))
{
    connect(this, SIGNAL(movePiece(const Move&)), SIGNAL(played()));
    connect(this, SIGNAL(gameEnded(PlayerType)), machine, SLOT(stop()));
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

void Player::move(const Move& m)
{
    gameState->move(m);
    emit movePiece(m);
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
    wait->addTransition(this, SIGNAL(gameEnded(PlayerType)), end);
    play->addTransition(this, SIGNAL(gameEnded(PlayerType)), end);
    
    // selectPiece -> PieceClicked -> selectDest
    PieceClicked* t1(new PieceClicked(this));
    t1->setTargetState(selectDest);
    selectPiece->addTransition(t1);
    
    // selectPiece -> SelfSquareClicked -> selectDest
    SelfSquareClicked* t2(new SelfSquareClicked(this));
    t2->setTargetState(selectDest);
    selectPiece->addTransition(t2);
    
    // selectDest -> PieceClicked -> selectDest
    PieceClicked* t3(new PieceClicked(this));
    t3->setTargetState(selectDest);
    selectDest->addTransition(t3);
    
    // selectDest -> SelfSquareClicked -> selectDest
    SelfSquareClicked* t4(new SelfSquareClicked(this));
    t4->setTargetState(selectDest);
    selectDest->addTransition(t4);
    
    // selectDest -> OpenSquareClicked -> selectPiece
    OpenSquareClicked* t5(new OpenSquareClicked(this));
    t5->setTargetState(selectPiece);
    selectDest->addTransition(t5);
    
    // selectDest -> DestSquareClicked -> wait
    DestSquareClicked* t6(new DestSquareClicked(this));
    t6->setTargetState(wait);
    selectDest->addTransition(t6);
    
    // selectDest -> DestDiagonalPieceClicked -> wait
    DestDiagonalPieceClicked* t7(new DestDiagonalPieceClicked(this));
    t7->setTargetState(wait);
    selectDest->addTransition(t7);
    
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
    connect(this, SIGNAL(highlightMoves(const QList<Move>&)),
            p,    SIGNAL(highlightMoves(const QList<Move>&)));
    connect(this, SIGNAL(blankMoves(const QList<Move>&)),
            p,    SIGNAL(blankMoves(const QList<Move>&)));
    connect(this, SIGNAL(move(const Move&)),
            p,    SLOT(move(const Move&)));
}

void SelectDestState::onEntry()
{
    QPoint piecePos =
       player->getStateData()->value("selectedPiece").toPoint();
    int i = piecePos.x(), j = piecePos.y();
    
    destMoves = player->getGameState()->moves(i, j);
    
    QList<QVariant> l = toVariantList<QList<Move>,Move>(destMoves);
    
    player->getStateData()->insert("destMoves", l);
    
    emit highlightMoves(destMoves);
}

void SelectDestState::onExit()
{
    emit blankMoves(destMoves);
    
    QVariantHash* data = player->getStateData();
    
    data->remove("destMoves");
    
    bool destSelected = data->value("destSquareClicked", false).toBool();
    data->remove("destSquareClicked");
    
    if (destSelected) {
        QPoint p1 = data->value("selectedPiece").toPoint();
        QPoint p2 = data->value("selectedSquare").toPoint();
        
        foreach(const Move& m, destMoves)
            if (m.origin() == p1 && m.destiny() == p2) {
                emit move(m);
                break;
            }
    }
    
    destMoves.clear();
}

        
ChooseStartPos::ChooseStartPos(Player* p, int _n)
        : PlayerState(p), n(_n)
{
    connect(this, SIGNAL(createPiece(int,int,PlayerType)), p,
                  SIGNAL(createPiece(int,int,PlayerType)));
    connect(this, SIGNAL(doneChoosing()), p,
                  SIGNAL(played()));
}

void ChooseStartPos::onExit()
{
   QPoint square = player->getStateData()->value("selectedSquare").toPoint();
   
   int i = square.x(), j = square.y();
   
   emit createPiece(i, j, Defense);
   player->getGameState()->insertDefensePiece(i, j);
   
   if (n == 2) emit doneChoosing();
}


ComputerPlayer::ComputerPlayer(
    const PlayerType& t, GameState* st, QObject* obj, AIPlayer* _ai)
        : Player(t, st, obj), ai(_ai)
{
    QtState* wait(new AIWaitState(this));
    wait->assignProperty(statusObj, "text", "Waiting");
    QtState* end(new EndState(this));
    end->assignProperty(statusObj, "text", "Ended");
    QtState* play(new AIPlayState(this));
    play->assignProperty(statusObj, "text", "Playing");

    wait->addTransition(this, SIGNAL(gameEnded(PlayerType)), end);
    play->addTransition(this, SIGNAL(gameEnded(PlayerType)), end);
    
    wait->addTransition(this, SIGNAL(opponentPlayed()), play);    
    play->addTransition(this, SIGNAL(played()), wait);
    
    machine->addState(wait);
    machine->addState(end);
    machine->addState(play);
    
    if (t == Defense) {
        QtState* choosePos(new AIChoosePosState(this));
        choosePos->assignProperty(statusObj, "text", "Choosing Pos");
        
        choosePos->addTransition(this, SIGNAL(played()), wait);
        choosePos->addTransition(this, SIGNAL(gameEnded(PlayerType)), end);
        
        machine->addState(choosePos);
        machine->setInitialState(choosePos);
    }
    else {
        machine->setInitialState(wait);
    }
    
    machine->start();
}


AIChoosePosState::AIChoosePosState(Player* p) : PlayerState(p)
{
    connect(this, SIGNAL(createPiece(int,int,PlayerType)),
            p, SIGNAL(createPiece(int,int,PlayerType)));
    connect(this, SIGNAL(doneChoosing()),
            p, SIGNAL(played()));
}

void AIChoosePosState::onEntry()
{
    int k = 0;
    GameState* st = player->getGameState();
    
    while (k < 2) {
        int i = randInt(2,4), j = randInt(2,4);
        
        if (st->isOpen(i, j)) {
            ++k;
            st->insertDefensePiece(i, j);
            emit createPiece(i, j, Defense);            
        }
    }
        
    emit doneChoosing();
}


AIWaitState::AIWaitState(Player* p) : PlayerState(p)
{ }


AIPlayState::AIPlayState(Player* p) : PlayerState(p)
{
    connect(this, SIGNAL(highlightMoves(const QList<Move>&)),
            p, SIGNAL(highlightMoves(const QList<Move>&)));
    connect(this, SIGNAL(blankMoves(const QList<Move>&)),
            p, SIGNAL(blankMoves(const QList<Move>&)));
}

void AIPlayState::onEntry()
{    
    GameState* st = player->getGameState();    
    ComputerPlayer* p = dynamic_cast<ComputerPlayer*>(player);
    
    m = p->ai->play(*st);
    
    doHighlight();
    QTimer::singleShot(aiWaitTime, this, SLOT(doMove()));
}

void AIPlayState::doMove()
{
    doBlank();
    player->move(m);
}

void AIPlayState::doHighlight()
{
    QList<Move> l;
    l << m;
    emit highlightMoves(l);
}

void AIPlayState::doBlank()
{
    QList<Move> l;
    l << m;
    emit blankMoves(l);
}
