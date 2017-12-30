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
    WeaponTripleBullet(float interval = 1.0f);

    virtual void generateOnce() override;
    
public:
    static WeaponTripleBullet * create(float interval = 1.0f);
};

#endif /* WeaponTripleBullet_h */
