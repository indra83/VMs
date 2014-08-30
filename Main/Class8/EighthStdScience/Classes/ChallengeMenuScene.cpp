#include "ChallengeMenuScene.h"
#include "GameLoadScene.h"
#include "Challenge.h"
//#include "SimpleAudioEngine.h"

#include "extensions/cocos-ext.h"

#define GREENISH Color4B(64,102,24,255)
//#define CHIME "audio/notification.mp3"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* ChallengeMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = ChallengeMenu::create();

    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ChallengeMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // preload music and play music
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(CHIME);
//    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME);

    // green board layer
    auto bglayer = LayerColor::create(GREENISH , visibleSize.width , visibleSize.height);
    this->addChild(bglayer);

    // scroll container
    auto scrollContainer = Layer::create();
    scrollContainer->setContentSize(Size(visibleSize.width , 2*visibleSize.height));
    scrollContainer->setAnchorPoint(Vec2::ZERO);
    scrollContainer->setPosition(Vec2::ZERO);
    this->addChild(scrollContainer);


    auto wd = scrollContainer->getContentSize().width;
    auto ht = scrollContainer->getContentSize().height;

    auto menu = Menu::create();
    // container item icons
    auto addChallenge =
    [&](std::string fileName, int id) -> void
    {
        auto chal = MenuItemImage::create(fileName, fileName, CC_CALLBACK_1(ChallengeMenu::touchDownAction, this));
        chal->setTag(id);
        chal->setPosition(wd/5, (5 - id) * ht/5);
        menu->addChild(chal);

        std::stringstream sstr;
        sstr << "challenge " << id;
        auto label = LabelTTF::create(sstr.str(), "fonts/EraserDust.ttf" , 40);
        label->setPosition(3*wd/5, (5 - id) * ht/5);
        menu->addChild(label);
    };

    addChallenge("chal_1.png", 1);
    addChallenge("chl.png", 2);
    addChallenge("chl.png", 3);
    addChallenge("chl.png", 4);

    menu->setPosition(Vec2::ZERO);
    scrollContainer->addChild(menu);

    // challenge label
	auto label = LabelTTF::create("CHALLENGES" , "fonts/EraserDust.ttf" , 60);
	label->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + 2*visibleSize.height - label->getContentSize().height));
	scrollContainer->addChild(label);

    // scroll view
    auto scrollview = ScrollView::create(visibleSize, scrollContainer);
    scrollview->setDirection(ScrollView::Direction::VERTICAL);
    scrollview->setPosition(Vec2::ZERO);
    scrollview->setContentOffset(Vec2(0 , -visibleSize.height), true);

    this->addChild(scrollview);

    this->setKeypadEnabled(true);

    return true;
}

void ChallengeMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
        Director::getInstance()->popScene();
}

void ChallengeMenu::touchDownAction(Ref *sender)
{
    int sceneId = dynamic_cast<Node *>(sender)->getTag();
    auto scene = Challenge1::createScene();
    // TODO: this should be conditional on how we got to the challengescreen
    Director::getInstance()->pushScene(scene);
}

