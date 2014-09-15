#include "GameLayerScene.h"
#include "MenuLayer.h"
#include "BackGroundLayer.h"
#include "SpriteLayer.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const int LAB_ZINDEX=0;
static const int POP_ZINDEX=1;
static const int SURF_ZINDEX=2;
static const float PADDING = 10.0;

class ControlSliderRollBack : public ControlSlider
{
    float _base;
    bool _forced;
public :
    
    static ControlSliderRollBack* create(const char* bgFile, const char* progressFile, const char* thumbFile, float base)
    {
        ControlSliderRollBack *pRet = new ControlSliderRollBack();
        Sprite *backgroundSprite = Sprite::create(bgFile);
        Sprite *progressSprite = Sprite::create(progressFile);
        Sprite *thumbSprite = Sprite::create(thumbFile);

        pRet->initWithSprites(backgroundSprite, progressSprite, thumbSprite);
        pRet->_base = base;
        pRet->_forced = false;
        pRet->autorelease();
        return pRet;
    }

    void forceSetValue(float value)
    {
        setValue(value);
        _forced = true;
    }

    bool onTouchBegan(Touch* pTouch, Event* pEvent) override
    {
        if(_forced)
            return false;
        return ControlSlider::onTouchBegan(pTouch, pEvent);
    }

    void onTouchMoved(Touch *pTouch, Event *pEvent) override
    {
        if(_forced)
            return;

        ControlSlider::onTouchMoved(pTouch, pEvent);
    }


    void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override
    {
        if(_forced)
        {
            _forced=false;
            return;
        }

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

    if (_forceLayer)
        removeChild(_forceLayer);

    _forceLayer = Layer::create();
    auto labelForce = LabelTTF::create("Change Force :", "fonts/Marker Felt.ttf", 35);
    labelForce->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4 + 20));
    _forceLayer->addChild(labelForce);

    _forceSlider = ControlSliderRollBack::create("sliderTrack.png", "sliderProgress.png" ,"slider_handle.png", start);
    _forceSlider->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _forceSlider->setMinimumValue(min); // Sets the min value of range
    _forceSlider->setMaximumValue(max); // Sets the max value of range
    // programatically change the value of the slider on change of force.
    
    auto scaleX = (max-min)/(2*SpriteLayer::MAX_FORCE);
    _forceSlider->getThumbSprite()->setScaleX(1/scaleX);
    _forceSlider->getSelectedThumbSprite()->setScaleX(1/scaleX);
    _forceSlider->setScaleX(scaleX);
    _forceSlider->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4 - 20));
    _forceSlider->setValue(start);

    // When the value of the slider will change, the given selector will be called
    _forceSlider->addTargetWithActionForControlEvents(target, handler, Control::EventType::VALUE_CHANGED);
    _forceLayer->addChild(_forceSlider);

    std::stringstream sstr_min;
    sstr_min << (int)min;
    auto labelL = LabelTTF::create(sstr_min.str(), "fonts/Marker Felt.ttf", 25);
    labelL->setPosition(Vec2(_forceSlider->getPosition().x - _forceSlider->getContentSize().width * _forceSlider->getScaleX()/2, 
                             _forceSlider->getPosition().y - 30));
    _forceLayer->addChild(labelL);

    std::stringstream sstr_max;
    sstr_max << (int)max;
    auto labelH = LabelTTF::create(sstr_max.str(), "fonts/Marker Felt.ttf", 25);
    labelH->setPosition(Vec2(_forceSlider->getPosition().x + _forceSlider->getContentSize().width * _forceSlider->getScaleX()/2,
                             _forceSlider->getPosition().y - 30));
    _forceLayer->addChild(labelH);
    addChild(_forceLayer);
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

void MenuLayer::setForceSliderValue(float force)
{
    if(!_forceSlider)
        return;
    _forceSlider->forceSetValue(force);
}

MenuLayer::~MenuLayer()
{ 
}
