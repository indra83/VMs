#include "PopUpScene.h"
#include "Util.h"

USING_NS_CC;

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

    _bgLayer = LayerColor::create(GREENISH, visibleSize.width/2, visibleSize.height/2);
    _bgLayer->setPosition(Vec2(visibleSize.width/4, visibleSize.height/4));

    auto inf_wd = _bgLayer->getContentSize().width;
    auto inf_ht = _bgLayer->getContentSize().height;

    auto close = MenuItemImage::create("close.png",
                "close.png", 
                [=](Ref * sender) -> void
                {
                    // CCLOG("CLOSE PRESSED");
                    Director::getInstance()->popScene();
                });
    auto menu = Menu::create(close , nullptr);
    menu->setPosition(Vec2(inf_wd, inf_ht));
    _bgLayer->addChild(menu, 1);

    _labelTitle = LabelTTF::create("", "fonts/EraserDust.ttf", 40);
    _labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _labelTitle->setPosition(Vec2(inf_wd/2, inf_ht - 20.0 ));
    _bgLayer->addChild(_labelTitle, 2);
    _labelCaption = LabelTTF::create("", "fonts/EraserDust.ttf", 30, Size(visibleSize.width/2, visibleSize.height/3));
    _labelCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelCaption->setPosition(Vec2(inf_wd/2, inf_ht/2 - 40));
    _bgLayer->addChild(_labelCaption, 2);

    this->addChild(_bgLayer);


    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = CC_CALLBACK_2(PopUp::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(PopUp::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PopUp::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

 
    this->setKeypadEnabled(true);

    return true;
}

bool PopUp::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    // CCLOG("Touch Began");

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if(_flag == true)
    {
        auto touchX = touch->getLocation().x;
        auto touchY = touch->getLocation().y;

        auto exp1 = visibleSize.width/2 - _bgLayer->getContentSize().width/2;
        auto exp2 = visibleSize.width/2 + _bgLayer->getContentSize().width/2;

        auto exp3 = visibleSize.height/2 - _bgLayer->getContentSize().height/2;
        auto exp4 = visibleSize.height/2 + _bgLayer->getContentSize().height/2;

        if( (touchX > exp1 && touchX < exp2) && (touchY > exp3 && touchY < exp4) )
        {
            // CCLOG("INTERNALLY TOUCHED");
            return false;
        }
        else
        {
            // CCLOG("EXTERNALLY TOUCHED");
            return true;
        }
    }
    else
        return true;
}

void PopUp::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    // CCLOG("Touch Moved");
}

void PopUp::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if(_flag) // for one time checking
    {
        Director::getInstance()->popScene();
        // CCLOG("SCENE POPPED");
        _flag = false;
    }

    CCLOG("Touch Ended");
}

void PopUp::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
    Director::getInstance()->popScene();
}

