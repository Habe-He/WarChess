#include "RoomListLayer.h"


#include "NetKit/NetKit.h"
#include "NetKit/NetMessage.h"
#include "Player/PlayerManager.h"
#include "GameManager/GameManager.h"
#include "PrepareLayer.h"
#include "StarLayer/StarLayer.h"
#include "CWiFiRoom.h"
#include "RoomState.h"
#include "RoomInfo.h"

static float addTime = 0.0f;
static float cellScale = 1.0f;			// 滚动列表 缩放

RoomListLayer::RoomListLayer()
{
	chooseNumber = -1;
}

RoomListLayer::~RoomListLayer()
{
	
}

Scene* RoomListLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(RoomListLayer::create());

	return scene;
}

bool RoomListLayer::init()
{
	auto roomNode = CSLoader::createNode("RoomListLayer.csb");
	this->addChild(roomNode, 10, RoomNodeFlag);

	auto btnBack = static_cast<Button*>(roomNode->getChildByName("Btn_Backup"));
	//btnBack->addTouchEventListener(this, toucheventselector(RoomListLayer::backClick));
	btnBack->addTouchEventListener(Widget::ccWidgetTouchCallback(CC_CALLBACK_2(RoomListLayer::backClick, this)));	auto btnJoin = static_cast<Button*>(roomNode->getChildByName("Btn_Join"));
	btnJoin->addTouchEventListener(CC_CALLBACK_2(RoomListLayer::JoinRoom, this));	auto btnCreate = static_cast<Button*>(roomNode->getChildByName("Btn_Create"));
	btnCreate->setTag(JoinButtonFlag);
	btnCreate->addTouchEventListener(CC_CALLBACK_2(RoomListLayer::createRoom, this));	listScrollView = static_cast<ScrollView*>(roomNode->getChildByName("ScrollView_Item"));	m_rMapSize = (Text*)roomNode->getChildByName("Text_MapSize");	m_rRoomSize = (Text*)roomNode->getChildByName("Text_Hero_Count");	m_rRoomMode = (Text*)roomNode->getChildByName("Text_Mode");	m_rRoomMode1 = (Text*)roomNode->getChildByName("Text_Mode_0");	
	JoinButtonEnable(false);

	NK()->setDelegate(this);
	NK()->stopBroadcast();
	NK()->startFindServer(BROADCAST_PORT);

 	this->scheduleUpdate();

	return true;
}

void RoomListLayer::backClick(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(StarLayer::scene());
	}
}

void RoomListLayer::createRoom(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		GameMgr()->setIsServer(true);
		Director::getInstance()->replaceScene(CWiFiRoom::createScene());
	}	
}

void RoomListLayer::JoinRoom(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		auto btn = (MenuItemLabel*)pSender;
		// TODO 此时先把 选择房间判断去掉
		if (chooseNumber == -1 || m_serverList.size() == 0)
		{
			return;
		}
		std::map<std::string, ServerInfo*>::iterator itr = m_serverList.begin();
		std::advance(itr, chooseNumber);
		ServerInfo* svrinfo = itr->second;
		bool succeed = NK()->connectServer(svrinfo->ip.c_str(), SERVER_PORT);
		if (!succeed)
		{
			NK()->stop();
			NK()->setDelegate(this);
			refreshList();
			//addChild(ToolTipLayer::createScene("连接失败,请重新加入", btn));
			return;
		}
		GameMgr()->setOppoName(svrinfo->name);

		ProtLogin prot = { 0 };
		prot.head.protoId = PROTO_LOGIN;
		std::string clientName = PLAYER_MGR()->localPlayer()->getName();
		strcpy(prot.name, clientName.c_str());
		prot.cReqTime = (int)GameMgr()->getCurrentTime();
		prot.sRspTime = 0;

		prot.headId = GameMgr()->getNameBoard(kHead);
		prot.titleId = GameMgr()->getNameBoard(kTitle);
		prot.shadeId = GameMgr()->getNameBoard(kShade);
		NK()->send(&prot, sizeof(prot));

		GameMgr()->setMapId(atoi(svrinfo->mapId.c_str()));
	}	
}

void RoomListLayer::JoinButtonEnable(bool isEnable)
{
	auto pNode = this->getChildByTag(RoomNodeFlag);
	auto joinButton = (Button*)pNode->getChildByName("Btn_Join");
	if ( isEnable )
	{
		joinButton->setVisible(true);
		joinButton->setEnabled(true);
		joinButton->setTouchEnabled(true);
	}
	else
	{
		joinButton->setVisible(false);
		joinButton->setEnabled(false);
		joinButton->setTouchEnabled(false);
	}
}

void RoomListLayer::onFindServer(const char* serverName, const char* ip, unsigned short port)
{
	std::vector<std::string> roomData = Utils::split_string(serverName, "#");
	std::string name = roomData[0];
	if (m_serverList.count(name) == 0)
	{
		ServerInfo* svrinfo = new ServerInfo();
		svrinfo->name = name;
		svrinfo->ip = ip;
		svrinfo->port = port;
		svrinfo->mapId = roomData[1];
		svrinfo->roomSize = roomData[2];
		svrinfo->roomMode = roomData[3];
		m_serverList.insert(std::make_pair(svrinfo->name, svrinfo));
		onFindNewServer(m_serverList);
	}
	else if (m_serverList.count(name) >= 0)
	{
		ServerInfo* srvInfo = m_serverList[name];
		if (srvInfo != nullptr /*&& (srvInfo->mapId != roomData[1])*/ )
		{
			srvInfo->mapId = roomData[1];

			need_sync = true;
		}
	}
}

