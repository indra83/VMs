#include "GameLoadScene.h"
#include "BackGroundLayer.h"
//#include "SimpleAudioEngine.h"
#include "ChallengeScene.h"
#include "NativeHelper.h"

#define BG_ZINDEX 0
#define SP_ZINDEX 1

#define BOUNCE_SOUND "audio/crash.mp3"
#define ALTITUDE 150
#define INITIAL_RESTITUTION 0.8
#define DELTA_RESTITUTION 0.2
#define INITIAL_VIBRATION 1000
#define DELTA_VIBRATION 200

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
    
    _current_res = INITIAL_RESTITUTION;
    _vibration_length = INITIAL_VIBRATION;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 2. add the background layer
    auto bgLayer = BackGroundLayer::create();
    this->addChild(bgLayer, BG_ZINDEX);

    // crate with physics properties
    auto crate = Sprite::create("crate.png");
    crate->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + ALTITUDE));

    auto crate_body = PhysicsBody::createBox(crate->getContentSize(), PhysicsMaterial(0.5, INITIAL_RESTITUTION, 0.5));
    crate_body->setContactTestBitmask(0x1); // enable callbacks
    crate->setPhysicsBody(crate_body);
    this->addChild(crate, SP_ZINDEX);

    // ground layer with physics properties
    auto ground_node = Node::create();
    ground_node->setAnchorPoint(Point::ZERO);
    ground_node->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 5*visibleSize.height/6 + 12));

    auto ground_body = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(1, INITIAL_RESTITUTION, 0.5), 3);
    ground_body->setContactTestBitmask(0x1); // enable callbacks
    ground_node->setPhysicsBody(ground_body);
    this->addChild(ground_node);

    // collision detection listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactPreSolve = CC_CALLBACK_2(GameLoad::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(GameLoad::onContactPostSolve, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    // preloading sound effects
    //CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BOUNCE_SOUND);

    this->setKeypadEnabled(true);

    return true;
}


void GameLoad::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode , cocos2d::Event *event)
{
	Director::getInstance()->end();
}

bool GameLoad::onContactPreSolve(cocos2d::PhysicsContact &contact, cocos2d::PhysicsContactPreSolve &solve)
{
    if (_current_res > 0.0 )
    {
        solve.setRestitution(_current_res);
        _current_res -= DELTA_RESTITUTION;
    }
    return true;
}

void GameLoad::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve)
{
    if (solve.getRestitution() > 0.0)
    {
        //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BOUNCE_SOUND);
        // TODO: this should ideally have been based on the current impulse
        NativeHelper::vibrate(_vibration_length);
        _vibration_length -= DELTA_VIBRATION;
    }
    else
    {
        // TODO: go to the challenges screen
        // should be asynchrounous? generate an event, add a handler outside
    }
}
