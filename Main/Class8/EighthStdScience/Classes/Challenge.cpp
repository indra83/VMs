#include "Challenge.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"
#include "ChallengeMenuScene.h"
#include "SimpleAudioEngine.h"
#include "NativeHelper.h"
#include "PopUpScene.h"
#include "Util.h"

static const int BG_ZINDEX=0;
static const int SP_ZINDEX=1;
static const int MN_ZINDEX=2;
static const int INF_ZINDEX=3;
static const int DEST_ZINDEX=4;


USING_NS_CC;
USING_NS_CC_EXT;

template< class Derived >
Derived * Challenge<Derived>::create(bool showInfo)
{
    auto *pRet = new Derived();
    if (pRet && pRet->init(showInfo))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

template< class Derived >
Scene* Challenge<Derived>::createScene(bool showInfo)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge<Derived>::create(showInfo);
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
template< class Derived >
bool Challenge<Derived>::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 1. add the background layer
    _bgLayer = BackGroundLayer::create();
    this->addChild(_bgLayer, BG_ZINDEX);

    /////////////////////////////
    // 2. add the menu layer
    _menuLayer = MenuLayer::create();

    // add the reset button
    auto restart_scene = MenuItemImage::create("reset_normal.png", "reset_normal.png");
    restart_scene->setScale(0.8);
    _menuLayer->addToTopMenu(restart_scene,
                             [](Ref * sender) -> void
                             {
                                Director::getInstance()->replaceScene(Derived::createScene(false));
                             });

    // add the info button
    auto info = MenuItemImage::create("info.png", "info.png");
    info->setScale(0.8);
    _menuLayer->addToTopMenu(info,
                             [this](Ref * sender) -> void
                             {
                                showInfoPopUp();
                             });

    // add the challenges menu
    auto list = MenuItemImage::create("menu.png", "menu.png");
    list->setScale(0.8);
    _menuLayer->addToTopMenu(list,
                             [](Ref * sender) -> void
                             {
                                Director::getInstance()->pushScene(ChallengeMenu::createScene(true, nullptr));
                             });

    this->addChild(_menuLayer, MN_ZINDEX);

    /////////////////////////////
    // 3. add the sprite layer
    _spriteLayer = SpriteLayer::create();
    _spriteLayer->setMass(30.0);
    _spriteLayer->setFrictionCoefficient(0.5);
    _spriteLayer->addToMovables(_bgLayer);
    _spriteLayer->changeForceValue(0.0);
    _spriteLayer->setMenuLayer(_menuLayer);
    this->addChild(_spriteLayer, SP_ZINDEX);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(CHIME.c_str());
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    // enable keypress cbs
    this->setKeypadEnabled(true);

    // show initial info popup
    if (showInfo)
        showInfoPopUp();

    return true;
}


template< class Derived >
void Challenge<Derived>::addPopupMenu(const std::string & title, const std::string & caption, bool loud, bool vibrate, bool replace)
{
    if (loud)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME.c_str());
    if (vibrate)
        NativeHelper::vibrate(500);

    captureScreenAsSprite(
                [=](Sprite * sprite) -> void
                {
                    auto scene = PopUp::createScene(title, caption, sprite);
                    if (replace)
                        Director::getInstance()->replaceScene(scene);
                    else
                        Director::getInstance()->pushScene(scene);
                });
}


template< class Derived >
void Challenge<Derived>::showInfoPopUp()
{
    addPopupMenu("INSTRUCTION", "Move the box to the right by applying force on it. Drag and hold the slider in position to apply a specific amount of force."
    		" Help the box reach the house which is some distance away", false);
}

template< class Derived >
void Challenge<Derived>::done()
{
    _spriteLayer->pause();
    addPopupMenu("Challenge Complete", "Congrats!!", true, true, true);
}

template< class Derived >
void Challenge<Derived>::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    _spriteLayer->changeForceValue(pSlider->getValue());
}

template< class Derived >
void Challenge<Derived>::frictionValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    _spriteLayer->changeFrictionValue(pSlider->getValue());
}

