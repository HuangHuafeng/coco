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
    
    int mNumberOfBullet;
    WarObject *mWarObject;
    
    void updateBullet();
    void fireABullet(const cocos2d::Vec2 &start, const cocos2d::Vec2 &destination);
    
public:
    ~Weapon();
    
    virtual Weapon * clone() const override;
    virtual void attachToWarObject(WarObject *warObject);
    void setBullet(Bullet *bullet);
    
    virtual void upgrade();
    virtual void downgrade();
    void setNumberOfBullet(int numberOfBullet);
};

#endif /* Weapon_h */
