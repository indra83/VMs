#include "PopUpScene.h"
#include "Util.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* PopUp::createScene(const std::string &title, const std::string &caption, Sprite * sprite)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = PopUp::create();
    layer->_labelTitle->setString(title);
    layer->_labelCaption->setString(caption);

    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer, 1);
    if (sprite)
    {
        sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        scene->addChild(sprite, 0);
    }

    // return the scene
    return scene;
}

bool PopUp::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

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
                    Director::getInstance()->popScene();
                });
    auto menu = Menu::create(close , nullptr);
    menu->setPosition(Vec2(inf_wd, inf_ht));
    bgLayer->addChild(menu, 1);

    _labelTitle = LabelTTF::create("", "fonts/EraserDust.ttf", 40);
    _labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _labelTitle->setPosition(Vec2(inf_wd/2, inf_ht - 20.0 ));
    bgLayer->addChild(_labelTitle, 2);
    _labelCaption = LabelTTF::create("", "fonts/EraserDust.ttf", 30, Size(visibleSize.width/3, visibleSize.height/4));
    _labelCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelCaption->setPosition(Vec2(inf_wd/2, inf_ht/2));
    bgLayer->addChild(_labelCaption, 2);

    this->addChild(bgLayer);
 
    this->setKeypadEnabled(true);

    return true;
}

void PopUp::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
    Director::getInstance()->popScene();
}