template< class Derived >
void Challenge<Derived>::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    Director::getInstance()->popScene();
}

//////////////////////////////
// challenge1
//////////////////////////////

static const int SHOW_AFTER = 2;
static const int TARGET_METRES = 50;

Scene* Challenge1::createScene(bool showInfo)
{
    return Challenge<Challenge1>::createScene(showInfo);
}

// on "init" you need to initialize your instance
bool Challenge1::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

    // add the force menu
    _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE, SpriteLayer::MAX_FORCE, 0, this, cccontrol_selector(Challenge1::forceValueChanged));

    // adding destination sprite at 100 meters away from current screen
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // add destinations on both sides
    auto addDestination = [this, visibleSize] ( bool right ) -> void
    {
        auto dest = Sprite::create("destination.png");
        dest->setScale(0.8);
        dest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        dest->setPosition(Vec2( (right ? 1 : -1) * TARGET_METRES * SpriteLayer::PTM_RATIO + visibleSize.width/2, 
                                visibleSize.height/3 + 10));

        auto dest_small = Sprite::create("destination.png");
        dest_small->setScale(0.8*SpriteLayer::MINI_MAP_SCALE);
        dest_small->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        dest_small->setPosition(Vec2(  (right ? 1 : -1) * TARGET_METRES * SpriteLayer::PTM_RATIO * SpriteLayer::MINI_MAP_SCALE + visibleSize.width/2, 0.0));
        _spriteLayer->addStationaryChild(dest, dest_small);
    };

    addDestination(true);
    addDestination(false);
 
    Vec2 originalPos = _bgLayer->getPosition();
    _spriteLayer->setPeriodicCB([this, originalPos] () -> bool
    {
        if( fabs(_bgLayer->getPosition().x - originalPos.x) >= TARGET_METRES * SpriteLayer::PTM_RATIO )
        {
            done();
            return false;
        }
        return true;
    });

   return true;
}

void Challenge1::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    static float prevValue=0.0;
    Challenge<Challenge1>::forceValueChanged(sender, controlEvent);
    if ( !_friendHelpShown && 
         fabs(_spriteLayer->getExternalForceValue()) == SpriteLayer::MAX_FORCE && 
         prevValue != _spriteLayer->getExternalForceValue() && ++_numMaxHits >= SHOW_AFTER)
    {
        addPopupMenu("INSTRUCTION",
                     "Ask for help ! For the body to move from rest, the force on the body should be greater than the friction force. "
                     "Try taking help from your friend! Press the seek help button on the left", true, false);

        Size visibleSize = Director::getInstance()->getVisibleSize();

        // seek help implementation
        auto menu = Menu::create();
        auto sprite = LayerColor::create(GREENISH, visibleSize.width/8, visibleSize.height/8);
        auto label = LabelTTF::create("Seek Help", "fonts/EraserDust.ttf", 20);
        label->setPosition(Vec2(visibleSize.width/16, visibleSize.height/16));
        sprite->addChild(label);

        auto friendButton = MenuItemSprite::create(sprite, sprite);
        friendButton->setCallback([this, menu](Ref * sender)-> void 
                                  {
                                        _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE*2, 
                                                                 SpriteLayer::MAX_FORCE*2, 
                                                                 0, this, cccontrol_selector(Challenge1::forceValueChanged));
                                        _spriteLayer->addAnotherPerson();
                                        _menuLayer->removeChild(menu);
                                  });

        friendButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        friendButton->setPosition(Vec2(-visibleSize.width/4, 0));
        menu->addChild(friendButton);
        _menuLayer->addChild(menu);

        _friendHelpShown = true;
    }
    prevValue = _spriteLayer->getExternalForceValue();
}

//////////////////////////////
// challenge2
//////////////////////////////
 
Scene* Challenge2::createScene(bool showInfo)
{
    return Challenge<Challenge2>::createScene(showInfo);
}

bool Challenge2::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }
    return true;
}

