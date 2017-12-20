//
//  Weapon.hpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#ifndef Weapon_h
#define Weapon_h

#include "Bullet.h"

typedef enum {
    WEAPON_FIRE_ACTION = 9999,
} WeaponAction;

class Weapon : public cocos2d::Node {

protected:
    ForceType mForceType;
    float mBulletInterval;
    
    virtual void fireBullet();
    
public:
    Weapon(ForceType forceType = ENEMY, float bulletInterval = 1.0f);
    
    void FireOn();
    void FireOff();
    void setBulletInterval(float bulletInterval);
};

#endif /* Weapon_h */
