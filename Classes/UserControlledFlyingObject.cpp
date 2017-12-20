//
//  UserControlledFlyingObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 19/12/2017.
//

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
    
    return true;
}

void UserControlledFlyingObject::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto touchLocation = touch->getLocation();
    
    auto currentPosition = getPosition();
    auto delta = touch->getDelta();
    auto newPosition = currentPosition + delta;
    setPosition(newPosition);
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
}

void UserControlledFlyingObject::onExit()
{
    CollideObject::onExit();
}

UserControlledFlyingObject* UserControlledFlyingObject::create(const std::string &filename)
{
    UserControlledFlyingObject* ucfo = new (std::nothrow) UserControlledFlyingObject();
    ucfo->initWithFile(filename);
    ucfo->initialize();
    ucfo->autorelease();
    
    return ucfo;
}
