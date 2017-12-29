//
//  ScrollingBackground.h
//  MyGame
//
//  Created by 黄华锋 on 15/12/2017.
//

#ifndef ScrollingBackground_h
#define ScrollingBackground_h

#include "GameObject.h"

class ScrollingBackground : public GameObject {
public:
    ScrollingBackground(const cocos2d::Size bkgSize);
    //virtual ~Background();
    
    static ScrollingBackground* create(const std::string& bkgImg1, const std::string& bkgImg2, const cocos2d::Size bkgSize);
    void startScroll(float timeForOneScroll);
    
protected:
    enum {
        IMAGE1 = 1,
        IMAGE2,
    };
    
private:
    cocos2d::Size mBackgroundSize;
};

#endif /* ScrollingBackground_h */
