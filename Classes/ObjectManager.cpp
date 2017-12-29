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
    const int type = object.value("type", j_null);
    bool ret = false;
    switch (type) {
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
    
    return ret;
}

bool ObjectManager::createBullet(const json &object)
{
    const json j_null;
    const int type = object.value("type", j_null);
    if (type != BULLET) {
        assert(false);
        return false;
    }

    const int id = object.value("id", j_null);
    if (getObject(id) != nullptr) {
        return false;
    }

    const std::string name = object.value("name", j_null);
    const std::string file = object.value("file", j_null);
    const float speed = object.value("speed", j_null);
    const int damage = object.value("damage", j_null);
    
    auto bullet = Bullet::create(file, speed, damage);
    bullet->setId(id);
    bullet->setName(name);
    bullet->retain();
    mBullets.push_back(bullet);
    return true;
}

bool ObjectManager::createWeapon(const json &object)
{
    const json j_null;
    const int type = object.value("type", j_null);
    if (type != WEAPON) {
        assert(false);
        return false;
    }
    
    const int id = object.value("id", j_null);
    if (getObject(id) != nullptr) {
        return false;
    }
    
    const std::string name = object.value("name", j_null);
    const float triggerInterval = object.value("triggerInterval", j_null);
    auto weapon = Weapon::create(triggerInterval * 0.001);  // triggerInterval is milliseconds in json file
    weapon->setId(id);
    weapon->setName(name);
    weapon->retain();
    mWeapons.push_back(weapon);
    
    const int bulletId = object.value("bullet", j_null);
    if (bulletId != 0) {
        auto bullet = getObject(bulletId);
        if (bullet) {
            weapon->setBullet(dynamic_cast<Bullet *>(bullet));
        }
    }
    
    return true;
}

bool ObjectManager::createFriendPlane(const json &object)
{
    const json j_null;
    const int type = object.value("type", j_null);
    if (type != FRIENDPLANE) {
        assert(false);
        return false;
    }
    
    if (mFriendPlane) {
        // sorry only one friend plane is supported
        return false;
    }
    
    const int id = object.value("id", j_null);
    if (getObject(id) != nullptr) {
        return false;
    }
    
    const std::string name = object.value("name", j_null);
    const std::string file = object.value("file", j_null);
    const float speed = object.value("speed", j_null);
    const float bornTime = object.value("bornTime", j_null);
    const float bornX = object.value("bornX", j_null);
    const float bornY = object.value("bornY", j_null);
    const float destinationX = object.value("destinationX", j_null);
    const float destinationY = object.value("destinationY", j_null);
    
    mFriendPlane = FriendPlane::create(file);
    mFriendPlane->setPosition(Vec2(bornX, bornY));
    mFriendPlane->setSpeed(speed);
    mFriendPlane->setDestination(Vec2(destinationX, destinationY));
    
    if (mScene) {
        mScene->addChild(mFriendPlane);
    }
    
    const int weaponId = object.value("weapon", j_null);
    if (weaponId != 0) {
        auto weapon = getObject(weaponId);
        if (weapon) {
            mFriendPlane->setWeapon(dynamic_cast<Weapon *>(weapon));
            mFriendPlane->openFire();
        }
    }
    
    mFriendPlane->retain();
    
    return true;
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
