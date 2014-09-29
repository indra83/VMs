#include "Challenge.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"
#include "ChallengeMenuScene.h"
#include "SimpleAudioEngine.h"
#include "NativeHelper.h"
#include "PopUpScene.h"
#include "Util.h"
#include "SimpleAudioEngine.h"

#include <iomanip> // setw, setfill

static const int BG_ZINDEX=0;
static const int SP_ZINDEX=1;
static const int MN_ZINDEX=2;


USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

template< class Derived >
Derived * Challenge<Derived>::create(bool showInfo)
{
    auto *pRet = new Derived();
    if (pRet && pRet->init(showInfo))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

template< class Derived >
Scene* Challenge<Derived>::createScene(bool showInfo)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Challenge<Derived>::create(showInfo);
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
template< class Derived >
bool Challenge<Derived>::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 1. add the background layer
    _bgLayer = BackGroundLayer::create();
    this->addChild(_bgLayer, BG_ZINDEX);

    /////////////////////////////
    // 2. add the menu layer
    _menuLayer = MenuLayer::create();

    // add the reset button
    auto restart_scene = MenuItemImage::create("reset_normal.png", "reset_normal.png");
    restart_scene->setScale(0.8);
    _menuLayer->addToTopMenu(restart_scene,
                             [](Ref * sender) -> void
                             {
                                Director::getInstance()->replaceScene(Derived::createScene(false));
                             });

    // add the info button
    auto info = MenuItemImage::create("info.png", "info.png");
    info->setScale(0.8);
    _menuLayer->addToTopMenu(info,
                             [this](Ref * sender) -> void
                             {
                                showInfoPopup();
                             });

    // add the challenges menu
    auto list = MenuItemImage::create("menu.png", "menu.png");
    list->setScale(0.8);
    _menuLayer->addToTopMenu(list,
                             [](Ref * sender) -> void
                             {
                                Director::getInstance()->pushScene(ChallengeMenu::createScene(true, nullptr));
                             });

    this->addChild(_menuLayer, MN_ZINDEX);

    /////////////////////////////
    // 3. add the sprite layer
    _spriteLayer = SpriteLayer::create();
    _spriteLayer->addToMovables(_bgLayer);
    _spriteLayer->changeForceValue(0.0);
    // hack to make life easier
    _spriteLayer->setMenuLayer(_menuLayer);
    this->addChild(_spriteLayer, SP_ZINDEX);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(CHIME.c_str());
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    // enable keypress cbs
    this->setKeypadEnabled(true);

    // show initial info popup
    if (showInfo)
        showInfoPopup();

    return true;
}

template< class Derived >
void Challenge<Derived>::addBaseSurface(MenuLayer::SurfaceType surf)
{
    _spriteLayer->setBaseSurface(MenuLayer::SURF_INFO[surf].coeff, MenuLayer::SURF_INFO[surf].sprite);
}

template< class Derived >
void Challenge<Derived>::addPopupMenu(const std::string & title, const std::string & caption, bool loud, bool vibrate, bool replace)
{
    if (loud)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CHIME.c_str());
    if (vibrate)
        NativeHelper::vibrate(500);

    captureScreenAsSprite(
                [=](Sprite * sprite) -> void
                {
                    auto scene = PopUp::createScene(title, caption, sprite);
                    if (replace)
                        Director::getInstance()->replaceScene(scene);
                    else
                        Director::getInstance()->pushScene(scene);
                });
}

template< class Derived >
void Challenge<Derived>::done(bool success)
{
    _spriteLayer->pause();
    if (success)
        addPopupMenu("Challenge Complete", "Congrats!!", true, true, true);
    else
        addPopupMenu("Failed!!", "Try Again", true, true, true);
}

template< class Derived >
void Challenge<Derived>::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    _spriteLayer->changeForceValue(pSlider->getValue());
}

template< class Derived >
void Challenge<Derived>::frictionValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    _spriteLayer->changeFrictionValue(pSlider->getValue());
}

template< class Derived >
void Challenge<Derived>::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    Director::getInstance()->popScene();
}

//////////////////////////////
// challenge1
//////////////////////////////

