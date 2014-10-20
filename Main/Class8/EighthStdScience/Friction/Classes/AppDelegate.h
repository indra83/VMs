#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "VmBase.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : public VmBase
{
public:
    AppDelegate();
    virtual ~AppDelegate();
};

#endif // _APP_DELEGATE_H_

