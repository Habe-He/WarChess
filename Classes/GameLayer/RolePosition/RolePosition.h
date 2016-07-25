/********************************************************************
 Copyright(C), 2012-2013,
 ��  ��: RolePosition.h
 ��  ��: ս�������ɫͷ��
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
const int FrameLayerFlag	= 11;				// ͷ�񱳾���
const int DotSpriteFlag		= 1000;				// �ָ�ͼƬ
const int RoleFrameZ		= 100;				// ͷ��㼶

const Size RoleFrameSize = Size(75, 75);		// ͷ���С
const Size DotSize		 = Size(20, 75);		// �ָ�ͼƬ��С

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

	bool m_isFirstPhoto;			// ��ͷ���ɫ����
	bool m_isCurVectorPhoto ;		// ��δ�ƶ���ɫͷ�� �� ��ͷ��
	bool m_isPreVectorPhoto ;		// �ƶ����Ľ�ɫͷ��
};

