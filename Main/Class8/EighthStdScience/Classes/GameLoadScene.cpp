#include "GameLoadScene.h"
#include "BackGroundLayer.h"
#include "SimpleAudioEngine.h"
#include "ChallengeScene.h"

#define BG_ZINDEX 0
#define SP_ZINDEX 1

USING_NS_CC;

Scene* GameLoad::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, -400));
    
    // 'layer' is an autorelease object
    auto layer = GameLoad::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLoad::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 2. add the background layer
    auto bgLayer = BackGroundLayer::create();
    this->addChild(bgLayer, BG_ZINDEX);

#define ALT 100
    // crate with physics properties
    auto crate = Sprite::create("crate.png");
    crate->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2 + ALT));

    auto crate_body = PhysicsBody::createBox( crate->getContentSize() , PhysicsMaterial(0,1,0));
    crate_body->setCollisionBitmask(1);
    crate_body->setContactTestBitmask(true);
    crate->setPhysicsBody(crate_body);
    this->addChild(crate);

    // ground layer with physics properties
    auto ground_node = Node::create();
    ground_node->setAnchorPoint(Point::ZERO);
    ground_node->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + 5*visibleSize.height/6 + 12));

    auto ground_body = PhysicsBody::createEdgeBox(visibleSize , PhysicsMaterial(1,0.8,0.5) , 3);
    ground_body->setCollisionBitmask(2);
    ground_body->setContactTestBitmask(true);
    ground_node->setPhysicsBody(ground_body);
    this->addChild(ground_node);

    // collision detection listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLoad::onContactBegin , this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener , this);

    // preloading sound effects
#define SOUND "audio/crash.mp3"

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(SOUND);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND);

    this->setKeypadEnabled(true);

    return true;
}


void GameLoad::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->end();
}

bool GameLoad::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	int count = 0;

	// check if the bodies are collided
	if((1 == a->getCategoryBitmask() && 2 == b->getCategoryBitmask())
			|| (2 == a->getCategoryBitmask() && 1 == b->getCategoryBitmask()))
	{

		count += 1;
		if(count == 1)	// restricting the sound effect to occour only once
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND);
		}

//		auto scene = Challenge::createScene();
//		Director::getInstance()->pushScene(scene);
	}

	return true;

}

