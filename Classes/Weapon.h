//
//  Weapon.hpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#ifndef Weapon_h
#define Weapon_h

#include "Bullet.h"
#include "ObjectGenerator.h"

class WarObject;    // need forward declaration here

class Weapon : public ObjectGenerator {

protected:
    Weapon(float interval = 1.0f);
    
    WarObject *mWarObject;
    cocos2d::Vec2 mBulletOffset;
    
    void updateBullet();
    
    virtual void generateOnce() override;
    
public:
    ~Weapon();
    
    virtual Weapon * clone() const override;
    virtual void attachToWarObject(WarObject *warObject);
    void setBullet(Bullet *bullet);
    
    static Weapon * create(float interval = 1.0f);
};

#endif /* Weapon_h */
