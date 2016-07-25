/****************************************
// File name: RoomListLayer.h
// ��   ��: 2015/12/25
// ��   ��: ouyang
// ��   ��: WiFi ��սѡ�񷿼�
*****************************************/

#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

#include "NetKit/NetKit.h"

const int RoomNodeFlag		= 1000;		// roomnode original node
const int JoinButtonFlag	= 1000;		// join room button

struct ServerInfo
{
	std::string name;
	std::string ip;
	std::string mapId;
	int port;
	std::string roomSize;
	std::string roomMode;

	ServerInfo() :
		name(""),
		ip(""),
		mapId(""),
		port(0),
		roomSize(""),
		roomMode("")
	{
	}
};

class RoomListLayer : public Layer, public NetKitDelegate
{
public:
	RoomListLayer();
	~RoomListLayer();

	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(RoomListLayer);

	void backClick(Ref *pSender, Widget::TouchEventType type);				// ���ذ�ť

	void createRoom(Ref* pSender, Widget::TouchEventType type);				// �������䰴ť

	void JoinRoom(Ref* pSender, Widget::TouchEventType type);				// ���뷿�䰴ť

	void JoinButtonEnable(bool isEnable);									// δѡ�񷿼�ʱ�����뷿�䰴ť������
	
	void onFindNewServer(std::map<std::string, ServerInfo*> data);
	virtual void onFindServer(const char* serverName, const char* ip, unsigned short port);
	virtual void onClientMessage(void* data);

	void createRoomList(std::map<std::string, ServerInfo*>& data);			// ���������б�

	void showRoomInfo(int _choose);											// ��ʾ������Ϣ

	void refreshList();														// �����б��Զ�ˢ��

	void removeServerList();												// �����б��Ƴ�

	void roomItemCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	virtual void update(float delta);										// ʵʱ���·����б���Ϣ
	virtual void active();
	virtual void deactive();
	virtual void b4remove();


private:
	bool need_sync;										// �Ƿ���������б�
	bool _findRoom;										// �Ƿ�ѡ���˷���
	int roomNumber;										// room count
	int chooseNumber;									// room list choose btn

	Text* m_rMapSize;									// �ҷ���ʾ�ĵ�ͼ��С
	Text* m_rRoomSize;									// team size
	Text* m_rRoomMode;									// game mode shijun
	Text* m_rRoomMode1;									// game mode quan jian

	std::map<std::string, ServerInfo* > m_serverList;
	ScrollView* listScrollView;
};