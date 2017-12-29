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
    WeaponTripleBullet(float bulletInterval = 1.0f);

    virtual void pullTrigger();
    
public:
    static WeaponTripleBullet * create(float bulletInterval);
};

#endif /* WeaponTripleBullet_h */
