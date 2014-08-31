#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"
void captureScreenAsSprite(std::function< void (cocos2d::Sprite *) > cb);

#endif // __UTIL_H__
