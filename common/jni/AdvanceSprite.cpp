//
//  AdvanceSprite.cpp
//
//  Created by varadharaj on 16/05/11.
//  
//  Copyright 2011 __Sourcebits Technologies Pvt. Ltd., Bangalore.__. All rights reserved.
//

#include "AdvanceSprite.h"

USING_NS_CC;

AdvanceSprite * AdvanceSprite::createWithFile( const std::string & fileName )
{
    auto pRet = new AdvanceSprite(); 
    if( pRet && pRet->init() )
    {
        pRet->addFrames(fileName);
        pRet->autorelease();
    }
    else
        CC_SAFE_DELETE(pRet);

    return pRet;
}

AdvanceSprite::AdvanceSprite()
:m_AnimationFrames()
,m_ElaspeTime(0.0)
,m_IncrementValue(0)
,m_isAnimationRunning(false)
,m_callback()
{
   
}

void AdvanceSprite::populateFramesFromPList(const std::string &pList)
{
    m_AnimationFrames.clear();
    auto pszPath = FileUtils::getInstance()->fullPathForFilename(pList);
	auto valueMap = FileUtils::getInstance()->getValueMapFromFile(pszPath);
    auto frames = valueMap["frames"].asValueMap();
    
    // MAJOR HACK ALERT: adding frames in the alphabetical order of names
    std::vector<std::string> keys; 
    for ( auto keyVal : frames )
        keys.push_back(keyVal.first);

    sort(keys.begin(), keys.end());

    for ( auto key : keys )
        m_AnimationFrames.pushBack(SpriteFrameCache::getInstance()->spriteFrameByName(key));


}

void AdvanceSprite::addFrames(const std::string &pList)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pList);
    populateFramesFromPList(pList);
    setDisplayFrame(0);
}

void AdvanceSprite::addFrames(const std::string &pList, const std::string &textureFileName)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pList, textureFileName);
    populateFramesFromPList(pList);
    setDisplayFrame(0);    
}

void AdvanceSprite::addFrames(const Vector<SpriteFrame *> &frames, int displayTextureIndex)
{
    m_AnimationFrames = frames;
    setDisplayFrame(displayTextureIndex);
}

void AdvanceSprite::addFrames(const Vector<SpriteFrame *> &frames)
{
    addFrames(frames, 0);
}

void AdvanceSprite::addFrames(Vector<SpriteFrame *> &&frames, int displayTextureIndex)
{
    m_AnimationFrames = std::move(frames);
    setDisplayFrame(displayTextureIndex);
}

void AdvanceSprite::addFrames(Vector<SpriteFrame *> &&frames)
{
    addFrames(std::move(frames), 0);
}

void AdvanceSprite::increaseCurrentIndex()
{
    if(m_AnimationMode && (m_EndingIndex - m_CurrentIndex) != 0)
        m_IncrementValue += (m_EndingIndex - m_CurrentIndex) / abs(m_EndingIndex - m_CurrentIndex);
    else if((m_StartingIndex - m_CurrentIndex) != 0)
        m_IncrementValue += (m_StartingIndex - m_CurrentIndex) / abs(m_StartingIndex - m_CurrentIndex);
}

void AdvanceSprite::update(float dt)
{
    m_ElaspeTime += dt;
    while (m_ElaspeTime >= m_FrameRate) 
    {
        m_ElaspeTime -= m_FrameRate;
        
        setDisplayFrame(m_CurrentIndex + m_IncrementValue);
        
         //Forward Animation....
        if (m_CurrentIndex == m_EndingIndex) 
        {
            if(m_NeedToRunReverseAnimation)
            {
                m_AnimationMode = false;
                increaseCurrentIndex();
                return;
            }
            
            m_RunningLoop++;
            if(m_RunningLoop == m_NumberOfLoop)
            {
                stopAnimaiton();
                //Raising Callback.
                if ( m_callback )
                    m_callback(this);

                return;
            }
            else
            {
                m_CurrentIndex = m_StartingIndex;
            }
        }
        
        //Reverse Animation...
        else if(m_CurrentIndex == m_StartingIndex)
        {
            m_RunningLoop++;
            if(m_RunningLoop == m_NumberOfLoop)
            {
                stopAnimaiton();
                //Raising Callback.
                if (m_callback)
                    m_callback(this);
                
                return;
            }
            else
            {
                m_AnimationMode = true;
                increaseCurrentIndex();
            }
        }
    }
}

void AdvanceSprite::startAnimation(int startInd, int endInd, int number_Loop, const std::function< void (Ref *)> &cb, int NumberOfFramesPerSecond, bool NeedToRunReverseAnimation)
{
    if(m_isAnimationRunning)
    {
        stopAnimaiton();
    }
    
    //Assign values.
    m_isAnimationRunning = true;
    
    m_StartingIndex = startInd - 1;
    m_EndingIndex = endInd - 1;
    
    m_NumberOfLoop = number_Loop;
    
    m_callback = cb;
    if(NumberOfFramesPerSecond != -1)
        m_FrameRate = calculateFrameRate(NumberOfFramesPerSecond);
    
    //Initilize values....
    m_NeedToRunReverseAnimation = NeedToRunReverseAnimation;
    m_AnimationMode = true;
    m_CurrentIndex = m_StartingIndex;
    m_RunningLoop = 0;
    m_IncrementValue = 0;
    increaseCurrentIndex();
    //scheduleUpdateWithPriority(-1);
    scheduleUpdate();
    resumeAnimation();
}

void AdvanceSprite::stopAnimaiton()
{
    m_isAnimationRunning = false;
    this->unscheduleUpdate();
}
