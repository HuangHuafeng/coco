//
//  ObjectManager.cpp
//  MyGame
//
//  Created by 黄华锋 on 28/12/2017.
//

#include "ObjectManager.h"
#include "FlyingObject.h"
#include "FriendPlane.h"
#include "EnemyPlane.h"
#include "WeaponTripleBullet.h"

ObjectManager * ObjectManager::mInstance;

USING_NS_CC;

ObjectManager::ObjectManager()
{
    mScene = nullptr;
    mBullets = {};
    mWeapons = {};
    mRetainedObjects = {};
    mObjectsInScene = {};
    mObjectsWithId0 = {};
    mFriendPlane = nullptr;
}

ObjectManager::~ObjectManager()
{
    if (mScene) {
        mScene->autorelease();
        mScene = nullptr;
    }
    
    std::vector<GameObject *>::const_iterator itObject = mRetainedObjects.cbegin();
    std::vector<GameObject *>::const_iterator endObject = mRetainedObjects.cend();
    while (itObject != endObject) {
        (*itObject)->autorelease();
        itObject++;
    }
    
    if (mFriendPlane) {
        mFriendPlane->autorelease();
        mFriendPlane = nullptr;
    }
}

void ObjectManager::setScene(cocos2d::Scene *scene)
{
    if (mScene) {
        mScene->autorelease();
        mScene = nullptr;
    }
    
    if (scene) {
        mScene = scene;
        mScene->retain();
    }
}

ObjectManager * ObjectManager::getInstance()
{
    if (mInstance == nullptr) {
        mInstance = new (std::nothrow) ObjectManager();
    }
    
    return mInstance;
}

bool ObjectManager::loadFromFile(const std::string &filename)
{
    std::string sbuf;
    auto status = FileUtils::getInstance()->getContents(filename, &sbuf);
    
    if (status != FileUtils::Status::OK) {
        log("failed to read file %s, status: %d", filename.c_str(), status);
        return false;
    }
    
    const json j_null;
    json game = json::parse(sbuf);
    if (game.type() != json::value_t::object) {
        log("%s is not a valid json file for this game, type %hhu", filename.c_str(), game.type());
        return false;
    }
    
    auto objects = game.value("objects", j_null);
    if (objects.is_null() == false) {
        return loadObjects(objects);
    }
    
    return true;
}

bool ObjectManager::loadObjects(const json &objects)
{
    if (objects.type() != json::value_t::array) {
        log("\"objects\" has type %hhu, it's not an array!", objects.type());
        return false;
    }
    
    json::const_iterator it = objects.cbegin();
    json::const_iterator end = objects.cend();
    while (it != end) {
        if (createObject(*it) == false) {
            log("failed to create object from: %s", (*it).dump(4).c_str());
        }
        it ++;
    }
    
    return true;
}

bool ObjectManager::createObject(const json &object)
{
    if (object.type() != json::value_t::object) {
        log("\"object\" has type %hhu, it's not an object!", object.type());
        return false;
    }
    
    const json j_null;
    auto bornTime = object.value("bornTime", j_null);
    if (bornTime.is_null()) {
        // no bornTime, create the object now
        return createObjectImmediately(object);
    } else {
        // the object will be created after "bornTime" **milliseconds**
        auto delay = DelayTime::create(static_cast<float>(bornTime) * 0.001);
        auto createObject = CallFunc::create(CC_CALLBACK_0(ObjectManager::createObjectImmediately, this, object));
        auto seq = Sequence::create(delay, createObject, nullptr);
        if (mScene) {
            mScene->runAction(seq);
        }
    }
    
    return true;
}

bool ObjectManager::createObjectImmediately(const json &object)
{
    if (object.type() != json::value_t::object) {
        log("\"object\" has type %hhu, it's not an object!", object.type());
        return false;
    }
    
    const json j_null;
    auto type = object.value("type", j_null);
    if (type.is_string() == false) {
        return false;
    }
    
    GameObject * ret = nullptr;
    if ("Bullet" == type) {
        ret = createBullet(object);
    } else if ("Weapon" == type) {
        ret = createWeapon(object);
    } else if ("FriendPlane" == type) {
        ret = createFriendPlane(object);
    } else if ("EnemyPlane" == type) {
        ret = createEnemyPlane(object);
    } else if ("EnemyGenerator" == type) {
        ret = createEnemyGenerator(object);
    } else {
        log("Don't know how to create object from json: %s", object.dump(4).c_str());
    }

    if (ret) {
        // successfully created an object, check if it should be added to scene
        auto addToScene = object.value("addToScene", j_null);
        if (addToScene.is_boolean()) {
            if (static_cast<bool>(addToScene) == true && mScene) {
                mScene->addChild(ret);
            } else {
                // the object is NOT going to be added to the scene, **AS A RULE**
                // ObjectManager retains it as it probably will be used by other objects
                // like a weapon which will be used later when creating WarObject
                // a bullet which will be used later when creating a Weapon
                // a template WarObject which will be used later to create many copies
                ret->retain();
                mRetainedObjects.push_back(ret);
            }
        }
    }
    
    return ret;
}

