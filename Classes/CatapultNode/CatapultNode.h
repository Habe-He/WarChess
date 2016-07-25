#ifndef CatapultNode_hpp
#define CatapultNode_hpp

#include "cocos2d.h"
USING_NS_CC;

class CatapultNode : public Node            // 单利
{
public:
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    
    CREATE_FUNC(CatapultNode);
};
#endif /* CatapultNode_hpp */
