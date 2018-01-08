//
//  PlayScene.cpp
//  MyGame
//
//  Created by 黄华锋 on 31/12/2017.
//

#include "PlayScene.h"
#include "ScrollingBackground.h"
#include "HealthIndicator.h"
#include "FriendPlane.h"
#include "EnemyPlane.h"

USING_NS_CC;

PlayScene::PlayScene()
{
    log("PlayScene::PlayScene()");
    
}

PlayScene::~PlayScene()
{
    log("PlayScene::~PlayScene()");
}

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

void PlayScene::onPlayerPlaneExit(FriendPlane * friendPlane)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto localZOrder = 100000;
    
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

void PlayScene::ObjectEnterScene(GameObject *object)
{
    GameScene::ObjectEnterScene(object);
    
    if (dynamic_cast<WarObject *>(object)) {
        auto enemyObject = dynamic_cast<EnemyObject *>(object);
        if (enemyObject) {
            HealthIndicator::createHealthIndicatorForObject(enemyObject);
            //enemyObject->setRotation(360.0f * rand_0_1());
        } else {
            auto friendPlane = dynamic_cast<FriendPlane *>(object);
            if (friendPlane) {
                auto hi = HealthIndicator::createHealthIndicatorForObject(friendPlane);
                if (hi) {
                    auto windowSize = Director::getInstance()->getWinSize();
                    const auto barHeight = windowSize.height / HealthIndicator::HEIGHT_WIDTH_RATIO;
                    hi->dontMoveWithLinkedObject(Rect(0, windowSize.height - barHeight, windowSize.width, barHeight));
                    addChild(hi, 10000);
                }
            } else {
                assert(false);
            }
        }
        
    }
    
}
