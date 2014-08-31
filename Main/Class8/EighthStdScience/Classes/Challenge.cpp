#include "Challenge.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"
#include "ChallengeMenuScene.h"

static const int BG_ZINDEX=0;
static const int SP_ZINDEX=1;
static const int MN_ZINDEX=2;
static const int INF_ZINDEX=3;

USING_NS_CC;
USING_NS_CC_EXT;

template< class Derived >
std::function<bool ()> Challenge<Derived>::RETURN_FALSE([]()-> bool { return false; });

template< class Derived >
Derived * Challenge<Derived>::create()
{
    auto *pRet = new Derived();
    if (pRet && pRet->init())
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
Scene* Challenge<Derived>::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge<Derived>::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
template< class Derived >
bool Challenge<Derived>::init()
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
    // 2. add the sprite layer
    _spriteLayer = SpriteLayer::create();
    _spriteLayer->setMass(30.0);
    _spriteLayer->setFrictionCoefficient(0.5);
    _spriteLayer->setMoveCB(
            [this](float deltaX) -> void
            {
                _bgLayer->runAction(Place::create(_bgLayer->getPosition() + Vec2(-deltaX, 0.0)));
            });
    _spriteLayer->changeForceValue(0.0);
    this->addChild(_spriteLayer, SP_ZINDEX);

    /////////////////////////////
    // 3. add the menu layer
    _menuLayer = MenuLayer::create();

    // add the reset button
    auto restart_scene = MenuItemImage::create("reset_normal.png", "reset_normal.png");
    restart_scene->setScale(0.8);
    _menuLayer->addToTopMenu(restart_scene,
                             [](Ref * sender) -> void
                             {
                                Director::getInstance()->replaceScene(Derived::createScene());
                             });

    // add the info button
    auto info = MenuItemImage::create("info.png", "info.png");
    info->setScale(0.8);
    _menuLayer->addToTopMenu(info,
                             [this](Ref * sender) -> void
                             {
                                addPopupMenu("Objective", "Try to move the crate by changing the force applied");
                             });

    // add the challenges menu
    auto list = MenuItemImage::create("list.png", "list.png");
    list->setScale(0.2);
    _menuLayer->addToTopMenu(list,
                             [](Ref * sender) -> void
                             {
                                Director::getInstance()->pushScene(ChallengeMenu::createScene(true));
                             });

    this->addChild(_menuLayer, MN_ZINDEX);

    // enable keypress cbs
    this->setKeypadEnabled(true);

    return true;
}


template< class Derived >
void Challenge<Derived>::addPopupMenu(const std::string & title, const std::string & caption)
{
    auto closeCb = [this]() -> bool
    {
        if (_menuLayer->isShowingPopupMenu())  
        {
            _menuLayer->disablePopUpMenu();
            setBackCallBack(Challenge::RETURN_FALSE);
            if (_challengeOver)
                Director::getInstance()->popScene();
            return true;
        }
        return false;
    };

    setBackCallBack(closeCb);
    _menuLayer->addPopupMenu(title, caption, closeCb);
}

template< class Derived >
void Challenge<Derived>::done()
{
    _challengeOver = true;
    addPopupMenu("Challenge Complete", "Congrats!!");
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
    if (!_backCB())
    {
        Director::getInstance()->popScene();
    }
}

//////////////////////////////
// challenge1
//////////////////////////////

Scene* Challenge1::createScene()
{
    return Challenge<Challenge1>::createScene();
}

// on "init" you need to initialize your instance
bool Challenge1::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init() )
    {
        return false;
    }

    // add the force menu
    _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE, SpriteLayer::MAX_FORCE, 0, this, cccontrol_selector(Challenge1::forceValueChanged));

    return true;
}

#define SHOW_AFTER 3
void Challenge1::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    static float prevValue=0.0;
    Challenge<Challenge1>::forceValueChanged(sender, controlEvent);
    if ( !_friendHelpShown && 
         fabs(_spriteLayer->getExternalForceValue()) == SpriteLayer::MAX_FORCE && 
         prevValue != _spriteLayer->getExternalForceValue() && ++_numMaxHits >= SHOW_AFTER)
    {
        addPopupMenu("Ask for Help", 
                     "Not enough force, ask a friend to help out, by clicking the friend button on the top right"); 

        auto friendButton = MenuItemImage::create("help.png", "help.png");
        friendButton->setScale(0.25);
        _menuLayer->addToTopMenu(friendButton,
                                 [&](Ref * sender)-> void 
                                 {
                                    dynamic_cast< MenuItemImage * >( sender )->setEnabled(false);
                                    this->_menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE*2, SpriteLayer::MAX_FORCE*2, 0, this, cccontrol_selector(Challenge1::forceValueChanged));
                                    this->_spriteLayer->addAnotherPerson();
                                 }); 
        friendButton->retain();
        _friendHelpShown = true;
    }
    prevValue = _spriteLayer->getExternalForceValue();
}
