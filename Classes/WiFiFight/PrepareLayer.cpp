#include "PrepareLayer.h"
#include "GameManager/GameManager.h"
#include "NetKit/NetMessage.h"
#include "Player/PlayerManager.h"

#include "SelectLayer/SelectLayer.h"
#include "DataManager/DataManager.h"
#include "SelectLayer/RoleSelect.h"
#include "PublicTool/ComResDef.h"
#include "GameLayer/GameLayer.h"

PrepareLayer::PrepareLayer()
{
	m_prepare = false;
	m_setTimeNum = 0;
	m_connect = false;
	m_disconnect = false;
	m_connectFlag = false;
	m_heartAction = nullptr;
}

PrepareLayer::~PrepareLayer()
{
	
}

Scene* PrepareLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(PrepareLayer::create());

	return scene;
}

bool PrepareLayer::init()
{
	initRolePhoto();

	auto spriteNor = Sprite::create("sssssstart.png");
	auto spriteClick = Sprite::create("sssssstart.png");
	spriteClick->setScale(0.5f);
	spriteClick->setPosition(Vec2(spriteNor->getPosition().x + spriteNor->getBoundingBox().size.width / 2 - spriteClick->getBoundingBox().size.width / 2,
		spriteNor->getPosition().y + spriteNor->getBoundingBox().size.height / 2 - spriteClick->getBoundingBox().size.height / 2));

	auto startMenu = MenuItemSprite::create(spriteNor, spriteClick, CC_CALLBACK_1(PrepareLayer::startButton, this));
	startMenu->setTag(ServersBtnFlag);
	startMenu->setPosition(Vec2(200, 200));


	auto wifiNor = Sprite::create("Prepared.png");
	auto wifiClick = Sprite::create("Prepared.png");
	wifiClick->setScale(0.5f);
	wifiClick->setPosition(Vec2(wifiNor->getPosition().x + wifiNor->getBoundingBox().size.width / 2 - wifiClick->getBoundingBox().size.width / 2,
		wifiNor->getPosition().y + wifiNor->getBoundingBox().size.height / 2 - wifiClick->getBoundingBox().size.height / 2));
	auto prepareMenu = MenuItemSprite::create(wifiNor, wifiClick, CC_CALLBACK_1(PrepareLayer::prepareButton, this));
	prepareMenu->setTag(ClientBtnFlag);
	prepareMenu->setPosition(Vec2(400, 200));

	auto pMenu = Menu::create(startMenu, prepareMenu, NULL);
	pMenu->setPosition(Vec2(0, 0));
	this->addChild(pMenu, 10, PLMenuFlag);

	NK()->setDelegate(this);

	if (GameMgr()->getIsServer())
	{
		prepareMenu->setVisible(false);
		prepareMenu->setEnabled(false);

		auto sprite = Sprite::create("duishouzhunbei.png");
		sprite->setTag(10000);
		sprite->setPosition(Vec2(100, 100  ));
		this->addChild(sprite);

		char buffer[256];
		sprintf(buffer, "%s#%d", PLAYER_MGR()->localPlayer()->getName(), GameMgr()->getMapId());
		m_broadcastStr = buffer;
		NK()->stopBroadcast();
		NK()->startServer(SERVER_PORT);
		NK()->startBroadServer(BROADCAST_PORT, m_broadcastStr.c_str());
	}
	else
	{
		startMenu->setVisible(false);
		startMenu->setEnabled(false);

		ProtClientOk prot = { 0 };
		prot.head.protoId = PROTO_CLIENT_OK;
		prot.prepareOK = false;
		NK()->send(&prot, sizeof(prot));
		startHeartBeat();
	}

	scheduleUpdate();
	activation();

	updateTwoInformation();

	return true;
}

void PrepareLayer::startButton(Ref* pSender)
{
	// 本机作为服务端时：对方玩家点击准备以后，才可以开始游戏
	if (m_prepare && starClick())
	{
// 		auto pMenu = (Menu*)this->getChildByTag(PLMenuFlag);
// 		auto startMenu = (MenuItemLabel*)pMenu->getChildByTag(ServersBtnFlag);
// 		startMenu->setString("I'm Client");
// 		startMenu->setEnabled(true);
		
		ProtStart prot = { 0 };
		prot.head.protoId = PROTO_START;
		NK()->send(&prot, sizeof(prot));
		NK()->setDelegate(NULL);
		Director::getInstance()->replaceScene(GameLayer::scene());
	}
}

