//
//  AdvanceSprite.h
//
//  Created by varadharaj on 16/05/11.
//  
//  Copyright 2011 __Sourcebits Technologies Pvt. Ltd., Bangalore.__. All rights reserved.
//

#ifndef _AdvanceSprite_H
#define _AdvanceSprite_H

#include "cocos2d.h" 

class AdvanceSprite : public cocos2d::Sprite
{
    cocos2d::Vector<cocos2d::SpriteFrame *> m_AnimationFrames;
    //Indexing variable.
    int m_StartingIndex, m_EndingIndex, m_CurrentIndex, m_IncrementValue;
    
    //Loopoing variable.
    int m_RunningLoop, m_NumberOfLoop;
    
    //Frame Rate.
    float m_FrameRate;
    
    //Callback function pointer.
    std::function< void (cocos2d::Ref *) > m_callback;
    
    //Frame Rate Calculation variable.
    float m_ElaspeTime;
    
    //Animation mode variable.
    bool m_NeedToRunReverseAnimation, m_AnimationMode;
    bool m_isAnimationRunning;
    //Local funciton.
    float calculateFrameRate(int number_frames) { return 1.0f / number_frames; }
    void increaseCurrentIndex();
    
    void populateFramesFromPList(const std::string &pList);
    
    void setDisplayFrame(int i) { m_CurrentIndex = i; setSpriteFrame(m_AnimationFrames.at(i)); };
    virtual void update(float dt);
public:

    CREATE_FUNC(AdvanceSprite);
    static AdvanceSprite * createWithFile( const std::string & fileName );

    //
    AdvanceSprite();
    
    //Add Frames to run Animation.
    void addFrames(const std::string & pList); // Has Texture file name in pList.
    void addFrames(const std::string &pList, const std::string &textureFileName);
    void addFrames(const cocos2d::Vector<cocos2d::SpriteFrame*> &frames);
    void addFrames(cocos2d::Vector<cocos2d::SpriteFrame*> &&frames);
    void addFrames(const cocos2d::Vector<cocos2d::SpriteFrame*> &frames, int displayTextureIndex);
    void addFrames(cocos2d::Vector<cocos2d::SpriteFrame*> &&frames, int displayTextureIndex);
    
    //Sets Frame Rate for running Animation
    void setFrameRate(int NumberOfFramesPerSecond) { m_FrameRate = calculateFrameRate(NumberOfFramesPerSecond); }
    void setFrameRate(float frameRate) { m_FrameRate = frameRate; }
    
    /***************************  Animation Function *************************
    
     startInd - Starting index of loaded array for running Animation
     endInd - Ending index of loaded array for running Animation
     number_Loop - Number of Loop that Animaiton to run.
     cb - notification callback on completion
     NumberOfFramesPerSecond - Number of Frames to run per Second.
     NeedToRunReverseAnimation - Needs to run reverse Animation.
     NeedToDeleteItself - Needs the object to delete after finish the animation.
     
    ***************************************************************************/
    
    void startAnimation(int startInd, int endInd, int number_Loop, const std::function< void ( cocos2d::Ref * ) > &cb, int NumberOfFramesPerSecond, bool NeedToRunReverseAnimation);
    
    //Running Animation from Current index to Parameter Index.
    void moveTo(int index) { startAnimation(m_CurrentIndex + 1, index, 1, m_callback, -1, false); }
    void moveTo(int index, const std::function< void (Ref *) > &cb, int NumberOfFramesPerSecond ) { startAnimation(m_CurrentIndex + 1, index, 1, cb, NumberOfFramesPerSecond, false); }
    
    //Controller for Animation.
    void pauseAnimation() { pauseSchedulerAndActions(); }
    void resumeAnimation() { resumeSchedulerAndActions(); }
    void stopAnimaiton();

    //data about the animation
    int getNumFrames() { return m_AnimationFrames.size(); } 
    bool isAnimating() { return m_isAnimationRunning; } 
};

#endif
