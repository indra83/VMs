#include "ChallengeScene.h"
#include "GameLoadScene.h"

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
    scrollContainer->setAnchorPoint(Vec2::ZERO);
    scrollContainer->setPosition(Vec2::ZERO);
    scrollContainer->setContentSize(Size(visibleSize.width , 2*visibleSize.height));
    this->addChild(scrollContainer);


    auto wd = scrollContainer->getContentSize().width;
    auto ht = scrollContainer->getContentSize().height;

    // container item icons
    auto chal_1 = Sprite::create("chalbox.png");
    chal_1->setPosition(wd/5 , 4*ht/5);
    scrollContainer->addChild(chal_1);

    auto chal_2 = Sprite::create("chalbox.png");
    chal_2->setPosition(wd/5 , 3*ht/5);
    scrollContainer->addChild(chal_2);

    auto chal_3 = Sprite::create("chalbox.png");
    chal_3->setPosition(wd/5 , 2*ht/5);
    scrollContainer->addChild(chal_3);

    auto chal_4 = Sprite::create("chalbox.png");
    chal_4->setPosition(wd/5 , ht/5);
    scrollContainer->addChild(chal_4);

    // container item info
    auto label_1 = LabelTTF::create("challenge 1" , "fonts/Marker Felt.ttf" , 20);
    label_1->setPosition(3*wd/5 , 4*ht/5);
    scrollContainer->addChild(label_1);

    auto label_2 = LabelTTF::create("challenge 2" , "fonts/Marker Felt.ttf" , 20);
    label_2->setPosition(3*wd/5 , 3*ht/5);
	scrollContainer->addChild(label_2);

	auto label_3 = LabelTTF::create("challenge 3" , "fonts/Marker Felt.ttf" , 20);
	label_3->setPosition(3*wd/5 , 2*ht/5);
	scrollContainer->addChild(label_3);

	auto label_4 = LabelTTF::create("challenge 4" , "fonts/Marker Felt.ttf" , 20);
	label_4->setPosition(3*wd/5 , ht/5);
	scrollContainer->addChild(label_4);


    // scroll view
    auto scrollview = ScrollView::create(visibleSize, scrollContainer);
    scrollview->setDirection(ScrollView::Direction::VERTICAL);
   	scrollview->setPosition(Vec2::ZERO);

   	this->addChild(scrollview);

    return true;

}

void Challenge::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->popScene();
}

