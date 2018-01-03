//
//  ObjectManager.cpp
//  MyGame
//
//  Created by 黄华锋 on 28/12/2017.
//

#include "ObjectManager.h"
#include "GameScene.h"

USING_NS_CC;

ObjectManager::ObjectManager(GameScene *scene)
{
    mBullets = {};
    mWeapons = {};
    //mFriendPlanes = {};
    mRetainedObjects = {};
    mObjectsInScene = {};
    mObjectsWithId0 = {};
    mPlayerPlane = nullptr;
    
    mScene = scene;
    if (mScene) {
        mScene->retain();
    }
    
    initTypeCreators();
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
}

void ObjectManager::initTypeCreators()
{
    mTypeCreators.clear();
    mTypeCreators.insert(std::make_pair("Bullet", std::bind(&ObjectManager::addSceneObject, this, std::placeholders::_1)));
}


/*
ObjectManager * ObjectManager::getInstance()
{
    if (mInstance == nullptr) {
        mInstance = new (std::nothrow) ObjectManager();
    }
    
    return mInstance;
}
*/

bool ObjectManager::loadFromFile(const std::string &filename)
{
    std::string sbuf;
    auto status = FileUtils::getInstance()->getContents(filename, &sbuf);
    
    if (status != FileUtils::Status::OK) {
        log("failed to read file %s, status: %d", filename.c_str(), status);
        return false;
    }
    
    return loadFromJsonString(sbuf);
}

bool ObjectManager::loadFromJsonString(const std::string &jsonString)
{
    const json j_null;
    json sceneData = json::parse(jsonString);
    if (sceneData.type() != json::value_t::object) {
        log("%s is not a valid json string for this game, type %hhu", jsonString.c_str(), sceneData.type());
        return false;
    }
    
    auto objects = sceneData.value("objects", j_null);
    if (objects.is_null() == false) {
        return loadObjects(objects);
    }
    
    return false;
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
        // no bornTime, this object is a class object that will be used by others, create it immediately
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
    }  else if ("WeaponTripleBullet" == type) {
        ret = createWeaponTripleBullet(object);
    } else if ("FriendPlane" == type) {
        ret = createFriendPlane(object);
    } else if ("EnemyPlane" == type) {
        ret = createEnemyPlane(object);
    } else if ("EnemyGenerator" == type) {
        ret = createEnemyGenerator(object);
    } else if ("ScrollingBackground" == type) {
        ret = createScrollingBackground(object);
    } else if ("PlayerPlane" == type) {
        ret = addPlayerPlane(object);
    } else if ("SceneObject" == type) {
        ret = addSceneObject(object);
    } else {
        log("Don't know how to create object from json: %s", object.dump(4).c_str());
    }

    if (ret) {
        // successfully created an object, check if it should be added to scene
        if (type == "SceneObject" || type == "PlayerPlane" ) {
            // SceneObject is added to the scene, PlayerPlane is a special SceneObject
            if (mScene) {
                mScene->addChild(ret);
            }
        } else {
            // the object is NOT going to be added to the scene, **AS A RULE**
            // ObjectManager retains it as it probably will be used by other objects
            // like a weapon which will be used later when creating WarObject
            // a bullet which will be used later when creating a Weapon
            // a template WarObject which will be used later to create many copies
            //// not a SceneObject, meaning that this is a class object used by SceneObject.
            ret->retain();
            mRetainedObjects.push_back(ret);
        }
    }
    
    return ret;
}

