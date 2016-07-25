#include "RoomState.h"
#include "GameManager/GameManager.h"
#include "const.h"
#include "NetKit/NetMessage.h"
#include "Player/PlayerManager.h"
#include "PrepareLayer.h"
#include "CWiFiRoom.h"
#include "RoomListLayer.h"

Scene* RoomState::createScene()
{
	auto scene = Scene::create();
	scene->addChild(RoomState::create());

	return scene;
}

bool RoomState::init()
{
	auto roomStateNode = CSLoader::createNode("RoomPrepare.csb");
	this->addChild(roomStateNode, -10, StateNodeFlag);

	auto getBackBtn = (Button*)roomStateNode->getChildByName("Btn_Backup");
	getBackBtn->addTouchEventListener(CC_CALLBACK_2(RoomState::getBackClick, this));

	auto myTeamBtn = (Button*)roomStateNode->getChildByName("Button_2");
	myTeamBtn->addTouchEventListener(CC_CALLBACK_2(RoomState::myTeamClick, this));

	startGameBtn = (Button*)roomStateNode->getChildByName("Button_2_0");						// start game
	startGameBtn->addTouchEventListener(CC_CALLBACK_2(RoomState::startGameClick, this));
	startGameBtn->setVisible(false);
	startGameBtn->setTouchEnabled(false);

	prepareBtn = (Button*)roomStateNode->getChildByName("Button_2_0_0");						// prepare game
	prepareBtn->addTouchEventListener(CC_CALLBACK_2(RoomState::prepareClick, this));

	m_BtnColor_Up = (Button*)roomStateNode->getChildByName("Button_5");							// button color up
	m_BtnColor_Up->setTag(MyColorFlag);
	m_BtnColor_Up->setColor(Color3B(GameMgr()->getTeamColor(m_ColorA)));
	m_BtnColor_Up->addTouchEventListener(CC_CALLBACK_2(RoomState::myColorClick, this));
	m_BtnColor_Up->setTouchEnabled(false);

	m_remoteTeamColor = (Button*)roomStateNode->getChildByName("Button_5_0");					// button color down
	m_remoteTeamColor->addTouchEventListener(CC_CALLBACK_2(RoomState::remoteColorClick, this));
	m_remoteTeamColor->setColor(Color3B(GameMgr()->getTeamColor(m_ColorB)));
	m_remoteTeamColor->setTouchEnabled(false);

	spaceSprite = (ImageView*)roomStateNode->getChildByName("Image_6");
	spaceSprite->setVisible(false);
	spaceLabel = (Text*)roomStateNode->getChildByName("Text_13");
	spaceLabel->setVisible(false);

	// save two team color
	GameMgr()->setLocalColor(m_ColorA);
	GameMgr()->setRemoteColor(m_ColorB);

	m_sPreText0 = (Text*)roomStateNode->getChildByName("Text_14_0");
	m_sPreText1 = (Text*)roomStateNode->getChildByName("Text_14_0_0");
	m_sPreText0->setVisible(true);
	m_sPreText1->setVisible(false);

	m_cPreText0 = (Text*)roomStateNode->getChildByName("Text_14");
	m_cPreText1 = (Text*)roomStateNode->getChildByName("Text_14_1");
	m_cPreText0->setVisible(true);
	m_cPreText1->setVisible(false);

	sersName = (Text*)roomStateNode->getChildByName("Text_1");
	m_remoteName = (Text*)roomStateNode->getChildByName("Text_1_0_0");

	m_mapSize = (Text*)roomStateNode->getChildByName("Text_5_1");
	m_teamSize = (Text*)roomStateNode->getChildByName("Text_5_0_1");
	m_roomMode0 = (Text*)roomStateNode->getChildByName("Text_5_0_0_1");
	m_roomMode1 = (Text*)roomStateNode->getChildByName("Text_5_0_0_1_0");
	m_roomMode0->setVisible(false);
	m_roomMode1->setVisible(false);

	if (GameMgr()->getRoomMode() == Mode_Kill_King)
	{
		m_roomMode1->setVisible(true);
	}
	else
	{
		m_roomMode0->setVisible(true);
	}

	initTextContent();

	NK()->setDelegate(this);

	if (GameMgr()->getIsServer())
	{
		m_remoteTeamColor->setTouchEnabled(false);
		m_mapSize->setString(__String::createWithFormat("%d", GameMgr()->getMapId())->getCString());
		m_teamSize->setString(__String::createWithFormat("%d", GameMgr()->getRoomSize())->getCString());
		sersName->setString(PLAYER_MGR()->localPlayer()->getName());
		
		char buffer[256];
		sprintf(buffer, "%s#%d#%d#%d", GameMgr()->getRoomName().c_str(), GameMgr()->getRoomMap(), 
			GameMgr()->getRoomSize(), (int)GameMgr()->getRoomMode());
		m_broadcastStr = buffer;
		NK()->stopBroadcast();
		NK()->startServer(SERVER_PORT);
		NK()->startBroadServer(BROADCAST_PORT, m_broadcastStr.c_str());		
	}
	else
	{
		m_BtnColor_Up->setTouchEnabled(false);
		startGameBtn->setVisible(false);
		startGameBtn->setTouchEnabled(false);

		ProtClientOk prot = { 0 };
		prot.head.protoId = PROTO_CLIENT_OK;
		prot.prepareOK = false;
		NK()->send(&prot, sizeof(prot));
		startHeartBeat();

		// send servers team color
		RemoteColor protColor = { 0 };
		protColor.head.protoId = PROT_TEAM_COLOR;
		protColor.color = m_ColorB;
		std::string clientName = PLAYER_MGR()->localPlayer()->getName();
		strcpy(protColor.name, clientName.c_str());
		NK()->send(&protColor, sizeof(protColor));
	}
	
	scheduleUpdate();
	activation();

	return true;
}

