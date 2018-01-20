//
//  MultipleBulletDiffuse.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef MultipleBulletDiffuse_h
#define MultipleBulletDiffuse_h

#include "Weapon.h"

class MultipleBulletDiffuse : public Weapon {
    
protected:
    float mBulletStartInterval;
    float mBulletDestinationInterval;
    int mMaxNumberOfBullet;
    
    MultipleBulletDiffuse(float interval = 1.0f);

    virtual void generateOnce() override;
    
public:
    static MultipleBulletDiffuse * create(float interval = 1.0f);
    
    virtual MultipleBulletDiffuse * clone() const override;
};

#endif /* MultipleBulletDiffuse_h */
