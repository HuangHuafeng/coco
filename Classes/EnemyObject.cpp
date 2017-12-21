//
//  EnemyObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "EnemyObject.h"

USING_NS_CC;

EnemyObject::EnemyObject()
{
    mSelfDestroy = true;
}

void EnemyObject::setSelfDestroy(bool selfDestroy)
{
    mSelfDestroy = selfDestroy;
}

void EnemyObject::collideWith(CollideObject *otherCollideObject)
{
    removeFromParent();
}

// override updateRoute() to remove myself after reaching the destination
void EnemyObject::updateRoute()
{
    stop();
    
    if (mSpeed > 0) {
        auto currentPosition = getPosition();
        if (currentPosition != mDestination) {
            // calculate the time needed
            auto distance = currentPosition.getDistance(mDestination);
            auto moveTo = MoveTo::create(distance/mSpeed, mDestination);
            moveTo->setTag(ACTION_TAG);
            
            Sequence * seq = nullptr;
            if (mSelfDestroy) {
                auto removeSelf = RemoveSelf::create();
                removeSelf->setTag(ACTION_TAG);
                seq = Sequence::create(moveTo, removeSelf, nullptr);
            } else {
                seq = Sequence::create(moveTo, nullptr);
            }
            runAction(seq);
        }
    }
}