void RoomState::initTextContent()
{
 	auto roomStateNode = (Node*)this->getChildByTag(StateNodeFlag);
	auto mapSizeText = (Text*)roomStateNode->getChildByName("Text_5_1");			// 地图大小
	mapSizeText->setString(__String::createWithFormat("%d", GameMgr()->getRoomMap())->getCString());
	auto teamSizeText = (Text*)roomStateNode->getChildByName("Text_5_0_1");			// 队伍人数
	teamSizeText->setString(__String::createWithFormat("%d", GameMgr()->getRoomSize())->getCString());

	auto gameModeText0 = (Text*)roomStateNode->getChildByName("Text_5_0_0_1");		// 弑君
	gameModeText0->setVisible(false);
	auto gameModeText1 = (Text*)roomStateNode->getChildByName("Text_5_0_0_1_0");	// 全歼
	gameModeText1->setVisible(false);
	if ( GameMgr()->getRoomMode() == Mode_Kill_King )
	{
		gameModeText0->setVisible(true);
	}
	else
	{
		gameModeText1->setVisible(true);
	}

	auto myNameText = (Text*)roomStateNode->getChildByName("Text_1");				// 我的游戏名称
	auto myPossessText = (Text*)roomStateNode->getChildByName("Text_1_0");			// 阵营

	auto remoteNameText = (Text*)roomStateNode->getChildByName("Text_1_0_0");		// 对方游戏名称
	auto remotePossessText = (Text*)roomStateNode->getChildByName("Text_1_0_0_0");	// 对方阵营

	auto remoteColorBtn = (Button*)roomStateNode->getChildByName("Button_5_0");		// 对方阵营的颜色

	auto myStateText = (Text*)roomStateNode->getChildByName("Text_14_0");			// 我的状态
	auto remoteStateText = (Text*)roomStateNode->getChildByName("Text_14");			// 对方状态
}

void RoomState::getBackClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		if (GameMgr()->getIsServer())
		{
			Director::getInstance()->replaceScene(CWiFiRoom::createScene());
		}
		else
		{
			Director::getInstance()->replaceScene(RoomListLayer::createScene());
		}
		
	}
}

