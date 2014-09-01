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

void MenuLayer::addFrictionMenu(Ref * target, Control::Handler handler)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto labelF = LabelTTF::create("Change Friction :", "fonts/Marker Felt.ttf", 30);
    labelF->setPosition(Vec2(visibleSize.width/2, visibleSize.height/8));
    addChild(labelF);

    auto fSlider = ControlSlider::create("sliderTrack.png", "sliderProgress.png" ,"slider_handle.png");
    fSlider->setAnchorPoint(Vec2(0.5f, 0.5f));
    fSlider->setMinimumValue(0.0f); // Sets the min value of range
    fSlider->setMaximumValue(1.0f); // Sets the max value of range
    fSlider->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/8 - 30));
    fSlider->setValue(0.5f);
    fSlider->setScale(0.75f);

    // When the value of the slider will change, the given selector will be called
    fSlider->addTargetWithActionForControlEvents(target, handler, Control::EventType::VALUE_CHANGED);
    addChild(fSlider);

    auto labelL1 = LabelTTF::create("Low", "fonts/Marker Felt.ttf", 24);
    labelL1->setPosition(Vec2(fSlider->getPosition().x - fSlider->getContentSize().width*fSlider->getScale()/2, fSlider->getPosition().y - 20));
    addChild(labelL1);
     auto labelH1 = LabelTTF::create("High", "fonts/Marker Felt.ttf", 24);
    labelH1->setPosition(Vec2(fSlider->getPosition().x + fSlider->getContentSize().width*fSlider->getScale()/2, fSlider->getPosition().y - 20));
    addChild(labelH1);
 
    //////////////////////////////
    // add label
    auto label = LabelTTF::create("Friction", "fonts/Marker Felt.ttf", 30);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    addChild(label);
}

void MenuLayer::addToTopMenu(MenuItem * item, const std::function< void (Ref *)> &cb)
{
    item->setCallback(
            [cb, this]( Ref * sender) -> void
            {
                if (isShowingPopupMenu())
                    return;
                cb(sender);    
            });
    item->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topMenuOffsetX -= PADDING;
    item->setPosition(_topMenuOffsetX, -PADDING);
    _topMenuOffsetX -= item->getContentSize().width * item->getScale();
    _topMenu->addChild(item);
}

void MenuLayer::addPopupMenu(const std::string &title, const std::string & caption, std::function< bool () > cb )
{
    if (!_popupLayer)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto bgLayer = LayerColor::create(GREENISH, visibleSize.width/2, visibleSize.height/2);
        //bgLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bgLayer->setPosition(Vec2(visibleSize.width/4, visibleSize.height/4));

        auto inf_wd = bgLayer->getContentSize().width;
        auto inf_ht = bgLayer->getContentSize().height;

        auto close = MenuItemImage::create("close.png",
                "close.png", 
                [=](Ref * sender) -> void
                {
                    cb();
                });
        auto menu = Menu::create(close , nullptr);
        menu->setPosition(Vec2(inf_wd, inf_ht));
        bgLayer->addChild(menu, 1);

        _popupLabelTitle = LabelTTF::create("", "fonts/EraserDust.ttf", 40);
        _popupLabelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _popupLabelTitle->setPosition(Vec2(inf_wd/2, inf_ht - 20.0 ));
        bgLayer->addChild(_popupLabelTitle, 2);
        _popupLabelCaption = LabelTTF::create("", "fonts/EraserDust.ttf", 30 , Size(visibleSize.width/3, visibleSize.height/4));
        _popupLabelCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _popupLabelCaption->setPosition(Vec2(inf_wd/2, inf_ht/2));
        bgLayer->addChild(_popupLabelCaption, 2);

        _popupLayer = bgLayer;
        this->addChild(_popupLayer, 1);
    }

    // TODO: Change Size according to message
    // TODO: warp text
    _popupLabelTitle->setString(title);
    _popupLabelCaption->setString(caption);
}

void MenuLayer::disablePopUpMenu()
{
    this->removeChild(_popupLayer);
    _popupLayer = nullptr;
}

MenuLayer::~MenuLayer()
{ 
}
