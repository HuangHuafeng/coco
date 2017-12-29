//
//  Weapon.hpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#ifndef Weapon_h
#define Weapon_h

#include "Bullet.h"

class WarObject;    // need forward declaration here

typedef enum {
    WEAPON_FIRE_ACTION = 9999,
} WeaponAction;

class Weapon : public GameObject {

protected:
    Weapon(float triggerInterval = 1.0f);
    
    Bullet *mBullet;
    WarObject *mWarObject;
    float mTriggerInterval;
    cocos2d::Vec2 mBulletOffset;
    
    void updateBullet();
    virtual void pullTrigger();
    
public:
    ~Weapon();
    
    virtual void openFire();
    virtual void ceaseFire();
    virtual void attachToWarObject(WarObject *warObject);
    void setBulletInterval(float bulletInterval);
    void setBullet(Bullet *bullet);
    
    static Weapon * create(float triggerInterval = 1.0f);
};

#endif /* Weapon_h */
