#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <QtSignalTransition>
#include "player.h"

class Player;


class ConditionalSignalTransition : public QtSignalTransition
{
public:
    ConditionalSignalTransition(Player*, const char*);
    
protected:
    Player* player;
    bool eventTest(QEvent*) const;
    virtual bool trigger(int, int) const;
};


class OpenSquareClicked : public ConditionalSignalTransition
{
public:
    OpenSquareClicked(Player*);
protected:
    bool trigger(int, int) const;
};

class SelfSquareClicked : public ConditionalSignalTransition
{
public:
    SelfSquareClicked(Player*);
protected:
    bool trigger(int, int) const;
};

class PieceClicked : public ConditionalSignalTransition
{
public:
    PieceClicked(Player*);
protected:
    bool trigger(int, int) const;
};

class DestSquareClicked : public ConditionalSignalTransition
{
public:
    DestSquareClicked(Player*);
protected:
    bool trigger(int, int) const;
};


#endif // TRANSITIONS_H