static const int SHOW_AFTER = 2;
static const int TARGET_METRES = 50;

Scene* Challenge1::createScene(bool showInfo)
{
    return Challenge<Challenge1>::createScene(showInfo);
}

// on "init" you need to initialize your instance
bool Challenge1::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

    _spriteLayer->setMass(30.0);
    addBaseSurface(MenuLayer::GRASS);
    // add the force menu
    _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE, SpriteLayer::MAX_FORCE, 0, this, cccontrol_selector(Challenge1::forceValueChanged));

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // adding destination sprite at 100 meters away from current screen
    // add destinations on both sides
    auto addDestination = [this, visibleSize] ( bool right ) -> void
    {
        auto gen = [](bool mini) -> Node *
        {
            auto dest = Sprite::create("destination.png");
            dest->setScale(0.8);
            dest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            return dest;
        };

        _spriteLayer->addStationaryChild(gen, Vec2( (right ? 1 : -1) * TARGET_METRES * SpriteLayer::PTM_RATIO, visibleSize.height/3 + 10));
    };

    addDestination(true);
    addDestination(false);

    Vec2 originalPos = _bgLayer->getPosition();
    _spriteLayer->setPeriodicCB([this, originalPos] (float unused, float dt) -> bool
    {
        if( fabs(_bgLayer->getPosition().x - originalPos.x) >= TARGET_METRES * SpriteLayer::PTM_RATIO )
        {
            done(true);
            return false;
        }
        return true;
    });

   return true;
}

void Challenge1::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    static float prevValue=0.0;
    Challenge<Challenge1>::forceValueChanged(sender, controlEvent);
    if ( !_friendHelpShown && 
         fabs(_spriteLayer->getExternalForceValue()) == SpriteLayer::MAX_FORCE && 
         prevValue != _spriteLayer->getExternalForceValue() && ++_numMaxHits >= SHOW_AFTER)
    {
        addPopupMenu("INSTRUCTION",
                     "Ask for help ! For the body to move from rest, the force on the body should be greater than the friction force. "
                     "Try taking help from your friend! Press the seek help button on the left", true, false);

        Size visibleSize = Director::getInstance()->getVisibleSize();

        // seek help implementation
        auto menu = Menu::create();
        auto sprite = LayerColor::create(GREENISH, visibleSize.width/8, visibleSize.height/8);
        auto label = LabelTTF::create("Seek Help", "fonts/EraserDust.ttf", 20);
        label->setPosition(Vec2(visibleSize.width/16, visibleSize.height/16));
        sprite->addChild(label);

        auto friendButton = MenuItemSprite::create(sprite, sprite);
        friendButton->setCallback([this, menu](Ref * sender)-> void 
                                  {
                                        _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE*2, 
                                                                 SpriteLayer::MAX_FORCE*2, 
                                                                 0, this, cccontrol_selector(Challenge1::forceValueChanged));
                                        _spriteLayer->addAnotherPerson();
                                        _menuLayer->removeChild(menu);
                                  });

        friendButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        friendButton->setPosition(Vec2(-visibleSize.width/4, 0));
        menu->addChild(friendButton);
        _menuLayer->addChild(menu);

        _friendHelpShown = true;
    }
    prevValue = _spriteLayer->getExternalForceValue();
}

void Challenge1::showInfoPopup()
{
	addPopupMenu("INSTRUCTION", "Move the box to the right by applying force on it. Drag and hold the slider in position to apply a specific amount of force."
	    		" Help the box reach the house which is some distance away", false);
}

//////////////////////////////
// challenge2
//////////////////////////////
 
static const float TROLLEY_VELOCITY = 5.0; // metres/sec
static const float INTERVAL = 60; // metres

Scene* Challenge2::createScene(bool showInfo)
{
    return Challenge<Challenge2>::createScene(showInfo);
}

