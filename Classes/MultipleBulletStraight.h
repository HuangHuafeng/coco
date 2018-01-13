//
//  MultipleBulletStraight.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef MultipleBulletStraight_h
#define MultipleBulletStraight_h

#include "Weapon.h"

class MultipleBulletStraight : public Weapon {
    
protected:
    MultipleBulletStraight(float interval = 1.0f);

    virtual void generateOnce() override;
    
public:
    static MultipleBulletStraight * create(float interval = 1.0f);
    
    virtual MultipleBulletStraight * clone() const override;
};

#endif /* MultipleBulletStraight_h */
