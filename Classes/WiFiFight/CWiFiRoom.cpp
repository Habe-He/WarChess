#include "CWiFiRoom.h"
#include "RoomListLayer.h"
#include "NetKit/NetMessage.h"
#include "RoomState.h"
#include "GameManager/GameManager.h"

CWiFiRoom::CWiFiRoom()
{
	mode = Mode_Kill_All;
	viewState = View_Off;
	m_mapID = 0;
	m_roomSize = 1;
}

CWiFiRoom::~CWiFiRoom()
{
	
}

Scene* CWiFiRoom::createScene()
{
	auto scene = Scene::create();
	scene->addChild(CWiFiRoom::create());

	return scene;
}

bool CWiFiRoom::init()
{
	auto wifiRoomNode = CSLoader::createNode("CWiFiRoom.csb");
	this->addChild(wifiRoomNode, -10);

	textMode0 = (Text*)wifiRoomNode->getChildByName("Text_0");
	textMode1 = (Text*)wifiRoomNode->getChildByName("Text_1");
	textMode1->setVisible(false);

	textView0 = (Text*)wifiRoomNode->getChildByName("Text_0_1");
	textView1 = (Text*)wifiRoomNode->getChildByName("Text_0_0");

	textRoleCount = (Text*)wifiRoomNode->getChildByName("Text_Size");
	textRoleCount->setString("1-1");

	roomName = (TextField*)wifiRoomNode->getChildByName("TextField_RoomName");

	for (int i = 1; i < 9; ++ i)
	{
		auto numString = __String::createWithFormat("Btn_%d", i);
		auto btnNum = (Button*)wifiRoomNode->getChildByName(numString->getCString());
		btnNum->setTag(i);
		btnNum->addTouchEventListener(CC_CALLBACK_2(CWiFiRoom::numClick, this));
	}

	auto modeBtn = (Button*)wifiRoomNode->getChildByName("Button_9");
	modeBtn->addTouchEventListener(CC_CALLBACK_2(CWiFiRoom::modeClick, this));

	auto viewBtn = (Button*)wifiRoomNode->getChildByName("Button_10");
	viewBtn->addTouchEventListener(CC_CALLBACK_2(CWiFiRoom::viewClick, this));

	auto getBackBtn = (Button*)wifiRoomNode->getChildByName("Btn_Backup");
	getBackBtn->addTouchEventListener(CC_CALLBACK_2(CWiFiRoom::getBackClick, this));

	auto createBtn = (Button*)wifiRoomNode->getChildByName("Btn_Create");
	createBtn->addTouchEventListener(CC_CALLBACK_2(CWiFiRoom::createRoom, this));
	return true;
}

void CWiFiRoom::numClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		auto btnNum = (Button*)pSender;
		m_roomSize = btnNum->getTag();
		auto numString = __String::createWithFormat("1-%d", m_roomSize);
		textRoleCount->setString(numString->getCString());
	}	
}

void CWiFiRoom::modeClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		if (textMode0->isVisible())
		{
			mode = Mode_Kill_King;
			textMode0->setVisible(false);
			textMode1->setVisible(true);
		}
		else
		{
			mode = Mode_Kill_All;
			textMode0->setVisible(true);
			textMode1->setVisible(false);
		}
	}	
}

void CWiFiRoom::viewClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		if (textView0->isVisible())
		{
			viewState = View_On;
			textView0->setVisible(false);
			textView1->setVisible(true);
		}
		else
		{
			viewState = View_Off;
			textView0->setVisible(true);
			textView1->setVisible(false);
		}
	}	
}

void CWiFiRoom::getBackClick(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		Director::getInstance()->replaceScene(RoomListLayer::createScene());
	}	
}

void CWiFiRoom::createRoom(Ref* pSender, Widget::TouchEventType type)
{
	if ( type == Widget::TouchEventType::ENDED )
	{
		// 房间名不为空
		if (roomName->getString().c_str() != NULL)
		{			
			GameMgr()->setRoomName(roomName->getString());
			GameMgr()->setRoomSize(m_roomSize);
			GameMgr()->setRoomMode(mode);
			GameMgr()->setRoomView(viewState);

			Director::getInstance()->replaceScene(RoomState::createScene());
		}
	}
}