void RoomState::myTeamClick(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{

	}
}

void RoomState::startGameClick(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if ( GameMgr()->getLocalColor() != GameMgr()->getRemoteColor() )
		{
			startGameBtn->setVisible(false);
			startGameBtn->setTouchEnabled(false);

			spaceSprite->setVisible(true);
			spaceLabel->setVisible(true);

			this->schedule(schedule_selector(RoomState::spaceGame), 1.0f);
		}
		else
		{
			log("Two Team Select Same Color");
		}
	}
}

void RoomState::myColorClick(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if ( !GameMgr()->getIsServer())
		{
			return;
		}
		m_ColorA ++;
		if (m_ColorA >= 8)
			m_ColorA = 0;
		auto stateNode = (Node*)this->getChildByTag(StateNodeFlag);
		auto colorBtn = (Button*)stateNode->getChildByTag(MyColorFlag);
		colorBtn->setColor(GameMgr()->getTeamColor(m_ColorA));
		GameMgr()->setLocalColor(m_ColorA);

		RemoteColor prot = { 0 };
		prot.head.protoId = PROT_TEAM_COLOR;
		prot.color = m_ColorA;
		std::string clientName = PLAYER_MGR()->localPlayer()->getName();
		strcpy(prot.name, clientName.c_str());
		prot.mapID = GameMgr()->getMapId();
		prot.roomSize = GameMgr()->getRoomSize();
		prot.roomMode = GameMgr()->getRoomMode();
		NK()->send(&prot, sizeof(prot));
	}
}

void RoomState::remoteColorClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		if (GameMgr()->getIsServer())
		{
			return;
		}

		m_ColorB ++;
		if (m_ColorB >= 8)
			m_ColorB = 0;

		m_remoteTeamColor->setColor(GameMgr()->getTeamColor(m_ColorB));
		GameMgr()->setRemoteColor(m_ColorB);

		RemoteColor prot = { 0 };
		prot.head.protoId = PROT_TEAM_COLOR;
		prot.color = m_ColorB;
		std::string clientName = PLAYER_MGR()->localPlayer()->getName();
		strcpy(prot.name, clientName.c_str());
		prot.mapID = GameMgr()->getMapId();
		prot.roomSize = GameMgr()->getRoomSize();
		prot.roomMode = GameMgr()->getRoomMode();
		NK()->send(&prot, sizeof(prot));
	}
}

void RoomState::prepareClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		prepareBtn->setVisible(false);
		prepareBtn->setTouchEnabled(false);
		if ( GameMgr()->getIsServer() )
		{
			startGameBtn->setVisible(true);
			startGameBtn->setTouchEnabled(true);
			m_s_prepare = !m_s_prepare;

			if (m_s_prepare)
			{
				m_sPreText0->setVisible(false);
				m_sPreText1->setVisible(true);
			}
			else
			{
				m_sPreText1->setVisible(false);
				m_sPreText0->setVisible(true);
			}
			ProtPrepare prot = { 0 };
			prot.head.protoId = PROTO_PREPARE;
			prot.prepare = m_s_prepare;
			NK()->send(&prot, sizeof(prot));
		}
		else
		{
			m_c_prepare = !m_c_prepare;
			ProtPrepare prot = { 0 };
			prot.head.protoId = PROTO_PREPARE;
			prot.prepare = m_c_prepare;
			NK()->send(&prot, sizeof(prot));

			if (m_c_prepare)
			{
				m_sPreText1->setVisible(true);
				m_sPreText0->setVisible(false);
			}
			else
			{
				m_sPreText1->setVisible(false);
				m_sPreText0->setVisible(true);
			}
		}		
	}
}

