#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PlayScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = HelloWorld::create();
    
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
    if ( !Scene::init() )
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
    
    auto miStartGame = MenuItemFont::create("Start", CC_CALLBACK_1(HelloWorld::menuStartGameCallback, this));
    miStartGame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(miStartGame, closeItem, NULL);
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

    
    return true;
}


void HelloWorld::menuStartGameCallback(Ref *sender)
{
    std::string sbuf;
    auto status = FileUtils::getInstance()->getContents("testgame.json", &sbuf);
    
    if (status == FileUtils::Status::OK) {
        auto playScene = PlayScene::createScene(sbuf);
        if (playScene) {
            Director::getInstance()->pushScene(playScene);
        }
    } else {
        log("failed to read file testgame.json, status: %d", status);
    }
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
