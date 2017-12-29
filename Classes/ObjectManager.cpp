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
    mFriendPlane = nullptr;
}

ObjectManager::~ObjectManager()
{
    if (mScene) {
        mScene->autorelease();
        mScene = nullptr;
    }
    
    std::vector<Bullet *>::const_iterator itBullet = mBullets.cbegin();
    std::vector<Bullet *>::const_iterator endBullet = mBullets.cend();
    while (itBullet != endBullet) {
        (*itBullet)->autorelease();
        itBullet++;
    }
    
    std::vector<Weapon *>::const_iterator itWeapon = mWeapons.cbegin();
    std::vector<Weapon *>::const_iterator endWeapon = mWeapons.cend();
    while (itWeapon != endWeapon) {
        (*itWeapon)->autorelease();
        itWeapon++;
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
    
    json objects = game.value("objects", j_null);
    if (false == loadObjects(objects)) {
        return false;
    }
    
    log("looks good!");
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
    if (type.is_number_integer() == false) {
        return false;
    }
    
    GameObject * ret = nullptr;
    switch (static_cast<int>(type)) {
        case BULLET: // Bullet
            ret = createBullet(object);
            break;
            
        case WEAPON: // Weapon
            ret = createWeapon(object);
            break;
            
        case FRIENDPLANE: // FriendPlane
            ret = createFriendPlane(object);
            break;
            
        default:
            break;
    }
    
    if (ret) {
        // successfully created an object, check if it should be added to scene
        auto addToScene = object.value("addToScene", j_null);
        if (addToScene.is_boolean() && mScene) {
            if (static_cast<bool>(addToScene) == true) {
                mScene->addChild(ret);
            }
        }
    }
    
    return ret;
}

Bullet * ObjectManager::createBullet(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto file = object.value("file", j_null);
    auto speed = object.value("speed", j_null);
    auto damage = object.value("damage", j_null);
    if (id.is_number_integer() == false || name.is_string() == false || file.is_string() == false || speed.is_number_integer() == false || damage.is_number_integer() == false) {
        return nullptr;
    }
    
    if ( getObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    auto bullet = Bullet::create(file, speed, damage);
    bullet->setId(id);
    bullet->setName(name);
    bullet->retain();
    mBullets.push_back(bullet);
    return bullet;
}

Weapon * ObjectManager::createWeapon(const json &object)
{
    const json j_null;
    auto id = object.value("id", j_null);
    auto name = object.value("name", j_null);
    auto triggerInterval = object.value("triggerInterval", j_null);
    if (id.is_number_integer() == false || name.is_string() == false || triggerInterval.is_number_integer() == false) {
        return nullptr;
    }
    
    if ( getObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }

    // triggerInterval is **milliseconds** in json file
    auto weapon = Weapon::create(static_cast<float>(triggerInterval) * 0.001);
    weapon->setId(id);
    weapon->setName(name);
    weapon->retain();
    mWeapons.push_back(weapon);
    
    auto bulletId = object.value("bullet", j_null);
    if (bulletId.is_number_integer()) {
        auto bullet = getObject(static_cast<int>(bulletId));
        if (bullet) {
            weapon->setBullet(dynamic_cast<Bullet *>(bullet));
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
    if (id.is_number_integer() == false || name.is_string() == false || file.is_string() == false || speed.is_number_integer() == false) {
        return nullptr;
    }
    
    if ( getObject(id) != nullptr) {
        // there's already an object with this id, don't create another one
        return nullptr;
    }
    
    mFriendPlane = FriendPlane::create(file);
    mFriendPlane->retain();
    mFriendPlane->setSpeed(speed);
    
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
        auto weapon = getObject(weaponId);
        if (weapon) {
            mFriendPlane->setWeapon(dynamic_cast<Weapon *>(weapon));
            mFriendPlane->openFire();
        }
    }
    
    return mFriendPlane;
}

GameObject * ObjectManager::getObject(int id)
{
    std::vector<Bullet *>::const_iterator itBullet = mBullets.cbegin();
    std::vector<Bullet *>::const_iterator endBullet = mBullets.cend();
    while (itBullet != endBullet) {
        if ((*itBullet)->getId() == id) {
            return *itBullet;
        }
        itBullet++;
    }
    
    std::vector<Weapon *>::const_iterator itWeapon = mWeapons.cbegin();
    std::vector<Weapon *>::const_iterator endWeapon = mWeapons.cend();
    while (itWeapon != endWeapon) {
        if ((*itWeapon)->getId() == id) {
            return *itWeapon;
        }
        itWeapon++;
    }
    
    if (mFriendPlane && mFriendPlane->getId() == id) {
        return mFriendPlane;
    }
    
    return nullptr;
}
