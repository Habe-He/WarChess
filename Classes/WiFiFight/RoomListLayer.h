/****************************************
// File name: RoomListLayer.h
// 日   期: 2015/12/25
// 作   者: ouyang
// 简   介: WiFi 对战选择房间
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

	void backClick(Ref *pSender, Widget::TouchEventType type);				// 返回按钮

	void createRoom(Ref* pSender, Widget::TouchEventType type);				// 创建房间按钮

	void JoinRoom(Ref* pSender, Widget::TouchEventType type);				// 加入房间按钮

	void JoinButtonEnable(bool isEnable);									// 未选择房间时，加入房间按钮不可用
	
	void onFindNewServer(std::map<std::string, ServerInfo*> data);
	virtual void onFindServer(const char* serverName, const char* ip, unsigned short port);
	virtual void onClientMessage(void* data);

	void createRoomList(std::map<std::string, ServerInfo*>& data);			// 创建房间列表

	void showRoomInfo(int _choose);											// 显示房间信息

	void refreshList();														// 房间列表自动刷新

	void removeServerList();												// 房间列表移除

	void roomItemCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	virtual void update(float delta);										// 实时更新房间列表信息
	virtual void active();
	virtual void deactive();
	virtual void b4remove();


private:
	bool need_sync;										// 是否解析房间列表
	bool _findRoom;										// 是否选择了房间
	int roomNumber;										// room count
	int chooseNumber;									// room list choose btn

	Text* m_rMapSize;									// 右方显示的地图大小
	Text* m_rRoomSize;									// team size
	Text* m_rRoomMode;									// game mode shijun
	Text* m_rRoomMode1;									// game mode quan jian

	std::map<std::string, ServerInfo* > m_serverList;
	ScrollView* listScrollView;
};