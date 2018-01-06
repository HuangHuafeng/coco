//
//  GameScene.cpp
//  MyGame
//
//  Created by 黄华锋 on 31/12/2017.
//

#include "GameScene.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "FriendPlane.h"

USING_NS_CC;

GameScene::GameScene()
{
    mDataInJson = "";
    mObjectManager = nullptr;
}

GameScene::~GameScene()
{
    releaseObjectManager();
}

bool GameScene::setData(const std::string &sceneDataInJson)
{
    if (mObjectManager) {
        mObjectManager->autorelease();
        //delete mObjectManager;
        mObjectManager = nullptr;
    }
    
    mDataInJson = sceneDataInJson;
    if (mDataInJson != "") {
        mObjectManager = ObjectManager::createObjectManager(this);
        if (mObjectManager) {
            // we are good
            if (mObjectManager->loadFromJsonString(mDataInJson)) {
                mObjectManager->retain();
            } else {
                // nothing to do, mObjectManager will be auto released as it is not retained here
                return false;
            }
        } else {
            // nothing to do
            return false;
        }
    }
    
    return true;
}

void GameScene::releaseObjectManager()
{
    if (mObjectManager) {
        mObjectManager->autorelease();
        mObjectManager = nullptr;
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
    // DEBUG
    //if (mObjectManager->getNumberOfSceneObjects() > 2000) {
    //    return;
    //}
    // DEBUG END
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

void GameScene::onPlayerPlaneEnter(FriendPlane * friendPlane)
{
}

void GameScene::onPlayerPlaneExit(FriendPlane * friendPlane)
{

}


