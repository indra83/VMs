#include "Challenge.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"

static const int BG_ZINDEX=0;
static const int SP_ZINDEX=1;
static const int MN_ZINDEX=2;
//#define BUF_HT 15

USING_NS_CC;
USING_NS_CC_EXT;

template< class Derived >
Derived * Challenge<Derived>::create()
{
    auto *pRet = new Derived();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
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
    Point origin = Director::getInstance()->getVisibleOrigin();

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
            [&](float deltaX) -> void
            {
                _bgLayer->runAction(Place::create(_bgLayer->getPosition() + Point(-deltaX, 0.0)));
            });
    _spriteLayer->changeForceValue(0.0);
    this->addChild(_spriteLayer, SP_ZINDEX);

    /////////////////////////////
    // 3. add the menu layer
    _menuLayer = MenuLayer::create();

    // add the reset button
    auto restart_scene = MenuItemImage::create("reset_normal.png",
                                               "reset_normal.png",
                                               CC_CALLBACK_1(Challenge::restartScene , this));
    restart_scene->setScale(0.8);
    restart_scene->setPosition(-restart_scene->getContentSize().width/2, -restart_scene->getContentSize().height/2);
    _menuLayer->addToTopMenu(restart_scene);

    this->addChild(_menuLayer, MN_ZINDEX);

    // enable keypress cbs
    this->setKeypadEnabled(true);

    return true;
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
void Challenge<Derived>::restartScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(Derived::createScene());
}

template< class Derived >
void Challenge<Derived>::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	Director::getInstance()->popScene();
}


//////////////////////////////
// challenge1

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

void Challenge1::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    Challenge<Challenge1>::forceValueChanged(sender, controlEvent);
    if ( !_friendHelpShown && fabs(_spriteLayer->getExternalForceValue()) == SpriteLayer::MAX_FORCE)
    {
        //_menuLayer->addPopup("ask friend's help");
        auto help = MenuItemImage::create("help.png", "help.png", CC_CALLBACK_1(Challenge1::helpClickedAction, this));
        auto menu = Menu::create(help, nullptr);
        //_menuLayer->addPopupButton(menu);
        _spriteLayer->showAnotherPerson(_spriteLayer->getExternalForceValue() > 0.0);
        _friendHelpShown = true;
    }
}

void Challenge1::helpClickedAction(Ref * sender)
{
    //_menuLayer->dropPopUp();
    _spriteLayer->addAnotherPerson();
}
