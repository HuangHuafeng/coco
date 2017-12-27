//
//  WeaponTripleBullet.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef WeaponTripleBullet_h
#define WeaponTripleBullet_h

#include "Weapon.h"

class WeaponTripleBullet : public Weapon {
    
protected:
    WeaponTripleBullet(ForceType forceType = ENEMY, float bulletInterval = 1.0f);

    virtual void pullTrigger();
    
public:
    static WeaponTripleBullet * create(ForceType forceType, float bulletInterval);
};

#endif /* WeaponTripleBullet_h */
