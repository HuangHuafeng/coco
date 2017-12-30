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
#include "json.hpp" // installed by homebrew in /usr/local/Cellar/nlohmann_json/3.0.0/include/
using json = nlohmann::json;

typedef enum {
    BULLET = 1,
    WEAPON = 2,
    BACKGROUND = 3,
    FRIENDPLANE = 4,
    ENEMYPLANE = 5,
} ObjectType;

class ObjectManager {
private:
    static ObjectManager *mInstance;
    
    cocos2d::Scene *mScene;
    std::vector<Bullet *> mBullets;
    std::vector<Weapon *> mWeapons;
    std::vector<EnemyObject *> mEnemies;
    std::list<GameObject *> mObjectsInScene;
    FriendPlane *mFriendPlane;      // only on friend plane is supported
    
    ObjectManager();
    
    bool loadObjects(const json &objects);
    bool createObject(const json &object);
    bool createObjectImmediately(const json &object);
    
    Bullet * createBullet(const json &object);
    Weapon * createWeapon(const json &object);
    FriendPlane * createFriendPlane(const json &object);
    EnemyPlane * createEnemyPlane(const json &object);
    
public:
    ~ObjectManager();
    
    static ObjectManager * getInstance();
    
    bool loadFromFile(const std::string &filename);
    void setScene(cocos2d::Scene *scene);
    cocos2d::Scene * getScene() const;
    
    GameObject * findObject(int id) const;
    GameObject * findSceneObject(int id) const;
    Weapon * findWeapon(int id) const;
    Bullet * findBullet(int id) const;
    EnemyObject * findEnemy(int id) const;
    void ObjectEnterScene(GameObject *object);
    void ObjectExitScene(GameObject *object);
};

#endif /* ObjectManager_h */
