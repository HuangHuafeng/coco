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
    ScrollingBackground(const cocos2d::Size bkgSize, float speed = 0);
    //virtual ~Background();
    
    virtual ScrollingBackground * clone() const override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    static ScrollingBackground* create(const std::string& bkgImg1, const std::string& bkgImg2, const cocos2d::Size bkgSize, float speed = 0);
    void startScroll();
    
protected:
    enum {
        IMAGE1 = 1,
        IMAGE2,
    };
    
private:
    cocos2d::Size mBackgroundSize;
    float mSpeed;
};

#endif /* ScrollingBackground_h */
