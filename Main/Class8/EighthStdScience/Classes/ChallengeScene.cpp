#include "ChallengeScene.h"
#include "GameLoadScene.h"
#include "Challenge1.h"
#include "ControlButtonUsable.h"

#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* Challenge::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = Challenge::create();

    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Challenge::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    // scroll container
    auto scrollContainer = Layer::create();
    scrollContainer->setContentSize(Size(visibleSize.width , 2*visibleSize.height));
    scrollContainer->setAnchorPoint(Vec2::ZERO);
    scrollContainer->setPosition(Vec2::ZERO);
    this->addChild(scrollContainer);


    auto wd = scrollContainer->getContentSize().width;
    auto ht = scrollContainer->getContentSize().height;

    // container item icons
    auto addChallenge =
    [&](std::string fileName, int id) -> void
    {
        auto chal_sprite = Scale9Sprite::create(fileName);
        auto chal = ControlButtonUsable::create("Play", chal_sprite);
        chal->setTag(id);
        chal->setTouchDownHandler(this, cccontrol_selector(Challenge::touchDownAction));
        chal->setPosition(wd/5, (5 - id) * ht/5);
        scrollContainer->addChild(chal);

        std::stringstream sstr;
        sstr << "challenge " << id;
        auto label = LabelTTF::create(sstr.str(), "fonts/Marker Felt.ttf" , 20);
        label->setPosition(3*wd/5, (5 - id) * ht/5);
        scrollContainer->addChild(label);
    };

    // TODO: need to create a proper scale9 png for this
    addChallenge("chalbox.9.png", 1);
    addChallenge("chalbox.9.png", 2);
    addChallenge("chalbox.9.png", 3);
    addChallenge("chalbox.9.png", 4);

    // scroll view
    auto scrollview = ScrollView::create(visibleSize, scrollContainer);
    scrollview->setDirection(ScrollView::Direction::VERTICAL);
    scrollview->setPosition(Vec2::ZERO);
    scrollview->setContentOffset(Vec2(0 , -visibleSize.height), true);

    this->addChild(scrollview);

    this->setKeypadEnabled(true);

    return true;
}

void Challenge::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
        Director::getInstance()->popScene();
}

void Challenge::touchDownAction(Ref *sender, Control::EventType controlEvent)
{
    int sceneId = dynamic_cast<Node *>(sender)->getTag();
    auto scene = Challenge1::createScene();
    Director::getInstance()->replaceScene(scene);
}

