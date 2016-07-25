/****************************************
// File name: RoomState.h
// ��   ��: 2016/01/05
// ��   ��: ouyang
// ��   ��: WiFi������ҽ��뷿��׼��
*****************************************/
#pragma once
#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

#include "NetKit/NetKit.h"

const int StateNodeFlag = 10;
const int MyColorFlag = 11;

class RoomState : public Layer, public NetKitDelegate
{
public:
	RoomState()
	{
		m_ColorA = 3;
		m_ColorB = 0;
		m_c_prepare = false;
		m_s_prepare = false;
		m_setTimeNum = 0;
		m_connect = false;
		m_disconnect = false;
		m_connectFlag = false;
		m_heartAction = nullptr;
		TimeFlag = 5.0f;
	}

	~RoomState(){}

	static Scene* createScene();

	bool init();

	CREATE_FUNC(RoomState);

	void initTextContent();

	void getBackClick(Ref* pSender, Widget::TouchEventType type);

	void myTeamClick(Ref* pSender, Widget::TouchEventType type);

	void startGameClick(Ref* pSender, Widget::TouchEventType type);

	void myColorClick(Ref* pSender, Widget::TouchEventType type);		// 

	void remoteColorClick(Ref* pSender, Widget::TouchEventType type);

	void prepareClick(Ref* pSender, Widget::TouchEventType type);

	void spaceGame(float dt);											// ���뵹��ʱ������Ϸ

protected: // two mode connect
	virtual void onServerMessage(void* data);
	virtual void onClientMessage(void* data);
	virtual void onServerDisconnect();
	virtual void onClientDisconnect();
	void startHeartBeat();
	void stopHeartBeat();
	void heartBeatCall();
	void sendSetTime();
	void afterDisconnect();
	void update(float dt);
	void activation();
	virtual void active();
	virtual void deactive();
	virtual void b4remove();

private:
	bool m_connectFlag;
	bool m_connect;	bool m_disconnect;
	bool m_c_prepare;				// client prepare
	bool m_s_prepare;				// servers prepare
	int m_setTimeNum;
	std::string m_broadcastStr;
	std::vector<int> m_setTimeValues;
	std::vector<int> m_pingValues;
	Action* m_heartAction;

private:
	int m_ColorA;					// Ĭ���Ϸ���ť��ɫ
	int m_ColorB;					// Ĭ�ϵڶ�����ť��ɫ
	Button* prepareBtn;
	Button* startGameBtn;
	Button* m_BtnColor_Up;			// �Ϸ�����ɫ��ť
	Button* m_remoteTeamColor;		// remote team color
	Text* m_sPreText0;				// servers prepareing
	Text* m_sPreText1;				// servers prepared
	Text* sersName;					// servers name
	Text* m_cPreText0;				// client prepareing
	Text* m_cPreText1;				// client prepared
	Text* m_remoteName;				// client player name
	Text* m_mapSize;				// map size
	Text* m_teamSize;				// 
	Text* m_roomMode0;				// wifi fighting game mode shijun
	Text* m_roomMode1;				//                         quan jian
	Text* spaceLabel;				// ����ʱ�����ı�
	ImageView* spaceSprite;			// ����ʱ����ͼƬ
	float TimeFlag;					// ����ʱ ʱ��
};