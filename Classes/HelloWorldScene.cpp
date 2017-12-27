#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ScrollingBackground.h"
#include "FlyingObject.h"
#include "FriendPlane.h"
#include "EnemyPlane.h"
#include "WeaponTripleBullet.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = HelloWorld::create();
    scene->initWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto windowSize = Director::getInstance()->getWinSize();
    auto windowSizeInPixels = Director::getInstance()->getWinSizeInPixels();
    log("windowSize: %f, %f", windowSize.width, windowSize.height);
    log("visibleSize: %f, %f", visibleSize.width, visibleSize.height);
    log("windowSizeInPixels: %f, %f", windowSizeInPixels.width, windowSizeInPixels.height);
    log("origin: %f, %f", origin.x, origin.y);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x/2,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    
    // background
    auto bkg = ScrollingBackground::create("background.png", "background.png", windowSize);
    if (bkg) {
        bkg->startScroll(20.0f);
        addChild(bkg, -100);
    }
    
    
    // create the bullet, weapon
    auto friendBullet = Bullet::create("bullet1.png", 300, 100);
    auto friendWeapon = WeaponTripleBullet::create(FRIEND, 0.4f);
    friendWeapon->setBullet(friendBullet);
    
    //auto enemyBullet = Bullet::create("bullet1.png", 100, 200);
    auto enemyWeapon = Weapon::create(ENEMY, 0.8f);
    enemyWeapon->setBullet(friendBullet);
    
    auto ep = EnemyPlane::create("myPlane.png");
    if (ep) {
        ep->setSpeed(10);
        ep->setPosition(Vec2(320, 480));
        ep->setDestination(Vec2(240, 0));
        ep->setWeapon(enemyWeapon);
        ep->openFire();
        addChild(ep);
    }
    
    // how can I prevent creating instance from "abstract" FlyingObject
    /*
    auto fo2 = FlyingObject::create("myPlane.png");
    if (fo2) {
        fo2->setPosition(Vec2(240, 200));
        addChild(fo2);
    }
    */


    auto fo5 = FriendPlane::create("myPlane.png");
    if (fo5) {
        fo5->setPosition(Vec2(0, 0));
        fo5->setForceType(FRIEND);
        addChild(fo5);
        fo5->setSpeed(200);
        fo5->setDestination(Vec2(320, 480));
        fo5->setWeapon(friendWeapon);
        fo5->openFire();
    }

    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
