#include <QList>
#include <QPoint>
#include <QVariant>
#include <QtSignalEvent>

#include "transitions.h"
#include "game.h"
#include "util.h"


ConditionalSignalTransition::ConditionalSignalTransition(Player* p, const char* sig)
        : QtSignalTransition(p, sig), player(p)
{ }

bool ConditionalSignalTransition::eventTest(QEvent* e) const
{
    if (!QtSignalTransition::eventTest(e))
        return false;
    
    QList<QVariant> l = static_cast<QtSignalEvent*>(e)->arguments();
    
    if (l.size() >= 2)
        return trigger(l.at(0).toInt(), l.at(1).toInt());
    else
        return trigger(-1, -1);
}

bool ConditionalSignalTransition::trigger(int i, int j) const
{
    return true;
}


OpenSquareClicked::OpenSquareClicked(Player* p)
        : ConditionalSignalTransition(p, SIGNAL(squareClicked(int,int)))
{ }

bool OpenSquareClicked::trigger(int i, int j) const
{
    bool open = player->getGameState()->isOpen(i, j);
    
    QList<Move> l = fromVariantList<Move>(
            player->getStateData()->value("destMoves")
    );
    
    bool dest = isDestinySquare(i, j, l);
    
    if (open && !dest) {
        player->getStateData()->insert("selectedSquare", QPoint(i, j));
        return true;
    }
    else {
        return false;
    }
}


SelfSquareClicked::SelfSquareClicked(Player* p)
        : ConditionalSignalTransition(p, SIGNAL(squareClicked(int,int)))
{ }

bool SelfSquareClicked::trigger(int i, int j) const
{    
    SquareT st1 = squareType(player->getType());
    SquareT st2 = player->getGameState()->get(i, j);
    
    if (st1 == st2) {
        player->getStateData()->insert("selectedPiece", QPoint(i, j));
        return true;
    }
    else {
        return false;
    }
}


PieceClicked::PieceClicked(Player* p)
        : ConditionalSignalTransition(p, SIGNAL(pieceClicked(int,int)))
{ }

bool PieceClicked::trigger(int i, int j) const
{
    SquareT st1 = squareType(player->getType());
    SquareT st2 = player->getGameState()->get(i, j);
    
    if (st1 == st2) {
        player->getStateData()->insert("selectedPiece", QPoint(i, j));
        return true;
    }
    else {
        return false;
    }
}


DestDiagonalPieceClicked::DestDiagonalPieceClicked(Player* p)
        : ConditionalSignalTransition(p, SIGNAL(pieceClicked(int,int)))
{ }

bool DestDiagonalPieceClicked::trigger(int i, int j) const
{
    QVariant l = player->getStateData()->value("destMoves");
    QList<Move> moves = fromVariantList<Move>(l);
    
    if (isDestinySquare(i, j, moves)) {
        player->getStateData()->insert("selectedSquare", QPoint(i, j));
        player->getStateData()->insert("destSquareClicked", true);
        return true;
    }
    else {
        return false;
    }
}


DestSquareClicked::DestSquareClicked(Player* p)
        : ConditionalSignalTransition(p, SIGNAL(squareClicked(int,int)))
{ }

bool DestSquareClicked::trigger(int i, int j) const
{
    QVariant l = player->getStateData()->value("destMoves");
    QList<Move> moves = fromVariantList<Move>(l);
    
    if (isDestinySquare(i, j, moves)) {
        player->getStateData()->insert("selectedSquare", QPoint(i, j));
        player->getStateData()->insert("destSquareClicked", true);
        return true;
    }
    else {
        return false;
    }
}
