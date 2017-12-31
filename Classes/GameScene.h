//
//  GameScene.h
//  MyGame
//
//  Created by 黄华锋 on 31/12/2017.
//

#ifndef GameScene_h
#define GameScene_h

// GameScene is the base class of all scenes in the game
// it provide interface like getObjectManager(), and more if needed

class ObjectManager;

class GameObject;

class GameScene : public cocos2d::Scene {
    
protected:
    GameScene();
    
    std::string mDataInJson;
    ObjectManager *mObjectManager;
    
public:
    ~GameScene();
    
    void setData(const std::string &sceneDataInJson);
    const std::string & getData();
    void ObjectEnterScene(GameObject *object);
    void ObjectExitScene(GameObject *object);
    void AddObjectToScene(GameObject *object, int localZOrder = 0);
    int giveMeId();
    
    virtual void onFriendPlaneEnter();
    virtual void onFriendPlaneExit();
};

#endif /* GameScene_h */