void PrepareLayer::prepareButton(Ref* pSender)
{
	if ( starClick() )
	{
		auto pMenu = (Menu*)this->getChildByTag(PLMenuFlag);
		auto prepareMenu = (MenuItemSprite*)pMenu->getChildByTag(ClientBtnFlag);
		prepareMenu->setVisible(false);

		setClientButtonEnable(m_prepare);
		m_prepare = !m_prepare;

		ProtPrepare prot = { 0 };
		prot.head.protoId = PROTO_PREPARE;
		prot.prepare = m_prepare;
		NK()->send(&prot, sizeof(prot));
		updateTwoInformation();
	}	
}

void PrepareLayer::updateTwoInformation()
{
// 	auto pMenu = (Menu*)this->getChildByTag(PLMenuFlag);
	if (GameMgr()->getIsServer())
	{
// 		auto prepareMenu = (MenuItemLabel*)pMenu->getChildByTag(ClientBtnFlag);
// 		prepareMenu->setString("I'm Servers");
// 		prepareMenu->setEnabled(false);
	}
	else
	{
// 		auto startMenu = (MenuItemLabel*)pMenu->getChildByTag(ServersBtnFlag);
// 		startMenu->setString("I'm Client");
// 		startMenu->setEnabled(false);
	}
}

void PrepareLayer::setClientButtonEnable(bool enable)
{
	
}

void PrepareLayer::setServerButtonEnable(bool enable)
{
	
}

