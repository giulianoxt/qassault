#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <QList>
#include <QPoint>
#include <QObject>
#include <QTimeLine>
#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include "util.h"
#include "game.h"
#include "globals.h"
#include "assaultscene.h"

class PieceMovingAnimation : public QObject
{ Q_OBJECT

public:
    PieceMovingAnimation();
    void start(const Move&);
    
signals: 
    void finished();
    
protected:
    QTimeLine timeline;
    QList<QGraphicsItemAnimation*> animations;
};


class FadeOutAnimation : public QGraphicsItemAnimation
{
protected:
    virtual void afterAnimationStep(qreal);
};

#endif // ANIMATIONS_H
