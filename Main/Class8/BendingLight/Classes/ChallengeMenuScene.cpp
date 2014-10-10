#include "ChallengeMenuScene.h"

#include "SimpleAudioEngine.h"
#include "Util.h"
#include "Challenge.h"

#include "extensions/cocos-ext.h"

#define LABEL_OFFSET 45

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

Scene* ChallengeMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = ChallengeMenu::create();

    // add layer as a child to scene
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

	//////////////////////////////
	// 2. Challenge Scene

	// preload music and play music
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME.c_str());

	// green board layer
	auto bglayer = LayerColor::create(GREENISH , visibleSize.width , visibleSize.height);
	this->addChild(bglayer);

	// scroll container
	auto scrollContainer = Layer::create();
	scrollContainer->setContentSize(Size(visibleSize.width , 1.5 * visibleSize.height));
	scrollContainer->setAnchorPoint(Vec2::ZERO);
	scrollContainer->setPosition(Vec2::ZERO);
	this->addChild(scrollContainer);


	auto wd = scrollContainer->getContentSize().width;
	auto ht = scrollContainer->getContentSize().height;

	auto menu = Menu::create();
	// container item icons
	auto addChallenge =
	[&](std::string fileName, int id, const std::string &objective) -> void
	{
		auto chal = MenuItemImage::create(fileName, fileName, CC_CALLBACK_1(ChallengeMenu::touchDownAction, this));
		chal->setTag(id);
		chal->setPosition(wd/5, (4 - id) * ht/4);
		menu->addChild(chal);

		std::stringstream sstr;
		sstr << "challenge " << id;
		auto label = LabelTTF::create(sstr.str(), "fonts/EraserDust.ttf" , 40);
		label->setAnchorPoint(Vec2::ZERO);
		label->setPosition(2*wd/5, (4 - id) * ht/4 + LABEL_OFFSET);
		menu->addChild(label);
		auto obj = LabelTTF::create( std::string("Objective: ") + objective,
				"fonts/EraserDust.ttf" , 25 ,
				Size(550,100) , TextHAlignment::LEFT);
		obj->setAnchorPoint(Vec2::ZERO);
		obj->setPosition(Vec2(2*wd/5, (4 - id) * ht/4 - 2*LABEL_OFFSET));
		scrollContainer->addChild(obj);
	};

	addChallenge("chal.png", 1, "Dummy Text 1");
	addChallenge("chal.png", 2, "Dummy Text 2");
	addChallenge("chal.png", 3, "Dummy Text 3");

	menu->setPosition(Vec2::ZERO);
	scrollContainer->addChild(menu);

	// challenge label
	auto label = LabelTTF::create("CHALLENGES" , "fonts/EraserDust.ttf" , 60);
	label->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + 1.5 * visibleSize.height - label->getContentSize().height));
	scrollContainer->addChild(label);

	// scroll view
	// TODO: investigate random crashes in srollview update
	auto scrollview = ScrollView::create(visibleSize, scrollContainer);
	scrollview->setDirection(ScrollView::Direction::VERTICAL);
	scrollview->setPosition(Vec2::ZERO);
	scrollview->setContentOffset(Vec2(0 , - visibleSize.height/2), true);


	this->setKeypadEnabled(true);
	return true;
}

void ChallengeMenu::touchDownAction(Ref *sender)
{
    int sceneId = dynamic_cast<Node *>(sender)->getTag();
    Scene * scene = nullptr;
    switch(sceneId) {
        case 1 :
            scene = Challenge1::createScene(true);
            break;
        case 2 :
        	scene = Challenge2::createScene(true);
        	break;
        case 3 :
        	scene = Challenge3::createScene(true);
        	break;
    };

    if (_fromChallenge)
    {
        Director::getInstance()->popScene();
        Director::getInstance()->replaceScene(scene);
    }
    else
        Director::getInstance()->pushScene(scene);
}

void ChallengeMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	Director::getInstance()->end();
}
