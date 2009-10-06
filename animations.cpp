#include "animations.h"


PieceMovingAnimation::PieceMovingAnimation()
{
    timeline.setFrameRange(0, 100);
    timeline.setDuration(movingAnimationDuration);
    connect(&timeline, SIGNAL(finished()), this, SIGNAL(finished()));
}

void PieceMovingAnimation::start(const Move& move)
{
    foreach (QGraphicsItemAnimation* anim, animations)
        delete anim;
    animations.clear();
    timeline.stop();
    
    AssaultScene* scene = AssaultScene::getInstance();
    
    QGraphicsItem* movingPiece = scene->getPiece(move.origin());
        
    QGraphicsItemAnimation* anim(new QGraphicsItemAnimation);
    anim->setItem(movingPiece);
    anim->setTimeLine(&timeline);
    anim->setPosAt(0, movingPiece->pos());
    anim->setPosAt(1, boardToScene(move.destiny()));
        
    animations.push_back(anim);
    
    foreach (const QPoint& p, move.kills()) {
        QGraphicsItem* killedPiece = scene->getPiece(p);
        
        QGraphicsItemAnimation* anim(new FadeOutAnimation);
        anim->setItem(killedPiece);
        anim->setTimeLine(&timeline);     
        
        animations.push_back(anim);
    }
    
    timeline.start();
}

void FadeOutAnimation::afterAnimationStep(qreal step)
{
    item()->setOpacity(1 - step);
}