void PrepareLayer::onServerMessage(void* data)
{
	ProtHead* head = (ProtHead*)data;
	switch (head->protoId)
	{
		case PROTO_HEART_BEAT:
		{
			break;
		}
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

			prot1.mapId = GameMgr()->getMapId();
			std::string serverName = PLAYER_MGR()->localPlayer()->getName();
			strncpy(prot1.name, serverName.c_str(), sizeof(prot1.name) - 1);
			NK()->send(&prot1, sizeof(prot1));

			m_disconnect = false;
			startHeartBeat();
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
			m_prepare = prot->prepare;

			this->removeChildByTag(111111);
			auto label = Label::createWithSystemFont("", "", 20);
			label->setPosition(Vec2(600, 150));
			label->setTag(111111);
			this->addChild(label, 10);
			if ( m_prepare )
			{
				this->removeChildByTag(10000);

				auto sprite = Sprite::create("duishouwancheng.png");
				sprite->setTag(10000);
				sprite->setPosition(Vec2(100, 100));
				this->addChild(sprite);
			}
			else
			{
				this->removeChildByTag(10000);

				auto sprite = Sprite::create("duishouzhunbei.png");
				sprite->setTag(10000);
				sprite->setPosition(Vec2(100, 100));
				this->addChild(sprite);
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
	}
}

void PrepareLayer::onClientMessage(void* data)
{
	ProtHead* head = (ProtHead*)data;
	switch (head->protoId)
	{
		case PROTO_HEART_BEAT:
		{
			break;
		}
		case PROTO_SET_TIME:
		{
			m_setTimeNum ++;
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
			Director::getInstance()->replaceScene(GameLayer::scene());
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
	}
}

void PrepareLayer::onServerDisconnect()
{
	if (m_disconnect)
		return;
	m_disconnect = true;
	NK()->stop();
	NK()->startServer(SERVER_PORT);
	NK()->startBroadServer(BROADCAST_PORT, m_broadcastStr.c_str());
	NK()->setDelegate(this);
}

void PrepareLayer::onClientDisconnect()
{
	if (m_disconnect)
	{
		return;
	}
	m_disconnect = true;
	NK()->stop();
}

void PrepareLayer::startHeartBeat()
{
	return;
	stopHeartBeat();

	auto delay = DelayTime::create(10.0f);
	auto cb = CallFunc::create(CC_CALLBACK_0(PrepareLayer::heartBeatCall, this));
	m_heartAction = RepeatForever::create(Sequence::create(delay, cb, NULL));
	this->runAction(m_heartAction);
}

void PrepareLayer::stopHeartBeat()
{
	if (m_heartAction)
	{
		this->stopAction(m_heartAction);
		m_heartAction = nullptr;
	}
}

void PrepareLayer::heartBeatCall()
{
	ProtHead prot = { 0 };
	prot.protoId = PROTO_HEART_BEAT;
	NK()->send(&prot, sizeof(prot));
}

void PrepareLayer::sendSetTime()
{
	ProtSetTime prot = { 0 };
	prot.head.protoId = PROTO_SET_TIME;
	prot.ctime = GameMgr()->getCurrentTime();
	NK()->send(&prot, sizeof(prot));
}

void PrepareLayer::afterDisconnect()
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

void PrepareLayer::update(float dt)
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

void PrepareLayer::activation()
{
	GameMgr()->setClientServerDeltaTime(0);

	if (GameMgr()->getIsServer())
	{
		if (m_connect)
		{
			// send message
			ProtChangeData prot = { 0 };
			prot.head.protoId = PROTO_ACTOR;
			prot.shadeId = GameMgr()->getNameBoard(kShade);
			prot.headId = GameMgr()->getNameBoard(kHead);
			prot.titleId = GameMgr()->getNameBoard(kTitle);
			prot.mapId = GameMgr()->getMapId();
			NK()->send(&prot, sizeof(prot));
		}
		else
		{
			char buffer[256];
			sprintf(buffer, "%s#%d", PLAYER_MGR()->localPlayer()->getName(), GameMgr()->getMapId());
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
		auto cb = CallFunc::create(CC_CALLBACK_0(PrepareLayer::sendSetTime, this));
		auto delay = DelayTime::create(0.5f);
		auto rp = Repeat::create(Sequence::create(delay, cb, NULL), 21);
		this->runAction(rp);
	}
}

void PrepareLayer::active()
{
	activation();
}

void PrepareLayer::deactive()
{
	
}

void PrepareLayer::b4remove()
{
	ProtHead prot = { 0 };
	prot.protoId = PROTO_EXIT_ROOM;
	NK()->sendd(&prot, sizeof(prot));

	NK()->stop();
	NK()->stopBroadcast();
}




void PrepareLayer::initRolePhoto()
{
	mainLandValue = 0;
	skyValue = 0;
	geocenterValue = 0;
	DataMgr()->roleInBattle.clear();

	auto bg = Sprite::create("MapSprite/Map_1.png");
	bg->setAnchorPoint(Vec2::ZERO);
	this->addChild(bg, -2);
	auto roleVector = DataMgr()->getVRolePropty();
	vector<RoleProprty > mainLandVector;
	mainLandVector.clear();
	vector<RoleProprty > skyVector;
	skyVector.clear();
	vector<RoleProprty > geocenterVector;
	geocenterVector.clear();

	for (auto itor : roleVector)
	{
		if (itor.sR_RolePossess == P_MainLand)
		{
			mainLandVector.push_back(itor);
		}
		else if (itor.sR_RolePossess == P_Sky)
		{
			skyVector.push_back(itor);
		}
		else if (itor.sR_RolePossess == P_Geocenter)
		{
			geocenterVector.push_back(itor);
		}
	}

	for (unsigned int i = 0; i < mainLandVector.size(); ++i)
	{
		auto role = (RoleProprty)mainLandVector[i];
		auto mainLandRole = RoleSelect::createInstance(role.sR_ID);
		mainLandRole->setScale(0.6f);
		mainLandRole->setTag(role.sR_ID);
		mainLandRole->setPosition(Vec2(60 + (i % 4) * 60, 500 - (i / 4) * 80));
		this->addChild(mainLandRole, 10);
	}

	for (unsigned int i = 0; i < skyVector.size(); ++i)
	{
		auto role = (RoleProprty)skyVector[i];
		auto skyRole = RoleSelect::createInstance(role.sR_ID);
		skyRole->setScale(0.6f);
		skyRole->setTag(role.sR_ID);
		skyRole->setPosition(Vec2(360 + (i % 4) * 60, 500 - (i / 4) * 80));
		this->addChild(skyRole, 10);
	}

	for (unsigned int i = 0; i < geocenterVector.size(); ++i)
	{
		auto role = (RoleProprty)geocenterVector[i];
		auto geocenterRole = RoleSelect::createInstance(role.sR_ID);
		geocenterRole->setScale(0.6f);
		geocenterRole->setTag(role.sR_ID);
		geocenterRole->setPosition(Vec2(660 + i % 4 * 60, 500 - i / 4 * 80));
		this->addChild(geocenterRole, 10);
	}

	mainLandLabel = Label::createWithTTF("0 / 450", PixelFont, 20);
	mainLandLabel->setPosition(Vec2(150, 600));
	this->addChild(mainLandLabel, 2);

	auto sprite = Sprite::create("mainland.png");
	sprite->setPosition(Vec2(150, 560));
	this->addChild(sprite);

	skyLabel = Label::createWithTTF("0 / 450", PixelFont, 20);
	skyLabel->setPosition(Vec2(450, 600));
	this->addChild(skyLabel, 2);

	auto sprite2 = Sprite::create("sky.png");
	sprite2->setPosition(Vec2(450, 560));
	this->addChild(sprite2);

	geocenterLabel = Label::createWithTTF("0 / 450", PixelFont, 20);
	geocenterLabel->setPosition(Vec2(750, 600));
	this->addChild(geocenterLabel, 2);

	auto sprite1 = Sprite::create("ceo.png");
	sprite1->setPosition(Vec2(750, 560));
	this->addChild(sprite1);
}

bool PrepareLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto roleVector = DataManager::getInstance()->getVRolePropty();
	auto touchPoint = touch->getLocation();

	for (unsigned int i = 0; i < roleVector.size(); ++i)
	{
		auto roleFrame = (RoleSelect*)this->getChildByTag(i + RoleStarFlag);
		auto size = roleFrame->getFrameRect() * roleFrame->getScale();
		auto position = roleFrame->getPosition();
		auto rect = Rect(position.x - size.width / 2, position.y - size.height / 2, size.width, size.height);

		if (rect.containsPoint(touchPoint))
		{
			int id = roleFrame->getTag();
			int marshalValue = roleVector[id - RoleStarFlag].sR_Marshal;
			if (roleFrame->isSelect())
			{
				roleFrame->setGray(false);
				calcMarshal(roleVector[id - RoleStarFlag].sR_RolePossess, -marshalValue);
				auto roleBattle = DataManager::getInstance()->getRoleInBattle();
				for (vector<int>::iterator iter = roleBattle.begin(); iter != roleBattle.end();)
				{
					if (*iter == id)
						iter = roleBattle.erase(iter);
					else
						iter++;
					DataMgr()->roleInBattle.clear();
					DataMgr()->roleInBattle = roleBattle;
				}
			}
			else
			{
				roleFrame->setGray(true);
				calcMarshal(roleVector[id - RoleStarFlag].sR_RolePossess, marshalValue);
				DataMgr()->roleInBattle.push_back(id);
			}
		}
	}
	return true;
}

void PrepareLayer::calcMarshal(RolePossess _possess, int iMarshal)
{
	__String* valueString = nullptr;
	switch (_possess)
	{
	case P_MainLand:
		mainLandValue += iMarshal;
		valueString = __String::createWithFormat("%d / 450", mainLandValue);
		mainLandLabel->setString(valueString->getCString());
		break;
	case P_Geocenter:
		geocenterValue += iMarshal;
		valueString = __String::createWithFormat("%d / 450", geocenterValue);
		geocenterLabel->setString(valueString->getCString());
		break;
	case P_Sky:
		skyValue += iMarshal;
		valueString = __String::createWithFormat("%d / 450", skyValue);
		skyLabel->setString(valueString->getCString());
		break;
	}
}

void PrepareLayer::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PrepareLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PrepareLayer::onExit()
{
	Layer::onExit();
}

bool PrepareLayer::starClick()
{
	if (GameMgr()->getGameFightType() == GFT_AI)
	{
		if (mainLandValue > 0 && skyValue > 0 && geocenterValue > 0)
		{
			auto tipsLabel = Label::createWithSystemFont("Only Select Two Possess", "", 20);
			this->addChild(tipsLabel, 20);
			tipsLabel->setPosition(Vec2(480, 100));

			auto moveto = MoveTo::create(1.5f, Vec2(480, 300));
			auto callBack = CallFunc::create([=]()
			{
				tipsLabel->removeFromParentAndCleanup(true);
			});
			tipsLabel->runAction(Sequence::create(moveto, callBack, NULL));
			return false;
		}
		else if (mainLandValue == 0 && skyValue == 0 && geocenterValue == 0)
		{
			return false;
		}
		else if (mainLandValue > 450 || skyValue > 450 || geocenterValue > 450)
		{
			return false;
		}
		else
		{
			bool bMainLand = false;
			bool bSky = false;
			bool bGeocenter = false;
			auto roleInBattle = DataManager::getInstance()->getRoleInBattle();
			auto roleVector = DataManager::getInstance()->getVRolePropty();
			for (unsigned int i = 0; i < roleInBattle.size(); ++i)
			{
				int id = roleInBattle[i];
				if (id == 90002)
				{
					bSky = true;
				}
				else if (id == 90018)
				{
					bGeocenter = true;
				}
				else if (id == 90027)
				{
					bMainLand = true;
				}
			}

			if ((bSky && bMainLand) || (bSky && bGeocenter) || (bMainLand && bSky) || (bMainLand && bGeocenter) || (bGeocenter && bSky) || (bGeocenter && bMainLand))
			{
				return true;
			}
			else
			{
				auto tipsLabel = Label::createWithSystemFont("Select King", "", 20);

				this->addChild(tipsLabel, 20);
				tipsLabel->setPosition(Vec2(480, 100));

				auto moveto = MoveTo::create(1.5f, Vec2(480, 300));
				auto callBack = CallFunc::create([=]()
				{
					tipsLabel->removeFromParentAndCleanup(true);
				});
				tipsLabel->runAction(Sequence::create(moveto, callBack, NULL));
				return false;
			}
		}
	}
	else if (GameMgr()->getGameFightType() == GFT_WiFi)
	{
		if (((mainLandValue > 0 || skyValue > 0) && geocenterValue > 0)
			|| (mainLandValue > 0 && (skyValue > 0 || geocenterValue > 0))
			|| (skyValue > 0 && (mainLandValue > 0 || geocenterValue > 0))
			)
		{
			auto tipsLabel = Label::createWithSystemFont("Only Select One Possess", "", 20);
			this->addChild(tipsLabel, 20);
			tipsLabel->setPosition(Vec2(480, 100));

			auto moveto = MoveTo::create(1.5f, Vec2(480, 300));
			auto callBack = CallFunc::create([=]()
			{
				tipsLabel->removeFromParentAndCleanup(true);
			});
			tipsLabel->runAction(Sequence::create(moveto, callBack, NULL));
			return false;
		}
		else if (mainLandValue == 0 && skyValue == 0 && geocenterValue == 0)
		{
			return false;
		}
		else if (mainLandValue > 450 || skyValue > 450 || geocenterValue > 450)
		{
			return false;
		}
		else
		{
			bool bMainLand = false;
			bool bSky = false;
			bool bGeocenter = false;
			auto roleInBattle = DataManager::getInstance()->getRoleInBattle();
			auto roleVector = DataManager::getInstance()->getVRolePropty();
			for (unsigned int i = 0; i < roleInBattle.size(); ++i)
			{
				int id = roleInBattle[i];
				if (id == 90002)
				{
					bSky = true;
				}
				else if (id == 90018)
				{
					bGeocenter = true;
				}
				else if (id == 90027)
				{
					bMainLand = true;
				}
			}

			if (bSky || bMainLand || bGeocenter)
			{
				return true;
			}
			else
			{
				auto tipsLabel = Label::createWithSystemFont("Select King", "", 20);

				this->addChild(tipsLabel, 20);
				tipsLabel->setPosition(Vec2(480, 100));

				auto moveto = MoveTo::create(1.5f, Vec2(480, 300));
				auto callBack = CallFunc::create([=]()
				{
					tipsLabel->removeFromParentAndCleanup(true);
				});
				tipsLabel->runAction(Sequence::create(moveto, callBack, NULL));
				return false;
			}
		}
	}
	return true;
}