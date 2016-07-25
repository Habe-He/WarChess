/****************************************
// File name: CWiFiRoom.h
// ��   ��: 2016/01/05
// ��   ��: ouyang
// ��   ��: ����WiFi������ս����
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
	Text* textMode0;					// ��Ϸģʽ
	Text* textMode1;
	Text* textView0;					// ��ս
	Text* textView1;
	Text* textRoleCount;				// ս������������
	TextField* roomName;				// ����ķ�����

	int m_mapID;						// ѡ��ĵ�ͼ
	int m_roomSize;						// ��������
	FightingMode mode;					// ս��ģʽ
	ViewGameState viewState;			// ��սģʽ
};