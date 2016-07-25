/****************************************
// File name: CWiFiRoom.h
// 日   期: 2016/01/05
// 作   者: ouyang
// 简   介: 创建WiFi联机对战房间
*****************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameManager/GameInfo.h"

using namespace cocos2d::ui;

class CWiFiRoom : public Layer
{
public:
	CWiFiRoom();
	~CWiFiRoom();

	static Scene* createScene();

	bool init();

	CREATE_FUNC(CWiFiRoom);

	void numClick(Ref* pSender, Widget::TouchEventType type);

	void modeClick(Ref* pSender, Widget::TouchEventType type);

	void viewClick(Ref* pSender, Widget::TouchEventType type);

	void getBackClick(Ref* pSender, Widget::TouchEventType type);

	void createRoom(Ref* pSender, Widget::TouchEventType type);

protected:
	Text* textMode0;					// 游戏模式
	Text* textMode1;
	Text* textView0;					// 观战
	Text* textView1;
	Text* textRoleCount;				// 战斗中人数限制
	TextField* roomName;				// 输入的房间名

	int m_mapID;						// 选择的地图
	int m_roomSize;						// 房间人数
	FightingMode mode;					// 战斗模式
	ViewGameState viewState;			// 观战模式
};