#include "GameLoadScene.h"

#include "ChallengeMenuScene.h"
//#include "BackGroundLayer.h"
//#include "SpriteLayer.h"
//#include "MenuLayer.h"

USING_NS_CC;

// z-index of game layers
#define BG_ZINDEX 1
#define SP_ZINDEX 2
#define MN_ZINDEX 3


Scene* GameLoad::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLoad::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLoad::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
	// 2. Main Screen

    // splash screen image
    auto splashScrImg = Sprite::create("splash.png");
    splashScrImg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(splashScrImg, -1);

    // play button image
    auto play = MenuItemImage::create("play_normal.png", "play_selected.png", CC_CALLBACK_0(GameLoad::playSelected , this));
    play->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/4));

    auto scale_1 = ScaleBy::create(0.5, 1.3);
    auto scale_2 = ScaleTo::create(0.5, 1);
    auto seq = Sequence::create(scale_1, scale_2, nullptr);
    auto action = RepeatForever::create(seq);
    play->runAction(action);

    auto menu = Menu::create(play, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

//    //////////////////////////////
//	// 2. BackGroundLayer
//    auto bgLayer = BackGroundLayer::create();
//    this->addChild(bgLayer, BG_ZINDEX);
//
//    //////////////////////////////
//	// 3. SpriteLayer
//    auto spLayer = SpriteLayer::create();
//    this->addChild(spLayer, SP_ZINDEX);
//
//    //////////////////////////////
//	// 4. MenuLayer
//    auto mnLayer = MenuLayer::create();
//    this->addChild(mnLayer, MN_ZINDEX);

    // trigger to active key events
    this->setKeypadEnabled(true);

    return true;
}

void GameLoad::playSelected()
{
	// replacing the splash screen with the challenge scene
	auto challengeScene = ChallengeMenu::createScene();
	Director::getInstance()->replaceScene(challengeScene);
}

void GameLoad::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	Director::getInstance()->end();
}
