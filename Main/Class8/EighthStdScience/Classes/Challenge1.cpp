#include "Challenge1.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"

static const int BG_ZINDEX=0;
static const int SP_ZINDEX=1;
static const int MN_ZINDEX=2;
//#define BUF_HT 15

USING_NS_CC;
USING_NS_CC_EXT;

// on "init" you need to initialize your instance
bool Challenge1::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    // add the force menu
    _menuLayer->addForceMenu(this, cccontrol_selector(Challenge1::forceValueChanged));

    this->addChild(_menuLayer, MN_ZINDEX);

    // enable keypress cbs
    this->setKeypadEnabled(true);

    return true;
}

