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

    auto labelForce = LabelTTF::create("Change Force :", "fonts/Marker Felt.ttf", 35);
    labelForce->setPosition(Point(visibleSize.width/2, visibleSize.height/4 + 20));
    addChild(labelForce);

    auto slider = ControlSliderRollBack::create("sliderTrack.png", "sliderProgress.png" ,"slider_handle.png", start);
    slider->setAnchorPoint(Point(0.5f, 0.5f));
    slider->setMinimumValue(min); // Sets the min value of range
    slider->setMaximumValue(max); // Sets the max value of range
    slider->setPosition(Point(visibleSize.width/2, visibleSize.height/4 - 20));
    slider->setValue(start);

    // When the value of the slider will change, the given selector will be called
    slider->addTargetWithActionForControlEvents(target, handler, Control::EventType::VALUE_CHANGED);
    addChild(slider);

    std::stringstream sstr_min;
    sstr_min << (int)min;
    auto labelL = LabelTTF::create(sstr_min.str(), "fonts/Marker Felt.ttf", 25);
    labelL->setPosition(Point(slider->getPosition().x - slider->getContentSize().width/2, slider->getPosition().y - 30));
    addChild(labelL);
    /*
    auto labelM = LabelTTF::create("0", "fonts/Marker Felt.ttf", 25);
    labelM->setPosition(Point(slider->getPosition().x, slider->getPosition().y - 30 ));
    addChild(labelM);
    */
    std::stringstream sstr_max;
    sstr_max << (int)max;
    auto labelH = LabelTTF::create(sstr_max.str(), "fonts/Marker Felt.ttf", 25);
    labelH->setPosition(Point(slider->getPosition().x + slider->getContentSize().width/2, slider->getPosition().y - 30));
    addChild(labelH);
}

void MenuLayer::addFrictionMenu(Ref * target, Control::Handler handler)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto labelF = LabelTTF::create("Change Friction :", "fonts/Marker Felt.ttf", 30);
    labelF->setPosition(Point(visibleSize.width/2, visibleSize.height/8));
    addChild(labelF);

    auto fSlider = ControlSlider::create("sliderTrack.png", "sliderProgress.png" ,"slider_handle.png");
    fSlider->setAnchorPoint(Point(0.5f, 0.5f));
    fSlider->setMinimumValue(0.0f); // Sets the min value of range
    fSlider->setMaximumValue(1.0f); // Sets the max value of range
    fSlider->setPosition(Point(visibleSize.width/2 , visibleSize.height/8 - 30));
    fSlider->setValue(0.5f);
    fSlider->setScale(0.75f);

    // When the value of the slider will change, the given selector will be called
    fSlider->addTargetWithActionForControlEvents(target, handler, Control::EventType::VALUE_CHANGED);
    addChild(fSlider);

    auto labelL1 = LabelTTF::create("Low", "fonts/Marker Felt.ttf", 24);
    labelL1->setPosition(Point(fSlider->getPosition().x - fSlider->getContentSize().width*fSlider->getScale()/2, fSlider->getPosition().y - 20));
    addChild(labelL1);
     auto labelH1 = LabelTTF::create("High", "fonts/Marker Felt.ttf", 24);
    labelH1->setPosition(Point(fSlider->getPosition().x + fSlider->getContentSize().width*fSlider->getScale()/2, fSlider->getPosition().y - 20));
    addChild(labelH1);
 
    //////////////////////////////
    // add label
    auto label = LabelTTF::create("Friction", "fonts/Marker Felt.ttf", 30);
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    addChild(label);
}

void MenuLayer::addToTopMenu(MenuItem * item)
{
    item->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    item->setPosition(-item->getContentSize().width/2, 0.0);
    _topMenu->addChild(item);
}

void MenuLayer::addPopupMenu(const std::string &title, const std::string & caption)
{

    if (!_popupLayer)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point origin = Director::getInstance()->getVisibleOrigin();

        _popupLayer = Layer::create();

        auto bgLayer = LayerColor::create(GREENISH, visibleSize.width/2, visibleSize.height/2);
        bgLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bgLayer->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
        _popupLayer->addChild(bgLayer);

        auto inf_wd = bgLayer->getContentSize().width;
        auto inf_ht = bgLayer->getContentSize().height;

        auto close = MenuItemImage::create("close.png", "close.png" , CC_CALLBACK_1(MenuLayer::disablePopUpMenu, this));
        auto menu = Menu::create(close , nullptr);
        menu->setPosition(Vec2(inf_wd, inf_ht));
        _popupLayer->addChild(menu, 1);

        // TODO : set positions
        _popupLabelTitle = LabelTTF::create("", "fonts/EraserDust.ttf", 40);
        _popupLabelCaption = LabelTTF::create("", "fonts/EraserDust.ttf", 30);
    }

    _popupLabelTitle->setString(title);
    _popupLabelCaption->setString(caption);

}

void MenuLayer::disablePopUpMenu(Ref * sender)
{
    this->removeChild(_popupLayer);
    _popupLayer = nullptr;
}

MenuLayer::~MenuLayer()
{ 
}