void RoomState::spaceGame(float dt)
{
	TimeFlag -= 1.0f;
	auto timeStr = __String::createWithFormat("%1.0f", TimeFlag);
	spaceLabel->setString(timeStr->getCString());
	if ( TimeFlag == 0.0f )
	{		
		ProtWiFiOK prot = { 0 };
		prot.head.protoId = PROT_WIFI_OK;
		prot.isOK = true;
		NK()->send(&prot, sizeof(prot));
		this->unschedule(schedule_selector(RoomState::spaceGame));
		Director::getInstance()->replaceScene(PrepareLayer::createScene());
	}
}

void RoomState::onServerMessage(void* data)
{
	ProtHead* head = (ProtHead*)data;
	switch (head->protoId)
	{
		case PROTO_SET_TIME:
		{
			ProtSetTime* prot = (ProtSetTime*)data;

			ProtSetTime protb = { 0 };
			protb.head.protoId = PROTO_SET_TIME;
			protb.ctime = prot->ctime;
			protb.stime = GameMgr()->getCurrentTime();
			NK()->send(&protb, sizeof(protb));
		}
		break;
		case PROTO_EXIT_ROOM:
		{
			onServerDisconnect();
		}
		break;
		case PROTO_LOGIN:
		{
			NK()->stopBroadcast();
			NK()->stopListen();
			ProtLogin* prot = (ProtLogin*)data;
			GameMgr()->setOppoNameBoard(kHead, prot->headId);
			GameMgr()->setOppoNameBoard(kTitle, prot->titleId);
			PLAYER_MGR()->createPlayer(prot->name, false);
			GameMgr()->setOppoName(prot->name);
			m_connect = true;

			ProtLogin prot1 = { 0 };
			prot1.head.protoId = PROTO_LOGIN;
			prot1.cReqTime = prot->cReqTime;
			prot1.sRspTime = (int)GameMgr()->getCurrentTime();
			prot1.shadeId = GameMgr()->getNameBoard(kShade);
			prot1.headId = GameMgr()->getNameBoard(kHead);
			prot1.titleId = GameMgr()->getNameBoard(kTitle);

			prot1.mapId = 111;//GameMgr()->getMapId();
			std::string serverName = GameMgr()->getRoomName().c_str();
			strncpy(prot1.name, serverName.c_str(), sizeof(prot1.name) - 1);
			NK()->send(&prot1, sizeof(prot1));

			m_disconnect = false;
			startHeartBeat();
			activation();
			break;
		}
		case PROTO_PING_REQ:
		{
			ProtPing prot = { 0 };
			prot.head.protoId = PROTO_PING_REQ;
			prot.time = ((ProtPing*)data)->time;
			NK()->send(&prot, sizeof(prot));
			break;
		}
		case PROTO_PREPARE:
		{
			ProtPrepare* prot = (ProtPrepare*)data;
			m_c_prepare = prot->prepare;

			if (m_c_prepare)
			{
				m_cPreText0->setVisible(false);
				m_cPreText1->setVisible(true);
			}
			else
			{
				m_cPreText1->setVisible(false);
				m_cPreText0->setVisible(true);
			}
			break;
		}
		case PROTO_ACTOR:
		{
			ProtChangeData* prot = (ProtChangeData*)data;
			GameMgr()->setOppoNameBoard(kHead, prot->headId);
			GameMgr()->setOppoNameBoard(kTitle, prot->titleId);
			GameMgr()->setOppoNameBoard(kShade, prot->shadeId);
			break;
		}
		case PROTO_CLIENT_OK:
		{
			ProtChangeData prot = { 0 };
			prot.head.protoId = PROTO_ACTOR;
			prot.mapId = GameMgr()->getMapId();
			NK()->send(&prot, sizeof(prot));
			break;
		}
		case PROT_TEAM_COLOR:
		{
			RemoteColor* prot = (RemoteColor*)data;
			
			m_BtnColor_Up->setColor(GameMgr()->getTeamColor(m_ColorA));
			m_remoteTeamColor->setColor(GameMgr()->getTeamColor(prot->color));
			m_remoteName->setString(prot->name);
			sersName->setString(PLAYER_MGR()->localPlayer()->getName());
			break;
		}		
	}
}

