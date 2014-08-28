#include "Challenge1.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"

#define BG_ZINDEX 0
#define SP_ZINDEX 1
#define MN_ZINDEX 2
#define BUF_HT 15

USING_NS_CC;
USING_NS_CC_EXT;

Scene* Challenge1::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge1::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Challenge1::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add the background layer
    auto bgLayer = BackGroundLayer::create();
    this->addChild(bgLayer, BG_ZINDEX);

    /////////////////////////////
    // 2. add the sprite layer
    //auto spLayer = SpriteLayer::create();
    //this->addChild(spLayer, SP_ZINDEX);

    /////////////////////////////
    // 3. add the sprite layer
    auto menuLayer = MenuLayer::create();

    // reset button
    auto restart_scene = MenuItemImage::create("reset_normal.png",
                                               "reset_normal.png",
                                               CC_CALLBACK_1(Challenge1::restartScene , this));
    restart_scene->setScale(0.8);
    restart_scene->setPosition(-restart_scene->getContentSize().width/2, -restart_scene->getContentSize().height/2);
    menuLayer->addToTopMenu(restart_scene);
    menuLayer->addForceMenu(this, cccontrol_selector(Challenge1::forceValueChanged));

    this->addChild(menuLayer, MN_ZINDEX);

    this->setKeypadEnabled(true);

    return true;
}

void Challenge1::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    //_spriteLayer->changeForceValue(pSlider->getValue());
}

void Challenge1::restartScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(createScene());
}

void Challenge1::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->popScene();
}