bool Challenge2::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

    // add the force menu
    _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE, SpriteLayer::MAX_FORCE, 0, this, cccontrol_selector(Challenge2::forceValueChanged));

    _spriteLayer->setMass(15.0);
    addBaseSurface(MenuLayer::GRASS);

    Size visibleSize = Director::getInstance()->getVisibleSize();

    static const int PEOPLE_TAG = 1; 
    auto limitWidth = visibleSize.width / SpriteLayer::MINI_MAP_SCALE;

    // fill up the limits
    auto initTrollies = [=] (bool right) -> void
    {

        auto gen = [=](bool mini) -> Node *
        {
            auto trolley = Sprite::create("trolley.png");
            trolley->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            if (mini)
                return trolley;

            auto node = Node::create();
            node->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

            trolley->setPosition(Vec2::ZERO);
            node->addChild(trolley);

            auto people = Sprite::create("lift00.png");
            people->setPosition(Vec2(0.0, trolley->getContentSize().height * trolley->getScale()));
            people->setTag(PEOPLE_TAG);
            people->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            node->addChild(people, 1);

            /*
            Vector<SpriteFrame*> frames;
            for ( int i=0; i<=15; i++)
            {
                std::stringstream sstr;
                sstr << "lift" << std::setfill('0') << std::setw(2) << i << ".png";
                auto frame = SpriteFrame::create(sstr.str(), Rect(0.0, 0.0, people->getContentSize().width, people->getContentSize().height));
                frames.pushBack(frame);
            }
            Animation * anim = Animation::createWithSpriteFrames(frames, 0.1, -1);
            auto theAnim = Animate::create(anim); 
            people->runAction(theAnim);
            */

            return node;
        };

        auto widthCovered = -limitWidth;
        while(widthCovered < limitWidth)
        {
            auto trolley = _spriteLayer->addMovingChild(gen,
                    (right ? 1 : -1 ) * TROLLEY_VELOCITY, 
                    right ? SpriteLayer::TROLLEY_RIGHT_ZINDEX : SpriteLayer::TROLLEY_LEFT_ZINDEX,
                    Vec2( widthCovered, visibleSize.height/3 + 10),
                    false);
            _trollies.push_back(trolley);
            widthCovered += ( INTERVAL * SpriteLayer::PTM_RATIO );
        };
    };

    initTrollies(true);
    initTrollies(false);

    static std::map<Node *, float> sTimeInfo;
    sTimeInfo.clear();
    _spriteLayer->setPeriodicCB([=](float unused, float dt) -> bool
            {    
                auto move = [=]( Node * node, bool right) -> void
                {
                    auto direction = right ? 1 : -1;
                    node->setPosition(node->getPosition() + ( direction * Vec2(2*limitWidth, 0.0) ) );
                    auto miniNode = dynamic_cast< Node * >( node->getUserObject() ); 
                    if ( miniNode )
                        miniNode->setPosition(miniNode->getPosition() + ( direction * Vec2(2*limitWidth*SpriteLayer::MINI_MAP_SCALE, 0.0) ));
                };

                auto getPeople = [=]( Node * node ) -> Node *
                {
                    for (auto child : node->getChildren())
                        if (child->getTag() == PEOPLE_TAG)
                            return child;
                    return nullptr;
                };

                // periodically check where the current trolleys are.. add and remove sprites as they move out the visible region
                for( auto trolley : _trollies )                 
                {
                    if ( trolley->getPosition().x > visibleSize.width/2 + limitWidth )
                    {    
                        move(trolley, false);
                    }
                    if ( trolley->getPosition().x < visibleSize.width/2 - limitWidth)
                    {
                        move(trolley, true);
                    }

                    //CCLOG("++++++++++++++++ SMK: trolley(%x) - abs(%f, %f), pos(%f,%f)", 
                    //        trolley, getAbsolutePosition(trolley).x, getAbsolutePosition(trolley).y, trolley->getPosition().x, trolley->getPosition().y);
                    auto trolleySize = trolley->getContentSize() * trolley->getScale());
                    if ( trolley->getPosition().x + trolleySize.width/2 >= visibleSize.width/2
                       &&  trolley->getPosition().x - trolleySize.width/2 <= visibleSize.width/2 )
                    {
                        float &existing = sTimeInfo[trolley]; 
                        existing += dt;
                        
                        //get the people on top
                        auto people = getPeople(trolley);

                        auto peopleSize = people->getContentSize() * people->getScale();
                        auto skew = trolley->getPosition(); 
                        CCLOG("++++++++++++++ SMK :skew - (%f)", skew.x);
                        if (fabs(skew.x) < ( trolleySize.width/2 - peopleSize.width/2) )
                            people->setPosition(-skew.x, people->getPosition().y);

                        // TODO : explaination
                        static const float COINCIDENT_TIME = (trolley->getContentSize().width * trolley->getScale() / SpriteLayer::PTM_RATIO);
                        if (existing > COINCIDENT_TIME) 
                        {
                            done(true);
                            return false;
                        }
                    }
                    else
                    {
                        auto found = sTimeInfo.find(trolley);
                        if (found != sTimeInfo.end())
                            sTimeInfo.erase(found);
                    }
                };
                
                return true;
            });

    return true;
}

