//
//  GameObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "GameObject.h"
#include "GameScene.h"

USING_NS_CC;

GameObject::GameObject()
{
    mId = 0;
    mName = "";
}

GameObject::~GameObject()
{
    mId = 0;
    mName = "";
}


const std::string & GameObject::getName()
{
    return mName;
}

void GameObject::setObjectName(const std::string & name)
{
    mName = name;
}

const std::string & GameObject::getObjectName()
{
    return mName;
}

int GameObject::getObjectId() const
{
    return mId;
}

void GameObject::setObjectId(int id)
{
    mId = id;
}

void GameObject::onExit()
{
    auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (gameScene) {
        gameScene->ObjectExitScene(this);
    }
    
    Sprite::onExit();
}

void GameObject::onEnter()
{
    Sprite::onEnter();
    
    auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (gameScene && getParent() == gameScene) {
        // when this game object is NOT a child (for example, a weapon attached to a war object)
        gameScene->ObjectEnterScene(this);
    }
}

GameObject * GameObject::clone() const
{
    // I am not able to clone myself
    return nullptr;
}
