/********************************************************************
 Copyright(C), 2012-2013,
 ��  ��: FlyProps.h
 ��  ��: ���е��ߣ������ֹ���
*********************************************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#include "Hero/Hero.h"
#include "PublicTool/ToolFunc.h"
#include "Hero/Role/GongJian.h"

const float ArrowRate = 6.0f;		// �����ƶ�����

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