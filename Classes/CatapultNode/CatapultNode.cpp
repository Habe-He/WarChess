#include "CatapultNode.h"

bool CatapultNode::init()
{
    
    
    return true;
}

void CatapultNode::onEnter()
{
    Node::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CatapultNode::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CatapultNode::onExit()
{
    Node::onExit();
}

bool CatapultNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}
