#include "MenuLayer.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const int LAB_ZINDEX=0;
static const int POP_ZINDEX=1;
static const int SURF_ZINDEX=2;

// on "init" you need to initialize your instance
bool MenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

void MenuLayer::setTopMenu(cocos2d::Menu * menu) 
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _topMenu = menu;
    _topMenu->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topMenu->setPosition(visibleSize.width, visibleSize.height);
    this->addChild(_topMenu);
}

MenuLayer::~MenuLayer()
{ 
}