ScrollingBackground * ObjectManager::createScrollingBackground(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file1 = object.value("file1", j_null);
    auto file2 = object.value("file2", j_null);
    auto speed = object.value("speed", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || file1.is_string() == false || file2.is_string() == false || speed.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto windowSize = Director::getInstance()->getWinSize();
    auto bkg = ScrollingBackground::create(file1, file2, windowSize, speed);
    bkg->setObjectId(id);
    bkg->setObjectName(name);
    
    return bkg;
}

EnemyGenerator * ObjectManager::createEnemyGenerator(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto classId = object.value("classId", j_null);
    auto triggerInterval = object.value("triggerInterval", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || classId.is_number_unsigned() == false || triggerInterval.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto classObject = findRetainedObject(classId);
    if (classObject == nullptr) {
        // can NOT find the object with this classId, the object from which this generator will clone
        // don't know how to create this generator
        return nullptr;
    }
    
    // triggerInterval is **milliseconds** in json file
    auto enemyGenerator = EnemyGenerator::create(static_cast<float>(triggerInterval) * 0.001);
    enemyGenerator->setObjectId(id);
    enemyGenerator->setObjectName(name);
    enemyGenerator->setObject(classObject);
    
    return enemyGenerator;
}

EnemyPlane * ObjectManager::createEnemyPlane(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file = object.value("file", j_null);
    auto calmPeriod = object.value("calmPeriod", j_null);
    auto speed = object.value("speed", j_null);
    auto health = object.value("health", j_null);
    auto damage = object.value("damage", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || file.is_string() == false || calmPeriod.is_number_unsigned() == false || speed.is_number_unsigned() == false || health.is_number_unsigned() == false || damage.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto enemyPlane = EnemyPlane::create(file);
    enemyPlane->setObjectId(id);
    enemyPlane->setObjectName(name);
    enemyPlane->setCalmPeriod(static_cast<float>(calmPeriod) * 0.001f);
    enemyPlane->setSpeed(speed);
    enemyPlane->setHealth(health);
    enemyPlane->setDamage(damage);
    
    auto weaponId = object.value("weapon", j_null);
    if (weaponId.is_number_unsigned() && weaponId != 0) {
        auto weapon = dynamic_cast<Weapon *>(findRetainedObject(weaponId));
        if (weapon) {
            enemyPlane->setWeapon(weapon);
        }
    }
    
    return enemyPlane;
}

FriendPlane * ObjectManager::createFriendPlane(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file = object.value("file", j_null);
    auto calmPeriod = object.value("calmPeriod", j_null);
    auto speed = object.value("speed", j_null);
    auto health = object.value("health", j_null);
    auto damage = object.value("damage", j_null);
    if (id.is_number_unsigned() == false || name.is_string() == false || file.is_string() == false || calmPeriod.is_number_unsigned() == false || speed.is_number_unsigned() == false || health.is_number_unsigned() == false || damage.is_number_unsigned() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto friendPlane = FriendPlane::create(file);
    friendPlane->setObjectId(id);
    friendPlane->setObjectName(name);
    friendPlane->setCalmPeriod(static_cast<float>(calmPeriod) * 0.001f);
    friendPlane->setSpeed(speed);
    friendPlane->setHealth(health);
    friendPlane->setDamage(damage);
    //mFriendPlanes.push_back(friendPlane);
    
    auto weaponId = object.value("weapon", j_null);
    if (weaponId.is_number_unsigned() && weaponId != 0) {
        auto weapon = dynamic_cast<Weapon *>(findRetainedObject(weaponId));
        if (weapon) {
            friendPlane->setWeapon(weapon);
        }
    }
    
    return friendPlane;
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
    
    if (findRetainedObject(id) != nullptr) {
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
    if (bulletId.is_number_unsigned() && bulletId != 0) {
        auto bullet = dynamic_cast<Bullet *>(findRetainedObject(bulletId));
        if (bullet) {
            weapon->setBullet(bullet);
        }
    }
    
    return weapon;
}

WeaponTripleBullet * ObjectManager::createWeaponTripleBullet(const json &object)
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
    auto weaponTripleBullet = WeaponTripleBullet::create(static_cast<float>(triggerInterval) * 0.001);
    weaponTripleBullet->setObjectId(id);
    weaponTripleBullet->setObjectName(name);
    mWeapons.push_back(weaponTripleBullet);
    
    auto bulletId = object.value("bullet", j_null);
    if (bulletId.is_number_unsigned() && bulletId != 0) {
        auto bullet = dynamic_cast<Bullet *>(findRetainedObject(bulletId));
        if (bullet) {
            weaponTripleBullet->setBullet(bullet);
        }
    }
    
    return weaponTripleBullet;
}

GameObject * ObjectManager::addSceneObject(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto classId = object.value("classId", j_null);
    auto bornX = object.value("bornX", j_null);
    auto bornY = object.value("bornY", j_null);
    auto destinationX = object.value("destinationX", j_null);
    auto destinationY = object.value("destinationY", j_null);
    
    if (id.is_number_unsigned() == false || name.is_string() == false || classId.is_number_unsigned() == false || bornX.is_number_integer() == false || bornY.is_number_integer() == false || destinationX.is_number_integer() == false || destinationY.is_number_integer() == false) {
        return nullptr;
    }
    
    if ( findRetainedObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto classObject = findRetainedObject(classId);
    if (classObject == nullptr) {
        // can NOT find the object with this classId, the object from which this SceneObject will clone
        // don't know how to create this SceneObject
        return nullptr;
    }
    
    auto sceneObject = classObject->clone();
    sceneObject->setObjectId(id);
    sceneObject->setObjectName(name);
    sceneObject->setPosition(Vec2(static_cast<float>(bornX), static_cast<float>(bornY)));
    auto flyingObject = dynamic_cast<FlyingObject *>(sceneObject);
    if (flyingObject) {
        flyingObject->setDestination(Vec2(static_cast<float>(destinationX), static_cast<float>(destinationY)));
    }
    
    return sceneObject;
}

GameObject * ObjectManager::addPlayerPlane(const json &object)
{
    if (mPlayerPlane) {
        // sorry only one friend plane is supported
        return nullptr;
    }
    
    auto sceneObject = addSceneObject(object);
    mPlayerPlane = dynamic_cast<FriendPlane *>(sceneObject);
    
    // should return sceneObject, instead of mPlayerPlane
    return sceneObject;
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
    
    if (object == mPlayerPlane && mScene) {
        mScene->onFriendPlaneEnter();
    }
}

void ObjectManager::ObjectExitScene(GameObject *object)
{
    if (object == mPlayerPlane && mScene) {
        mScene->onFriendPlaneExit();
    }
    
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

