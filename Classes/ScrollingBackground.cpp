//
//  ScrollingBackground.cpp
//  MyGame
//
//  Created by 黄华锋 on 15/12/2017.
//

#include "ScrollingBackground.h"
#include "cocos2d.h"

USING_NS_CC;

ScrollingBackground::ScrollingBackground(const Size bkgSize, float speed)
{
    mBackgroundSize = bkgSize;
    mSpeed = speed;
}

ScrollingBackground * ScrollingBackground::clone() const
{
    Sprite *childImage1 = dynamic_cast<Sprite *>(getChildByTag(IMAGE1));
    Sprite *childImage2 = dynamic_cast<Sprite *>(getChildByTag(IMAGE2));
    if (childImage1 && childImage2) {
        auto image1 = Sprite::createWithTexture(childImage1->getTexture());
        auto image2 = Sprite::createWithTexture(childImage2->getTexture());
        auto scrollingBackground = new (std::nothrow) ScrollingBackground(mBackgroundSize, mSpeed);
        scrollingBackground->setObjectId(mId);
        scrollingBackground->setObjectName(mName);
        scrollingBackground->addChild(image1, 0, IMAGE1);
        scrollingBackground->addChild(image2, -1, IMAGE2);
        scrollingBackground->autorelease();
        return scrollingBackground;
    }
    
    return nullptr;
}

ScrollingBackground * ScrollingBackground::create(const std::string& bkgImg1, const std::string& bkgImg2, const cocos2d::Size bkgSize, float speed)
{
    auto image1 = Sprite::create(bkgImg1);
    auto image2 = Sprite::create(bkgImg2);
    if (image1 && image2)
    {
        ScrollingBackground* sbkg = new (std::nothrow) ScrollingBackground(bkgSize, speed);
        sbkg->addChild(image1, 0, IMAGE1);
        sbkg->addChild(image2, -1, IMAGE2);
        sbkg->autorelease();
        return sbkg;
    }

    return nullptr;
}

void ScrollingBackground::startScroll()
{
    auto img1 = this->getChildByTag(IMAGE1);
    auto img2 = this->getChildByTag(IMAGE2);
    
    img1->stopAllActions();
    img2->stopAllActions();
    
    const auto img1Size = img1->getContentSize();
    auto img1ScaleX = mBackgroundSize.width / img1Size.width;  // scale according to width, no strech
    auto img1ScaleY = img1ScaleX;
    if (img1Size.height * img1ScaleY < mBackgroundSize.height) {
        // scale further so that image1 fills the screen, 10% extra
        img1ScaleY = mBackgroundSize.height / img1Size.height * 1.1f;
    }
    img1->setScale(img1ScaleX, img1ScaleY);
    img1->setAnchorPoint(Vec2(0, 0));
    
    const auto img2Size = img2->getContentSize();
    const auto img2ScaleX = mBackgroundSize.width / img2Size.width;  // scale according to width, no strech
    auto img2ScaleY = img2ScaleX;
    if (img2Size.height * img2ScaleY < mBackgroundSize.height) {
        // scale further so that image1 fills the screen, 10% extra
        img2ScaleY = mBackgroundSize.height / img2Size.height * 1.1f;
    }
    img2->setScale(img2ScaleX, img2ScaleY);
    img2->setAnchorPoint(Vec2(0, 0));
    
    auto totalScrollHeight = img1Size.height * img1ScaleY + img2Size.height * img2ScaleY;
    
    // first set the position of the two images directly
    img1->setPosition(Vec2(0, 0));
    
    const auto timeForOneScroll = mBackgroundSize.height / mSpeed;
    
    // image1
    auto img1ScrollHeightPart1 = img1Size.height * img1ScaleY;
    auto img1ScrollTimePart1 = timeForOneScroll * (img1ScrollHeightPart1 / totalScrollHeight);
    auto img1MoveDownPart1 = MoveBy::create(img1ScrollTimePart1, Vec2(0, - img1ScrollHeightPart1));
    // wait as image1 is out side the screen, the time is calculated by how much image2 need to scroll
    auto delayTime = timeForOneScroll * ((img2Size.height * img2ScaleY - mBackgroundSize.height) / totalScrollHeight);
    auto img1Delay = DelayTime::create(delayTime);
    // move to top of the screen
    auto img1MoveTo = MoveTo::create(0.0f, Vec2(0, mBackgroundSize.height));
    // move in to the screen and back to position Vec2(0, 0)
    auto img1MoveDown2ndPart = MoveBy::create(timeForOneScroll - img1ScrollTimePart1 - delayTime, Vec2(0, - mBackgroundSize.height));
    
    auto img1Seq = Sequence::create(img1MoveDownPart1, img1Delay, img1MoveTo, img1MoveDown2ndPart, nullptr);
    img1->runAction(RepeatForever::create(img1Seq));
    
    // image2
    // move to the top of the screen
    auto img2MoveTo = MoveTo::create(0.0f, Vec2(0, mBackgroundSize.height));
    auto img2DelayTime = timeForOneScroll * (img1Size.height * img1ScaleY - mBackgroundSize.height) / totalScrollHeight;
    auto img2Delay = DelayTime::create(img2DelayTime);
    auto img2ScrollHeight = img2Size.height * img2ScaleY + mBackgroundSize.height;
    auto img2ScrollTime = timeForOneScroll * (img2ScrollHeight / totalScrollHeight);
    // scroll
    auto img2MoveDown = MoveBy::create(img2ScrollTime, Vec2(0, - img2ScrollHeight));
    // out of the screen, end! as image1 is at its initial position at this moment
    
    auto img2Seq = Sequence::create(img2MoveTo, img2Delay, img2MoveDown, nullptr);
    img2->runAction(RepeatForever::create(img2Seq));
}

void ScrollingBackground::onExit()
{
    stopAllActions();
    GameObject::onExit();
}

void ScrollingBackground::onEnter()
{
    GameObject::onEnter();
    startScroll();
}
