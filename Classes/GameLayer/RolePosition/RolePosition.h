/********************************************************************
 Copyright(C), 2012-2013,
 文  件: RolePosition.h
 描  述: 战斗界面角色头像
*********************************************************************/
#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#include "Hero/RoleDef.h"
#include "Hero/Hero.h"
#include "GameLayer/GameDef.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

const int FrameFlag			= 10;				// touxiang
const int FrameLayerFlag	= 11;				// 头像背景层
const int DotSpriteFlag		= 1000;				// 分割图片
const int RoleFrameZ		= 100;				// 头像层级

const Size RoleFrameSize = Size(75, 75);		// 头像大小
const Size DotSize		 = Size(20, 75);		// 分割图片大小

class RolePosition : public Node
{
public:
	RolePosition();
	~RolePosition();

	static RolePosition* creatInstance(RoleType roleType, int agality, Color3B color);

	bool createRoleFrame(RoleType roleType, int agality, Color3B color);

	void setFrameColor(Color3B color);

	void setFrameTag(int tag);

	void setFrameVisable(bool isVisable);

	void setFrameName(string name);
};

class RolePosManager : public Layer
{
public:
	RolePosManager();
	virtual ~RolePosManager();

	static Scene* scene();

	bool init();

	void showRoleOrder(vector<Hero* >& vCurRole, vector<Hero* >& vPreRole, OrderRole orderRole);

	bool deathRemove(vector<Hero* >& vCurRole, vector<Hero* >& vPreRole, Hero* deathRole);

	void sortRoleOrder(vector<Hero* > vCurRole, OrderRole roleOrder);

	CREATE_FUNC(RolePosManager);

private:
	vector<RolePosition* > vRolePhoto;
	ui::ScrollView* frameLayer;

	bool m_isFirstPhoto;			// 大头像角色死亡
	bool m_isCurVectorPhoto ;		// 在未移动角色头像 非 大头像
	bool m_isPreVectorPhoto ;		// 移动过的角色头像
};

