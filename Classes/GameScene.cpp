//
//  GameScene.cpp
//  MyGame
//
//  Created by 黄华锋 on 31/12/2017.
//

#include "GameScene.h"
#include "GameObject.h"
#include "ObjectManager.h"

USING_NS_CC;

GameScene::GameScene()
{
    mDataInJson = "";
    mObjectManager = nullptr;
}

GameScene::~GameScene()
{
    if (mObjectManager) {
        delete mObjectManager;
        mObjectManager = nullptr;
    }
}

void GameScene::setData(const std::string &sceneDataInJson)
{
    if (mObjectManager) {
        delete mObjectManager;
        mObjectManager = nullptr;
    }
    
    mDataInJson = sceneDataInJson;
    if (mDataInJson != "") {
        mObjectManager = new (std::nothrow) ObjectManager(this);
        if (mObjectManager && mObjectManager->loadFromJsonString(mDataInJson)) {
            // we are good
        } else {
            log("failed to create object manager for the scene");
            delete mObjectManager;
            mObjectManager = nullptr;
        }
    }
}

const std::string & GameScene::getData()
{
    return mDataInJson;
}

void GameScene::ObjectExitScene(GameObject *object)
{
    if (mObjectManager) {
        mObjectManager->ObjectExitScene(object);
    }
}

void GameScene::ObjectEnterScene(GameObject *object)
{
    if (mObjectManager) {
        mObjectManager->ObjectEnterScene(object);
    }
}

void GameScene::AddObjectToScene(GameObject *object, int localZOrder)
{
    auto lzo = localZOrder;
    if (lzo == 0 && mObjectManager) {
        lzo = mObjectManager->getNumberOfSceneObjects() + 1;
    }
    addChild(object, lzo);
}

int GameScene::giveMeId()
{
    if (mObjectManager) {
        return mObjectManager->giveMeId();
    } else {
        return 0;
    }
}

void GameScene::onFriendPlaneEnter()
{
}

void GameScene::onFriendPlaneExit()
{
    
}
