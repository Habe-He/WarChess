#include "GameOverLayer.h"
#include "WiFiFight/CWiFiRoom.h"
#include "StarLayer/StarLayer.h"

bool GameOverLayer::init()
{
	auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 166));
	this->addChild(maskLayer);

	m_resultLable = Label::createWithSystemFont("", "", 24);
	m_resultLable->setPosition(Vec2(480, 320));
	m_resultLable->setColor(Color3B::BLACK);
	maskLayer->addChild(m_resultLable);

	auto onceSprite = Sprite::create("returnRoom.png");
	auto onceMenu = MenuItemSprite::create(onceSprite, onceSprite, CC_CALLBACK_1(GameOverLayer::onceAgainClick, this));
	onceMenu->setPosition(Vec2(200, 200));

	auto exitLabel = Sprite::create("exitRoom.png");
	auto exitMenu = MenuItemSprite::create(exitLabel, exitLabel, CC_CALLBACK_1(GameOverLayer::exitRoomClick, this));
	exitMenu->setPosition(Vec2(600, 200));

	pMenu = Menu::create(onceMenu, exitMenu, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 10);
	
	return true;
}

void GameOverLayer::onceAgainClick(Ref* pSender)
{
	Director::getInstance()->replaceScene(CWiFiRoom::createScene());
}

void GameOverLayer::exitRoomClick(Ref* pSender)
{
	Director::getInstance()->replaceScene(StarLayer::scene());
}

void GameOverLayer::setResult(bool isWin)
{
	if (isWin)
		m_resultLable->setString("Client Disconnect You Win");
	else
		m_resultLable->setString("Server Disconnect You Win");

	auto callBack = CallFunc::create([=]
	{
		pMenu->setVisible(true);
		pMenu->setEnabled(true);
	});

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), callBack));
}

void GameOverLayer::onEnter()
{
	Node::onEnter();
}

void GameOverLayer::onExit()
{
	Node::onExit();
}