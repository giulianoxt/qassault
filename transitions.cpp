#include "transitions.h"
#include <QList>
#include <QPoint>
#include <QVariant>
#include <QtSignalEvent>


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
    if (player->getGameState()->isOpen(i, j)) {
        player->getStateData()->insert("selectedSquare", QPoint(i, j));
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
    return true;
}


DestSquareClicked::DestSquareClicked(Player* p)
        : ConditionalSignalTransition(p, SIGNAL(squareClicked(int,int)))
{ }

bool DestSquareClicked::trigger(int i, int j) const
{
    return true;
}
