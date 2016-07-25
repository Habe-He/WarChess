#include "StarLayer.h"
#include "PublicTool/ToolFunc.h"
#include "SelectLayer/SelectLayer.h"
#include "WiFiFight/RoomListLayer.h"
#include "PublicTool/ToolFunc.h"
#include "Player/PlayerManager.h"
#include "GameManager/GameManager.h"
#include "PublicTool/FontChina.h"
#include "GameOverLayer/GameOverLayer.h"

Scene* StarLayer::scene()
{
	auto scene = Scene::create();
	auto layer = StarLayer::create();
	scene->addChild(layer);
	return scene;
}

bool StarLayer::init()
{
	auto spriteNor = Sprite::create("deouble.png");
	auto spriteClick = Sprite::create("deouble.png");
	spriteClick->setScale(0.5f);
	spriteClick->setPosition(Vec2(spriteNor->getPosition().x + spriteNor->getBoundingBox().size.width / 2 - spriteClick->getBoundingBox().size.width / 2,
		spriteNor->getPosition().y + spriteNor->getBoundingBox().size.height / 2 - spriteClick->getBoundingBox().size.height / 2));

	auto startButton = MenuItemSprite::create(spriteNor, spriteClick, CC_CALLBACK_1(StarLayer::starMenuCallBack, this));
	startButton->setPosition(Vec2(300, 400));

	auto wifiNor = Sprite::create("wifi.png");
	auto wifiClick = Sprite::create("wifi.png");
	wifiClick->setScale(0.5f);
	wifiClick->setPosition(Vec2(wifiNor->getPosition().x + wifiNor->getBoundingBox().size.width / 2 - wifiClick->getBoundingBox().size.width / 2,
		wifiNor->getPosition().y + wifiNor->getBoundingBox().size.height / 2 - wifiClick->getBoundingBox().size.height / 2));
	auto wifiButton = MenuItemSprite::create(wifiNor, wifiClick, CC_CALLBACK_1(StarLayer::wifiFightingCall, this));
	wifiButton->setPosition(Vec2(600, 400));

	auto pMenu = Menu::create(startButton, wifiButton, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 10);


	auto userName = __String::createWithFormat("User%d", ToolFunc::calcRandomNum(1, 1000));
	PLAYER_MGR()->createPlayer(userName->getCString(), true);

	auto nameLabel = Label::createWithSystemFont(userName->getCString(), "", 20);
	nameLabel->setPosition(Vec2(480, 120));
	nameLabel->setAnchorPoint(Vec2(0, 0.5));
	this->addChild(nameLabel, 10);

	auto sprite = Sprite::create("name.png");
	sprite->setPosition(Vec2(450, 120));
	this->addChild(sprite);

	return true;
}

void StarLayer::starMenuCallBack(Ref* pSender)
{
	GameMgr()->setGameFightType(GFT_AI);
	Director::getInstance()->replaceScene(SelectLayer::create());
}

void StarLayer::wifiFightingCall(Ref* pSender)
{
	GameMgr()->setGameFightType(GFT_WiFi);
	Director::getInstance()->replaceScene(RoomListLayer::createScene());
}