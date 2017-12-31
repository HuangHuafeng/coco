//
//  PlayScene.cpp
//  MyGame
//
//  Created by 黄华锋 on 31/12/2017.
//

#include "PlayScene.h"
#include "ScrollingBackground.h"

USING_NS_CC;

PlayScene * PlayScene::createScene(const std::string &sceneDataInJson)
{
    PlayScene *scene = PlayScene::create();
    scene->setData(sceneDataInJson);
    
    return scene;
}

bool PlayScene::init()
{
    if (Scene::initWithPhysics() == false)
    {
        return false;
    }
    
    //getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    return true;
}

void PlayScene::onFriendPlaneExit()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto localZOrder = 1000;
    
    auto labelGameOver = Label::createWithTTF("Game Over!", "fonts/Marker Felt.ttf", 36);
    if (labelGameOver) {
        // position the label on the center of the screen
        labelGameOver->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height / 2 + labelGameOver->getContentSize().height));
        addChild(labelGameOver, localZOrder);
    }
    
    auto miEnd = MenuItemFont::create("OK", CC_CALLBACK_1(PlayScene::menuStartGameCallback, this));
    miEnd->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(miEnd, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, localZOrder);
}


void PlayScene::menuStartGameCallback(Ref *sender)
{
    Director::getInstance()->popScene();
}