void Challenge2::showInfoPopup()
{
	addPopupMenu("INSTRUCTION", "This is a test message of challenge 2", false);
}

//////////////////////////////
// challenge3
//////////////////////////////

Scene* Challenge3::createScene(bool showInfo)
{
    return Challenge<Challenge3>::createScene(showInfo);
}

bool Challenge3::init(bool showInfo)
{
    //////////////////////////////
    // 1. super init first
    if ( !Challenge::init(showInfo) )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // pre-loading effect for blast
    SimpleAudioEngine::getInstance()->preloadEffect("audio/explosion.mp3");

    _spriteLayer->setMass(10.0);
    addBaseSurface(MenuLayer::GRASS);
    _spriteLayer->setMiniMapOffset(-visibleSize.width/2);

    //add destination
    static float TARGET_LOC = 6 * visibleSize.width;
    static float TARGET_LOC_END = 7 * visibleSize.width;

    auto gen = [](bool mini) -> Node *
    {
        auto dest = Sprite::create("destination.png");
        dest->setScale(0.8);
        dest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        return dest;
    };
    _spriteLayer->addStationaryChild(gen, Vec2(TARGET_LOC, visibleSize.height/3 + 10));


    auto gen2 = [] (bool mini) -> Node *
    {
        auto tanker = Sprite::create("tanker.png");
        tanker->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        return tanker;
    };
    _spriteLayer->addStationaryChild(gen2, Vec2(7 * visibleSize.width, visibleSize.height/3 + 10));

    // add countdown timer
    auto timeLabel = LabelTTF::create(getTimeString().c_str() , "fonts/digital-7.ttf" , 100);
    timeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    timeLabel->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height - (timeLabel->getContentSize().height)));
    timeLabel->setColor(Color3B::BLACK);
    this->addChild(timeLabel);
    _timeLabel = timeLabel;

    auto secLabel = LabelTTF::create("sec" , "fonts/Marker Felt.ttf" , 30);
    secLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    secLabel->setPosition(Vec2(origin.x + visibleSize.width/2 + timeLabel->getContentSize().width,
                origin.y + visibleSize.height - timeLabel->getContentSize().height));
    secLabel->setColor(Color3B::BLACK);
    this->addChild(secLabel);

    // add surface selection menus
    auto selectCB = [this](int surf, float startPos, float endPos) -> void
    {
        _spriteLayer->setFriction(MenuLayer::SURF_INFO[surf].coeff, MenuLayer::SURF_INFO[surf].sprite, startPos, endPos);
        _bgLayer->setSurface(MenuLayer::SURF_INFO[surf].sprite, startPos, endPos);
    };

    auto selectCB1 = std::bind(selectCB, std::placeholders::_1, visibleSize.width/2, visibleSize.width/2 + TARGET_LOC/2);
    auto selectCB2 = std::bind(selectCB, std::placeholders::_1, visibleSize.width/2 + TARGET_LOC/2, visibleSize.width/2 + TARGET_LOC);

    // using menu layer to show surface selectors and play button for challenge 4
    // surface selector section callfunc
    auto surfSelectionMenu1 = _menuLayer->selectSurfaceFriction("Surface 1 Friction :", 
            Vec2(origin.x + visibleSize.width/8 , origin.y + visibleSize.height/4),
            Vec2(origin.x + visibleSize.width/8 , origin.y + visibleSize.height/6), selectCB1);
    auto surfSelectionMenu2 = _menuLayer->selectSurfaceFriction("Surface 2 Friction :", 
            Vec2(origin.x + 6*visibleSize.width/8 - 5 , origin.y + visibleSize.height/4),
            Vec2(origin.x + 7*visibleSize.width/8 , origin.y + visibleSize.height/6), selectCB2);

    // play button
    auto play = MenuItemImage::create("play.png",
                                  "play.png", 
                                  [=] (Ref* sender) -> void
                                  { 
                                        surfSelectionMenu1->setVisible(false);
                                        surfSelectionMenu2->setVisible(false);
                                        schedule(schedule_selector(Challenge3::countDownTimer), 1.0);
                                        dynamic_cast<Node *>(sender)->setVisible(false);
                                        // add the force menu
                                        _menuLayer->addForceMenu(-SpriteLayer::MAX_FORCE, SpriteLayer::MAX_FORCE, 0, this, cccontrol_selector(Challenge3::forceValueChanged));

                                  });
    play->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2));

    auto playMenu = Menu::create(play , nullptr);
    playMenu->setPosition(Vec2::ZERO);
    this->addChild(playMenu , MN_ZINDEX);
    auto halfCrateWidth = _spriteLayer->getCrateSize().width/2;
    // setup challenge completion checks
    Vec2 originalPos = _bgLayer->getPosition();
    _spriteLayer->setPeriodicCB([=](float vel, float dt) -> bool
    {
        static bool sPastSafePoint = false;
        if(fabs(_bgLayer->getPosition().x - originalPos.x) > TARGET_LOC &&
            fabs(_bgLayer->getPosition().x - originalPos.x) < TARGET_LOC_END - halfCrateWidth)
        {
            // TODO: check why it's not working
            if(vel == 0.0)
            {
                done(true);
                return false;	// returning false to stop updating the background layer
            }
        }
        else if( fabs(_bgLayer->getPosition().x - originalPos.x) >= TARGET_LOC_END - halfCrateWidth)
        {
            if (!sPastSafePoint)
            {
                sPastSafePoint = true;

                auto boom = Sprite::create("boom.png");
                boom->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2));
                this->addChild(boom , MN_ZINDEX);

                // playing blast sound effect
                SimpleAudioEngine::getInstance()->playEffect("audio/explosion.mp3");
                NativeHelper::vibrate(500);

                // tint by white action
                auto zoomin  = ScaleBy::create(0.5 , 2);
                auto zoomout = ScaleTo::create(0.5, 1);
                auto seq = Sequence::create(zoomin , zoomout , nullptr);
                boom->runAction(RepeatForever::create(seq));

                auto delay = DelayTime::create(3);
                auto doneAction = CallFunc::create([=]() -> void { done(false); });
                auto seq1 = Sequence::create(delay, doneAction, nullptr);
                boom->runAction(seq1);

                // TODO: show done dialog after 3 seconds
                //				done(false);
                return false;	// returning false to stop updating the background layer
            }
            return false; 	// returning false to stop updating background layer for points > TARGET_LOC_END
        }
        else
            sPastSafePoint = false;

        return true;	// no condition met then return true to update the background layer
    });

    return true;	// return true of init()
}

std::string Challenge3::getTimeString() 
{
    std::stringstream sstr;
    sstr << (int)_timeLeft;
    return sstr.str();
}

void Challenge3::countDownTimer(float dt)
{
    _timeLeft -= dt;
    if(_timeLeft <= 11.0)
    {
    	_timeLabel->setColor(Color3B::RED);
    	_timeLabel->runAction(Blink::create(10 , 10));
    }

    _timeLabel->setString(getTimeString());
    if(floor(_timeLeft) <= 0)
    {
        unschedule(schedule_selector(Challenge3::countDownTimer));
        done(false);
    }
}

void Challenge3::showInfoPopup()
{
	addPopupMenu("INSTRUCTION", "Start by selecting a surface type for the two segments (Friction depends on surface type). "
			"Click on the play button to begin (Your time starts ticking!). "
			"Move the box from point A and stop it precisely at point B (do not overshoot!)", false);
}