void RoomState::onClientMessage(void* data)
{
	ProtHead* head = (ProtHead*)data;
	switch (head->protoId)
	{
		case PROTO_SET_TIME:
		{
			m_setTimeNum++;
			ProtSetTime* prot = (ProtSetTime*)data;
			int curCTime = GameMgr()->getCurrentTime();
			int ping = (curCTime - prot->ctime) / 2;
			int curSTime = prot->stime + ping;
			int csdt = curSTime - curCTime;
			m_setTimeValues.push_back(csdt);
			m_pingValues.push_back(ping);

			int sum = 0;
			int psum = 0;
			int size = m_setTimeValues.size();
			for (int i = 0; i < size; i++)
			{
			   sum += m_setTimeValues[i];
			   psum += m_pingValues[i];
			}
			int pjdt = sum / size;
			GameMgr()->setClientServerDeltaTime(pjdt);
			int pjp = psum / size;
			GameMgr()->setPjPing(pjp);
		}
		break;
		case PROTO_EXIT_ROOM:
		{
			onClientDisconnect();
		}
		break;
		case PROTO_START:
		{
			NK()->setDelegate(NULL);
			Director::getInstance()->replaceScene(PrepareLayer::createScene());
		}
		break;
		case PROTO_PING_REQ:
		{
			long sendTime = ((ProtPing*)data)->time;
			long curTime = GameMgr()->getCurrentTime();
			long ping = curTime - sendTime;

			printf("======== PROTO_PING_REQ time=%ld ping=%ld\n", curTime, ping);
			break;
		}
		case PROTO_ACTOR:
		{
			ProtChangeData* prot = (ProtChangeData*)data;
			GameMgr()->setOppoNameBoard(kHead, prot->headId);
			GameMgr()->setOppoNameBoard(kTitle, prot->titleId);
			GameMgr()->setOppoNameBoard(kShade, prot->shadeId);
			if (prot->mapId != 0)
			{
				GameMgr()->setMapId(prot->mapId);
			}
			break;
		}
		case PROTO_PREPARE:
		{
			ProtPrepare* prot = (ProtPrepare*)data;
			m_s_prepare = prot->prepare;

			if (m_s_prepare)
			{
			  m_cPreText1->setVisible(true);
			  m_cPreText0->setVisible(false);
			}
			else
			{
			  m_cPreText1->setVisible(false);
			  m_cPreText0->setVisible(true);
			}
			break;
		}
		case PROT_TEAM_COLOR:
		{
			RemoteColor* prot = (RemoteColor*)data;
			m_BtnColor_Up->setColor(GameMgr()->getTeamColor(prot->color));
			m_remoteTeamColor->setColor(GameMgr()->getTeamColor(m_ColorB));
			sersName->setString(prot->name);
			m_remoteName->setString(PLAYER_MGR()->localPlayer()->getName());
			m_mapSize->setString(__String::createWithFormat("%d", prot->mapID)->getCString());
			m_teamSize->setString(__String::createWithFormat("%d", prot->roomSize)->getCString());

			GameMgr()->setRoomMode((FightingMode)prot->roomMode);

			if (prot->roomMode == Mode_Kill_King)
			{
				m_roomMode0->setVisible(true);
				m_roomMode1->setVisible(false);
			}
			else
			{
				m_roomMode0->setVisible(false);
				m_roomMode1->setVisible(true);
			}
			break;
		}
		case PROT_WIFI_OK:
		{
			Director::getInstance()->replaceScene(PrepareLayer::createScene());
			break;
		}
	}
}

void RoomState::onServerDisconnect()
{
	if (m_disconnect)
		return;
	m_disconnect = true;
	NK()->stop();
	NK()->startServer(SERVER_PORT);
	NK()->startBroadServer(BROADCAST_PORT, m_broadcastStr.c_str());
	NK()->setDelegate(this);
}

void RoomState::onClientDisconnect()
{
	if (m_disconnect)
	{
		return;
	}
	m_disconnect = true;
	NK()->stop();
}