void RoomListLayer::onClientMessage(void* data)
{
	ProtHead* head = (ProtHead*)data;
	switch (head->protoId)
	{
		case PROTO_LOGIN:
		{
			NK()->stopBroadcast();
			ProtLogin* prot = (ProtLogin*)data;
			PLAYER_MGR()->createPlayer(prot->name, false);
			long cTime = GameMgr()->getCurrentTime();
			long ping = (cTime - prot->cReqTime) / 2;
			long dt = prot->sRspTime + ping - cTime;
			GameMgr()->setClientServerDeltaTime(dt);

			GameMgr()->setOppoNameBoard(kHead, prot->headId);
			GameMgr()->setOppoNameBoard(kTitle, prot->titleId);
			GameMgr()->setOppoNameBoard(kShade, prot->shadeId);
			GameMgr()->setMapId(prot->mapId);
			removeServerList();
			refreshList();

			Director::getInstance()->replaceScene(RoomState::createScene());
		}
		break;
	}
}

void RoomListLayer::onFindNewServer(std::map<std::string, ServerInfo*> data)
{
	if (data.size() == roomNumber)
	{
		return;
	}
	roomNumber = (int)data.size();
	_findRoom = true;
}

void RoomListLayer::createRoomList(std::map<std::string, ServerInfo*>& data)
{
	if (data.size() == 0)
		return;

	// init RoomListItem
	listScrollView->removeAllChildrenWithCleanup(true);
	auto itemCount = data.size();
	Size scrollSize = listScrollView->getInnerContainerSize();
	int cellWidth = 0;											// 滚动列表 宽间隔
	int cellHeight = 10;										// 滚动列表 高间隔
	auto height = (scrollSize.height * cellScale + cellHeight) * itemCount;
	height = (height > scrollSize.height) ? height : scrollSize.height;
	listScrollView->setInnerContainerSize(Size(scrollSize.width, height));

	int number = 0;
	for (std::map<std::string, ServerInfo*>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		std::string name = iter->first;
		auto btn_room = Button::create("XUANZHEFANGJIAN/Room.png", "XUANZHEFANGJIAN/Room.png");
		btn_room->setTag(number);
		btn_room->setAnchorPoint(Point(0.5, 0.5));
		btn_room->setPosition(Vec2(number * (scrollSize.width * cellScale + cellWidth) + 220,
			height - 40 - number * (scrollSize.height * cellScale + cellHeight)));
		listScrollView->addChild(btn_room);
		btn_room->addTouchEventListener(CC_CALLBACK_2(RoomListLayer::roomItemCallBack, this));

		auto nameLabel = Label::createWithSystemFont(name.c_str(), "", 20);
		nameLabel->setColor(Color3B::BLACK);
		nameLabel->setPosition(Vec2(btn_room->getContentSize().width / 2, btn_room->getContentSize().height / 2));
		btn_room->addChild(nameLabel, 10);
		number ++;
	}

	_findRoom = false;
}

void RoomListLayer::showRoomInfo(int _choose)
{
	int number = 0;

	for (std::map<std::string, ServerInfo*>::iterator iter = m_serverList.begin(); iter != m_serverList.end(); iter++)
	{
		ServerInfo* info = iter->second;
		if (_choose == number)
		{
			if (atoi(info->roomMode.c_str()) == Mode_Kill_All)
			{
				m_rRoomMode->setVisible(false);
				m_rRoomMode1->setVisible(true);
			}
			else
			{
				m_rRoomMode->setVisible(true);
				m_rRoomMode1->setVisible(false);
			}
			m_rMapSize->setString(__String::createWithFormat("%s", info->mapId.c_str())->getCString());
			m_rRoomSize->setString(__String::createWithFormat("%s", info->roomSize.c_str())->getCString());
		}
		number++;
	}	
}

void RoomListLayer::refreshList()
{
	
}

void RoomListLayer::removeServerList()
{
	
}

void RoomListLayer::roomItemCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		auto itemNode = (Node*)pSender;
		chooseNumber = itemNode->getTag();
		showRoomInfo(chooseNumber);
	}
}

void RoomListLayer::update(float delta)
{
	addTime += delta;
	if ( addTime >= 300 )
	{
		// 刷新列表
		addTime -= 300;
	}
	if ( need_sync )
	{
		for ( auto itor : m_serverList )
		{

		}
		need_sync = false;
	}

	if ( m_serverList.size() > 0 )
	{
		JoinButtonEnable(true);
	}

	if (!_findRoom)
		return;

	createRoomList(m_serverList);
}

void RoomListLayer::active()
{
	NK()->setDelegate(this);
	NK()->stopBroadcast();
	NK()->startFindServer(BROADCAST_PORT);
}

void RoomListLayer::deactive()
{

}

void RoomListLayer::b4remove()
{
	NK()->stopBroadcast();
	NK()->stop();
}
