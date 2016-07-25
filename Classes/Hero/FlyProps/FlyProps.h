/********************************************************************
 Copyright(C), 2012-2013,
 文  件: FlyProps.h
 描  述: 飞行道具：弓箭手弓箭
*********************************************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#include "Hero/Hero.h"
#include "PublicTool/ToolFunc.h"
#include "Hero/Role/GongJian.h"

const float ArrowRate = 6.0f;		// 弓箭移动速率

class FlyProps : public Node
{
public:
	FlyProps(void);
	virtual ~FlyProps(void);

	static FlyProps* createProps(Hero* _hero, Hero* _targetRole, AttackOrCounter _type);

	bool create(Hero* _hero, Hero* _targetRole, AttackOrCounter _type);

protected:
	Sprite* flySprite;
};