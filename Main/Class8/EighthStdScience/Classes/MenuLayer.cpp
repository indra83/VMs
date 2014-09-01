#include "GameLayerScene.h"
#include "MenuLayer.h"
#include "BackGroundLayer.h"
#include "SpriteLayer.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const Color4B GREENISH(64,102,24,255);
static const int LAB_ZINDEX=0;
static const int POP_ZINDEX=1;
static const int SURF_ZINDEX=2;
static const float PADDING = 10.0;

class ControlSliderRollBack : public ControlSlider
{
    float _base;
public :
    
    static ControlSliderRollBack* create(const char* bgFile, const char* progressFile, const char* thumbFile, float base)
    {
        ControlSliderRollBack *pRet = new ControlSliderRollBack();
        Sprite *backgroundSprite = Sprite::create(bgFile);
        Sprite *progressSprite = Sprite::create(progressFile);
        Sprite *thumbSprite = Sprite::create(thumbFile);

        pRet->initWithSprites(backgroundSprite, progressSprite, thumbSprite);
        pRet->_base = base;
        pRet->autorelease();
        return pRet;
    }

    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        ControlSlider::onTouchEnded(pTouch, pEvent);
        setValue(_base);
    }

};

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


    //////////////////////////////
    // 2. common display controls
    _topMenu = Menu::create();
    _topMenu->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topMenu->setPosition(visibleSize.width, visibleSize.height);
    this->addChild(_topMenu);

    return true;
}

void MenuLayer::addForceMenu(float min, float max, float start, Ref * target, Control::Handler handler)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (_forceSlider)
        removeChild(_forceSlider);

    _forceSlider = Layer::create();
    auto labelForce = LabelTTF::create("Change Force :", "fonts/Marker Felt.ttf", 35);
    labelForce->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4 + 20));
    _forceSlider->addChild(labelForce);

    auto slider = ControlSliderRollBack::create("sliderTrack.png", "sliderProgress.png" ,"slider_handle.png", start);
    slider->setAnchorPoint(Vec2(0.5f, 0.5f));
    slider->setMinimumValue(min); // Sets the min value of range
    slider->setMaximumValue(max); // Sets the max value of range
    slider->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4 - 20));
    slider->setValue(start);

    // When the value of the slider will change, the given selector will be called
    slider->addTargetWithActionForControlEvents(target, handler, Control::EventType::VALUE_CHANGED);
    _forceSlider->addChild(slider);

    std::stringstream sstr_min;
    sstr_min << (int)min;
    auto labelL = LabelTTF::create(sstr_min.str(), "fonts/Marker Felt.ttf", 25);
    labelL->setPosition(Vec2(slider->getPosition().x - slider->getContentSize().width/2, slider->getPosition().y - 30));
    _forceSlider->addChild(labelL);
    /*
    auto labelM = LabelTTF::create("0", "fonts/Marker Felt.ttf", 25);
    labelM->setPosition(Vec2(slider->getPosition().x, slider->getPosition().y - 30 ));
    addChild(labelM);
    */
    std::stringstream sstr_max;
    sstr_max << (int)max;
    auto labelH = LabelTTF::create(sstr_max.str(), "fonts/Marker Felt.ttf", 25);
    labelH->setPosition(Vec2(slider->getPosition().x + slider->getContentSize().width/2, slider->getPosition().y - 30));
    _forceSlider->addChild(labelH);
    addChild(_forceSlider);
}

void MenuLayer::addSurfaceMenu(std::function<void (Ref *)> cb)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // surface selection section
    auto surf_label = LabelTTF::create("Set Friction :" , "fonts/Marker Felt.ttf" , 30);
    surf_label->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/6));
    this->addChild(surf_label , SURF_ZINDEX);

    auto surf_ice = MenuItemImage::create("low-fric.png" , "low-fric.png", cb);
    surf_ice->setTag((int)MenuLayer::ICE);

    auto surf_grass = MenuItemImage::create("med-fric.png" , "med-fric.png", cb);
    surf_grass->setTag((int)MenuLayer::GRASS);

    auto surf_gravel = MenuItemImage::create("high-fric.png" , "high-fric.png", cb);
    surf_gravel->setTag((int)MenuLayer::GRAVEL);

    auto menu = Menu::create(surf_ice , surf_grass , surf_gravel ,nullptr);
    menu->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/13));
    menu->alignItemsHorizontally();
    this->addChild(menu , SURF_ZINDEX);
}

void MenuLayer::addToTopMenu(MenuItem * item, const std::function< void (Ref *)> &cb)
{
    item->setCallback(cb);
    item->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topMenuOffsetX -= PADDING;
    item->setPosition(_topMenuOffsetX, -PADDING);
    _topMenuOffsetX -= item->getContentSize().width * item->getScale();
    _topMenu->addChild(item);
}

MenuLayer::~MenuLayer()
{ 
}
