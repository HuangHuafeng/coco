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
            auto onArriveDestination = CallFunc::create(CC_CALLBACK_0(EnemyObject::onArriveDestination, this));
            auto seq = Sequence::create(moveTo, onArriveDestination, nullptr);
            runAction(seq);
        }
    }
}

// by default, enemy object destroys itself
// 1. is at the destination
// 2. is out of the screen
// subclass should override onArriveDestination() to behave differently
void EnemyObject::onArriveDestination()
{
    auto winSize = Director::getInstance()->getWinSize();
    auto visibleRect = Rect(Vec2(0, 0), winSize);
    auto objectRect = getBoundingBox();
    if(visibleRect.intersectsRect(objectRect) == false) {
        removeFromParent();
    }
}

EnemyObject * EnemyObject::clone() const
{
    // I am not able to clone myself
    return nullptr;
}
