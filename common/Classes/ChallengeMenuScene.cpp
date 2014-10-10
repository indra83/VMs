#include "ChallengeMenuScene.h"
#include "Util.h"
#include "VmBase.h"
#include "SimpleAudioEngine.h"

#include "extensions/cocos-ext.h"

#define LABEL_OFFSET 45

USING_NS_CC;
USING_NS_CC_EXT;

static const std::string CHIME("audio/notification.mp3");

Scene* ChallengeMenu::createScene(bool fromChallenge, Sprite * sprite)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = ChallengeMenu::create();

    layer->_fromChallenge = fromChallenge;
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
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME.c_str());

    // green board layer
    auto bglayer = LayerColor::create(GREENISH , visibleSize.width , visibleSize.height);
    this->addChild(bglayer);

    populateWithInfo();
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
    
    auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
    if (!base)
        return;

    std::stringstream sstr;
    sstr << sceneId;
    auto gen = base->getChallengeInitializers()[sstr.str()];
    Scene * scene = gen ? gen(true) : nullptr;
    if (!scene)
        return;

    if (_fromChallenge)
    {
        Director::getInstance()->popScene();
        Director::getInstance()->replaceScene(scene);
    }
    else
        Director::getInstance()->pushScene(scene);
}

void ChallengeMenu::onInfo(const ChallengeInfo & info)
{
    populateWithInfo();
}

void ChallengeMenu::populateWithInfo()
{
    if (_scrollview)
        this->removeChild(_scrollview);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // scroll container
    auto scrollContainer = Layer::create();
    scrollContainer->setContentSize(Size(visibleSize.width , 1.5 * visibleSize.height));
    scrollContainer->setAnchorPoint(Vec2::ZERO);
    scrollContainer->setPosition(Vec2::ZERO);

    auto wd = scrollContainer->getContentSize().width;
    auto ht = scrollContainer->getContentSize().height;

    int n = 0;
    auto menu = Menu::create();
    // container item icons
    auto addChallenge =
    [&](std::string fileName, int id, const std::string & title, const std::string &objective) -> void
    {
        auto chal = MenuItemImage::create(fileName, fileName, CC_CALLBACK_1(ChallengeMenu::touchDownAction, this));
        chal->setTag(id);
        chal->setPosition(wd/(n+1), (n - id) * ht/n);
        menu->addChild(chal);

        auto label = LabelTTF::create(title, "fonts/EraserDust.ttf" , 40);
        label->setAnchorPoint(Vec2::ZERO);
        label->setPosition(2*wd/(n+1), (n - id) * ht/n + LABEL_OFFSET);
        menu->addChild(label);
        auto obj = LabelTTF::create( std::string("Objective: ") + objective,
                "fonts/EraserDust.ttf" , 25 ,
                Size(550,100) , TextHAlignment::LEFT);
        obj->setAnchorPoint(Vec2::ZERO);
        obj->setPosition(Vec2(2*wd/(n+1), (n - id) * ht/n - 2*LABEL_OFFSET));
        scrollContainer->addChild(obj);
    };

    
    auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
    if (!base)
        return;

    n = base->getChallengeInitializers().size() + 1;
    for (auto keyValue : base->getChallengeInitializers())
    {
        // TODO: this should be in the xml
        std::stringstream sstr;
        sstr << "chal_" << keyValue.first << ".png";
        ChallengeInfo & ch = base->getChallengeInfo()[keyValue.first];
        addChallenge(sstr.str(), atoi(keyValue.first.c_str()), ch._title, ch._desc);
    }

    menu->setPosition(Vec2::ZERO);
    scrollContainer->addChild(menu);

    // challenge label
	auto label = LabelTTF::create("CHALLENGES" , "fonts/EraserDust.ttf" , 60);
	label->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + 1.5 * visibleSize.height - label->getContentSize().height));
	scrollContainer->addChild(label);

    // scroll view
    _scrollview = ScrollView::create(visibleSize, scrollContainer);
    _scrollview->setDirection(ScrollView::Direction::VERTICAL);
    _scrollview->setPosition(Vec2::ZERO);
    _scrollview->setContentOffset(Vec2(0 , - visibleSize.height/2), true);

    this->addChild(_scrollview);
}

