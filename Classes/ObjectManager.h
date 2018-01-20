//
//  ObjectManager.h
//  MyGame
//
//  Created by 黄华锋 on 28/12/2017.
//

#ifndef ObjectManager_h
#define ObjectManager_h

#include "Bullet.h"
#include "Weapon.h"
#include "FriendPlane.h"
#include "EnemyPlane.h"
#include "EnemyGenerator.h"
#include "FriendPlane.h"
#include "EnemyPlane.h"
#include "MultipleBulletDiffuse.h"
#include "ScrollingBackground.h"

#include "json.hpp" // installed by homebrew in /usr/local/Cellar/nlohmann_json/3.0.0/include/
using json = nlohmann::json;

class GameScene;

typedef enum {
    BULLET = 1,
    WEAPON = 2,
    BACKGROUND = 3,
    FRIENDPLANE = 4,
    ENEMYPLANE = 5,
} ObjectType;

typedef std::function<GameObject *(const json &)> CreatorCallback;

class ObjectManager : public cocos2d::Ref {
private:
    ObjectManager();
    
    GameScene *mScene;
    std::vector<Bullet *> mBullets;
    std::vector<Weapon *> mWeapons;
    std::vector<EnemyObject *> mEnemies;
    std::vector<GameObject *> mRetainedObjects;
    std::map<GameObject *, GameObject *> mObjectsInScene;
    FriendPlane *mPlayerPlane;      // only one player plane is supported
    
    std::map<std::string, CreatorCallback> mTypeCreators;
    
    void initTypeCreators();
    
    bool loadObjects(const json &objects);
    bool createObject(const json &object);
    bool createObjectImmediately(const json &object);
    
    Bullet * createBullet(const json &object);
    Weapon * createWeapon(const json &object);
    MultipleBulletDiffuse * createMultipleBulletDiffuse(const json &object);
    FriendPlane * createFriendPlane(const json &object);
    EnemyPlane * createEnemyPlane(const json &object);
    EnemyGenerator * createEnemyGenerator(const json &object);
    ScrollingBackground * createScrollingBackground(const json &object);
    GameObject * addSceneObject(const json &object);
    GameObject * addPlayerPlane(const json &object);
    GameObject * addBackground(const json &object);
    
public:
    ~ObjectManager();
    
    // ObjectManager should not be a singleton object as every scene
    // has its own object manager. And object manager is created/destroyed by scene
    //static ObjectManager * getInstance();
    
    static ObjectManager * createObjectManager(GameScene *scene);
    CREATE_FUNC(ObjectManager);
    
    virtual bool init();
    
    bool loadFromFile(const std::string &filename);
    bool loadFromJsonString(const std::string &jsonString);
    void setScene(GameScene *scene);

    int giveMeId();
    int getNumberOfSceneObjects() const;
    GameObject * findRetainedObject(int id) const;
    GameObject * findSceneObject(int id) const;
    //Weapon * findWeapon(int id) const;
    //Bullet * findBullet(int id) const;
    //EnemyObject * findEnemy(int id) const;
    void ObjectEnterScene(GameObject *object);
    void ObjectExitScene(GameObject *object);
};

#endif /* ObjectManager_h */