EnemyGenerator * ObjectManager::createEnemyGenerator(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto triggerInterval = object.value("triggerInterval", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || triggerInterval.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    // triggerInterval is **milliseconds** in json file
    auto enemyGenerator = EnemyGenerator::create(static_cast<float>(triggerInterval) * 0.001);
    enemyGenerator->setObjectId(id);
    enemyGenerator->setObjectName(name);
    enemyGenerator->start();
    
    auto enemyObjectId = object.value("enemyObject", j_null);
    if (enemyObjectId.is_number_unsigned()) {
        auto enemyObject = dynamic_cast<EnemyObject *>(findRetainedObject(enemyObjectId));
        if (enemyObject) {
            enemyGenerator->setObject(enemyObject);
        }
    }
    
    return enemyGenerator;
}

EnemyPlane * ObjectManager::createEnemyPlane(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file = object.value("file", j_null);
    auto speed = object.value("speed", j_null);
    auto health = object.value("health", j_null);
    auto damage = object.value("damage", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || file.is_string() == false || speed.is_number_unsigned() == false || health.is_number_unsigned() == false || damage.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto enemyPlane = EnemyPlane::create(file);
    enemyPlane->setObjectId(id);
    enemyPlane->setObjectName(name);
    enemyPlane->setSpeed(speed);
    enemyPlane->setHealth(health);
    enemyPlane->setDamage(damage);
    
    auto bornX = object.value("bornX", j_null);
    auto bornY = object.value("bornY", j_null);
    auto destinationX = object.value("destinationX", j_null);
    auto destinationY = object.value("destinationY", j_null);
    
    if (bornX.is_number_integer() && bornY.is_number_integer() && destinationX.is_number_integer() && destinationY.is_number_integer()) {
        enemyPlane->setPosition(Vec2(static_cast<float>(bornX), static_cast<float>(bornY)));
        enemyPlane->setDestination(Vec2(static_cast<float>(destinationX), static_cast<float>(destinationY)));
    }
    
    const int weaponId = object.value("weapon", j_null);
    if (weaponId != 0) {
        auto weapon = dynamic_cast<Weapon *>(findRetainedObject(weaponId));
        if (weapon) {
            enemyPlane->setWeapon(weapon);
        }
    }
    
    return enemyPlane;
}

Bullet * ObjectManager::createBullet(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file = object.value("file", j_null);
    auto speed = object.value("speed", j_null);
    auto damage = object.value("damage", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || file.is_string() == false || speed.is_number_unsigned() == false || damage.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto bullet = Bullet::create(file, speed, damage);
    bullet->setObjectId(id);
    bullet->setObjectName(name);
    mBullets.push_back(bullet);
    return bullet;
}

Weapon * ObjectManager::createWeapon(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto triggerInterval = object.value("triggerInterval", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || triggerInterval.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }

    // triggerInterval is **milliseconds** in json file
    auto weapon = Weapon::create(static_cast<float>(triggerInterval) * 0.001);
    weapon->setObjectId(id);
    weapon->setObjectName(name);
    mWeapons.push_back(weapon);
    
    auto bulletId = object.value("bullet", j_null);
    if (bulletId.is_number_unsigned()) {
        auto bullet = dynamic_cast<Bullet *>(findRetainedObject(bulletId));
        if (bullet) {
            weapon->setBullet(bullet);
        }
    }
    
    return weapon;
}

FriendPlane * ObjectManager::createFriendPlane(const json &object)
{
    if (mFriendPlane) {
        // sorry only one friend plane is supported
        return nullptr;
    }
    
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file = object.value("file", j_null);
    auto speed = object.value("speed", j_null);
    auto health = object.value("health", j_null);
    auto damage = object.value("damage", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || file.is_string() == false || speed.is_number_unsigned() == false || health.is_number_unsigned() == false || damage.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    mFriendPlane = FriendPlane::create(file);
    mFriendPlane->setObjectId(id);
    mFriendPlane->setObjectName(name);
    mFriendPlane->setSpeed(speed);
    mFriendPlane->setHealth(health);
    mFriendPlane->setDamage(damage);
    
    auto bornX = object.value("bornX", j_null);
    auto bornY = object.value("bornY", j_null);
    auto destinationX = object.value("destinationX", j_null);
    auto destinationY = object.value("destinationY", j_null);
    
    if (bornX.is_number_integer() && bornY.is_number_integer() && destinationX.is_number_integer() && destinationY.is_number_integer()) {
        mFriendPlane->setPosition(Vec2(static_cast<float>(bornX), static_cast<float>(bornY)));
        mFriendPlane->setDestination(Vec2(static_cast<float>(destinationX), static_cast<float>(destinationY)));
    }
    
    const int weaponId = object.value("weapon", j_null);
    if (weaponId != 0) {
        auto weapon = dynamic_cast<Weapon *>(findRetainedObject(weaponId));
        if (weapon) {
            mFriendPlane->setWeapon(weapon);
            //mFriendPlane->openFire();
        }
    }
    
    return mFriendPlane;
}

/*
GameObject * ObjectManager::findObject(int id) const
{
    GameObject *ret = findBullet(id);
    if (ret) {
        return ret;
    }
    
    ret = findWeapon(id);
    if (ret) {
        return ret;
    }
    
    ret = findEnemy(id);
    if (ret) {
        return ret;
    }
    
    if (mFriendPlane && mFriendPlane->getObjectId() == id) {
        return mFriendPlane;
    }
    
    return nullptr;
}

Weapon * ObjectManager::findWeapon(int id) const
{
    std::vector<Weapon *>::const_iterator itWeapon = mWeapons.cbegin();
    std::vector<Weapon *>::const_iterator endWeapon = mWeapons.cend();
    while (itWeapon != endWeapon) {
        if ((*itWeapon)->getObjectId() == id) {
            return *itWeapon;
        }
        itWeapon++;
    }
    
    return nullptr;
}

Bullet * ObjectManager::findBullet(int id) const
{
    std::vector<Bullet *>::const_iterator itBullet = mBullets.cbegin();
    std::vector<Bullet *>::const_iterator endBullet = mBullets.cend();
    while (itBullet != endBullet) {
        if ((*itBullet)->getObjectId() == id) {
            return *itBullet;
        }
        itBullet++;
    }
    
    return nullptr;
}
*/

void ObjectManager::ObjectEnterScene(GameObject *object)
{
    auto id = object->getObjectId();
    if (id != 0) {
        // AS A RULE, object with id 0 is not important
        // Besides, there will be many objects with id 0 (like bullets), we actually
        // cannot trace them as the objects in mObjectsInScene have unique id
        if (findSceneObject(id) == nullptr) {
            // only add the object if there's no object with "id"
            object->retain();
            mObjectsInScene.push_back(object);
            //log("object with id %d enters, %lu objects in scene", object->getObjectId(), mObjectsInScene.size());
        } else {
            log("try to add object with id %d, but there's already an object with id %d in list. We should NOT reach here!", id, id);
        }
    } else {
        // we still should trace the objects with id 0
        // of course, we cannot trace them by id, we trace them by the pointers
        object->retain();
        mObjectsWithId0.push_back(object);
        //log("add one object with id 0, there're %lu objects with id 0.", mObjectsWithId0.size());
    }
}

void ObjectManager::ObjectExitScene(GameObject *object)
{
    auto id = object->getObjectId();
    if (id != 0) {
        std::list<GameObject *>::const_iterator itObject = mObjectsInScene.cbegin();
        std::list<GameObject *>::const_iterator endObject = mObjectsInScene.cend();
        
        while (itObject != endObject) {
            if ((*itObject)->getObjectId() == object->getObjectId()) {
                mObjectsInScene.remove(*itObject);
                (*itObject)->autorelease();
                //log("object with id %d exits, %lu objects in scene", object->getObjectId(), mObjectsInScene.size());
                break;
            }
            itObject++;
        }
    } else {
        std::list<GameObject *>::const_iterator itObject = mObjectsWithId0.cbegin();
        std::list<GameObject *>::const_iterator endObject = mObjectsWithId0.cend();
        
        while (itObject != endObject) {
            if (*itObject == object) {
                mObjectsWithId0.remove(*itObject);
                (*itObject)->autorelease();
                //log("remove one object with id 0, there're %lu objects with id 0 left.", mObjectsWithId0.size());
                break;
            }
            itObject++;
        }
    }
}

GameObject * ObjectManager::findRetainedObject(int id) const
{
    std::vector<GameObject *>::const_iterator itObject = mRetainedObjects.cbegin();
    std::vector<GameObject *>::const_iterator endObject = mRetainedObjects.cend();
    
    while (itObject != endObject) {
        if ((*itObject)->getObjectId() == id) {
            return *itObject;
        }
        itObject++;
    }
    
    return nullptr;
}


GameObject * ObjectManager::findSceneObject(int id) const
{
    std::list<GameObject *>::const_iterator itObject = mObjectsInScene.cbegin();
    std::list<GameObject *>::const_iterator endObject = mObjectsInScene.cend();
    
    while (itObject != endObject) {
        if ((*itObject)->getObjectId() == id) {
            return *itObject;
        }
        itObject++;
    }
    
    return nullptr;
}

int ObjectManager::giveMeId()
{
    static int id = static_cast<int>(500000 + 1000000.0f * rand_0_1());
    return id++;
}

int ObjectManager::getNumberOfSceneObjects() const
{
    return static_cast<int>(mObjectsInScene.size());
}

void ObjectManager::AddObjectToScene(GameObject *object, int localZOrder)
{
    if (object && mScene) {
        auto lzo = localZOrder;
        if (!lzo) {
            lzo = getNumberOfSceneObjects() + 1;
        }
        mScene->addChild(object, lzo);
    }
}
