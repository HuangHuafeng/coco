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
    virtual void pullTrigger();
    
public:
    WeaponTripleBullet(ForceType forceType = ENEMY, float bulletInterval = 1.0f);
};

#endif /* WeaponTripleBullet_h */
