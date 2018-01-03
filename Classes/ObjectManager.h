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
#include "WeaponTripleBullet.h"
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

typedef std::function<GameObject *(const json &)> creatorCallback;

class ObjectManager {
private:    
    GameScene *mScene;
    std::vector<Bullet *> mBullets;
    std::vector<Weapon *> mWeapons;
    //std::vector<FriendPlane *> mFriendPlanes;
    std::vector<EnemyObject *> mEnemies;
    std::vector<GameObject *> mRetainedObjects;
    std::list<GameObject *> mObjectsInScene;
    std::list<GameObject *> mObjectsWithId0;
    FriendPlane *mPlayerPlane;      // only one player plane is supported
    
    std::map<std::string, creatorCallback> mTypeCreators;
    
    void initTypeCreators();
    
    bool loadObjects(const json &objects);
    bool createObject(const json &object);
    bool createObjectImmediately(const json &object);
    
    Bullet * createBullet(const json &object);
    Weapon * createWeapon(const json &object);
    WeaponTripleBullet * createWeaponTripleBullet(const json &object);
    FriendPlane * createFriendPlane(const json &object);
    EnemyPlane * createEnemyPlane(const json &object);
    EnemyGenerator * createEnemyGenerator(const json &object);
    ScrollingBackground * createScrollingBackground(const json &object);
    GameObject * addSceneObject(const json &object);
    GameObject * addPlayerPlane(const json &object);
    
public:
    ObjectManager(GameScene *scene);
    ~ObjectManager();
    
    // ObjectManager should not be a singleton object as every scene
    // has its own object manager. And object manager is created/destroyed by scene
    //static ObjectManager * getInstance();
    
    bool loadFromFile(const std::string &filename);
    bool loadFromJsonString(const std::string &jsonString);

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