void RoomState::startHeartBeat()
{
	return;
	stopHeartBeat();

	auto delay = DelayTime::create(10.0f);
	auto cb = CallFunc::create(CC_CALLBACK_0(RoomState::heartBeatCall, this));
	m_heartAction = RepeatForever::create(Sequence::create(delay, cb, NULL));
	this->runAction(m_heartAction);
}

void RoomState::stopHeartBeat()
{
	if (m_heartAction)
	{
		this->stopAction(m_heartAction);
		m_heartAction = nullptr;
	}
}

void RoomState::heartBeatCall()
{
	ProtHead prot = { 0 };
	prot.protoId = PROTO_HEART_BEAT;
	NK()->send(&prot, sizeof(prot));
}

void RoomState::sendSetTime()
{
	ProtSetTime prot = { 0 };
	prot.head.protoId = PROTO_SET_TIME;
	prot.ctime = GameMgr()->getCurrentTime();
	NK()->send(&prot, sizeof(prot));
}

void RoomState::afterDisconnect()
{
	stopHeartBeat();
	if (GameMgr()->getIsServer())
	{
		// clear room
		PLAYER_MGR()->removeRemotePlayer();
		m_connect = false;
		//updateInformation();
	}
	else
	{
		//GAMEUI()->removeTopLayer();
	}
	m_disconnect = false;
}

void RoomState::update(float dt)
{
	if (m_disconnect)
	{
		afterDisconnect();
		return;
	}

	// 	if (m_connectFlag == false)
	// 	{
	// 		m_connectFlag = true;
	// 		NK()->stop();
	// 
	// 		this->unscheduleUpdate();
	// 	}
}

void RoomState::activation()
{
	GameMgr()->setClientServerDeltaTime(0);

	if (GameMgr()->getIsServer())
	{
		if (m_connect)
		{
			// send servers team color and local player name
			RemoteColor prot = { 0 };
			prot.head.protoId = PROT_TEAM_COLOR;
			prot.color = m_ColorA;
			std::string clientName = PLAYER_MGR()->localPlayer()->getName();
			strcpy(prot.name, clientName.c_str());
			prot.mapID = GameMgr()->getMapId();
			log("gameMgr get mapid = %d", prot.mapID);
			prot.roomSize = GameMgr()->getRoomSize();
			prot.roomMode = GameMgr()->getRoomMode();
			NK()->send(&prot, sizeof(prot));
		}
		else
		{
			char buffer[256];
			sprintf(buffer, "%s#%d#%d#%d", GameMgr()->getRoomName().c_str(), GameMgr()->getRoomMap(),
				GameMgr()->getRoomSize(), (int)GameMgr()->getRoomMode());
			m_broadcastStr = buffer;
			NK()->changeName(m_broadcastStr.c_str());
		}
	}
	else
	{
		ProtChangeData prot = { 0 };
		prot.head.protoId = PROTO_ACTOR;
		prot.shadeId = GameMgr()->getNameBoard(kShade);
		prot.headId = GameMgr()->getNameBoard(kHead);
		prot.titleId = GameMgr()->getNameBoard(kTitle);
		prot.mapId = GameMgr()->getMapId();
		NK()->send(&prot, sizeof(prot));

		m_setTimeNum = 0;
		m_setTimeValues.clear();
		m_pingValues.clear();
		auto cb = CallFunc::create(CC_CALLBACK_0(RoomState::sendSetTime, this));
		auto delay = DelayTime::create(0.5f);
		auto rp = Repeat::create(Sequence::create(delay, cb, NULL), 21);
		this->runAction(rp);
	}
}

void RoomState::active()
{
	activation();
}

void RoomState::deactive()
{

}

void RoomState::b4remove()
{
	ProtHead prot = { 0 };
	prot.protoId = PROTO_EXIT_ROOM;
	NK()->sendd(&prot, sizeof(prot));

	NK()->stop();
	NK()->stopBroadcast();
}