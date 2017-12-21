//
//  UserControlledFlyingObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 19/12/2017.
//

//#include "base/ccUtils.h"
#include "UserControlledFlyingObject.h"

USING_NS_CC;

UserControlledFlyingObject::UserControlledFlyingObject()
{
}

bool UserControlledFlyingObject::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    stop();
    auto touchLocation = touch->getLocation();
    cocos2d::log("onTouchBegan(): %f,%f", touchLocation.x, touchLocation.y);
    mPreviousTime = utils::gettime();
    
    return true;
}

void UserControlledFlyingObject::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto currentTime = utils::gettime();
    if (mPreviousTime < mTimeLastFrame) {
        // the user does not move the touch for a while, this is not accurate, but good enough
        mPreviousTime = mTimeLastFrame;
    }
    auto touchLocation = touch->getLocation();
    
    auto currentPosition = getPosition();
    auto delta = touch->getDelta();
    delta.normalize();
    auto timeDelta = currentTime - mPreviousTime;
    auto newPosition = keepInsideScreen(currentPosition + mSpeed * delta * timeDelta);
    setPosition(newPosition);
    
    mPreviousTime = currentTime;
    log("mPreviousTime: %f", mPreviousTime);
}

const cocos2d::Vec2 UserControlledFlyingObject::keepInsideScreen(const cocos2d::Vec2& position)
{
    auto modifiedPosition = position;
    auto windowSize = Director::getInstance()->getWinSize();
    if (modifiedPosition.x < 0) {
        modifiedPosition.x = 0;
    }
    if (modifiedPosition.x > windowSize.width) {
        modifiedPosition.x = windowSize.width;
    }
    if (modifiedPosition.y < 0) {
        modifiedPosition.y = 0;
    }
    if (modifiedPosition.y > windowSize.height) {
        modifiedPosition.y = windowSize.height;
    }
    
    return modifiedPosition;
}

void UserControlledFlyingObject::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto currentPosition = getPosition();
    setDestination(currentPosition);
}

void UserControlledFlyingObject::onEnter()
{
    CollideObject::onEnter();
    log("UserControlledFlyingObject::onEnter()");

    // response to touch event
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = CC_CALLBACK_2(UserControlledFlyingObject::onTouchBegan, this);
    eventListener->onTouchMoved = CC_CALLBACK_2(UserControlledFlyingObject::onTouchMoved, this);
    eventListener->onTouchEnded = CC_CALLBACK_2(UserControlledFlyingObject::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    //
    scheduleUpdate();
}

void UserControlledFlyingObject::onExit()
{
    CollideObject::onExit();
}

void UserControlledFlyingObject::update (float delta)
{
    mTimeLastFrame = utils::gettime();
}
