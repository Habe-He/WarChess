#include "GameLayer.h"
#include "PublicTool/ToolFunc.h"
#include "DataManager/DataManager.h"
#include "Prompt/Prompt.h"
#include "StarLayer/StarLayer.h"

#include "Hero/FlyProps/FlyProps.h"
#include "RolePosition/RolePosition.h"
#include "Hero/Role/Assassin.h"
#include "NetKit/NetMessage.h"
#include "GameManager/GameManager.h"
#include "GameOverLayer/GameOverLayer.h"

GameLayer::GameLayer(void) :
tipsCount(TipsFlag), hittedHero1(NULL), hittedHero2(NULL), isRunning(false)
{
	bgOrigin = Vec2(Vec2::ZERO);
	aStartInstance = nullptr;
	pathRole = nullptr;
	isSelectCurRole = false;
	isTouchScreen = true;
	m_ServersCoord = Vec2(-1, -1);
}

GameLayer::~GameLayer(void)
{

}

Scene* GameLayer::scene()
{
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init()
{
	_chessRule = ChessRule::getInstance();

	// 当为WiFi联机对战时，发送当前队伍信息
	if (GameMgr()->getGameFightType() == GFT_WiFi)
	{
		NK()->setDelegate(this);

		ProtExchangeTeam prot = { 0 };
		prot.head.protoId = PROT_EXCHANGE_TEAM;

		__String* idStr;
		std::string idstr;
		for (auto itor : DataMgr()->getRoleInBattle())
		{
			idStr = __String::createWithFormat("%d#", itor);
			idstr.append(idStr->getCString());
		}
		memset(prot.c_team, 0, sizeof(prot.c_team));
		memcpy(prot.c_team, idstr.c_str(), idstr.length());
		NK()->send(&prot, sizeof(prot));
	}

	loadMap();
	initBottom();
	if ( GameMgr()->getGameFightType() != GFT_WiFi )
	{
		initHero();
		sortByAgility();
		showCurRole(FirstOrder);

		// init role frame photo
		m_RolePosMagr = RolePosManager::create();
		this->addChild(m_RolePosMagr, UILayer - 10);
		m_RolePosMagr->showRoleOrder(curVector, preVectot, FirstOrder);
	}

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::notCallBack), AorrwNotCall, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::notCallBack), ArrowCounter, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::notCallBack), PassTouch, NULL);

	this->scheduleUpdate();

	this->unscheduleUpdate();

	labelsss = Label::createWithSystemFont("", "", 16);
	labelsss->setPosition(Vec2( 80, 500 ));
	labelsss->setColor(Color3B::BLACK);
	labelsss->setAnchorPoint(Vec2(0, 0.5));
	this->addChild(labelsss, UILayer);
	
	return true;
}

void GameLayer::loadMap()
{
	bgSprite = Sprite::create("MapSprite/Map_1.png");
	bgSprite->setScale(0.7f);
	bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgSprite->setPosition(Vec2(480, 320));
	this->addChild(bgSprite, SpriteBg);	

	tmxMap = TMXTiledMap::create("TileMap1.tmx");
	for (auto itor : tmxMap->getChildren())
	{
		auto child = (TMXLayer*)itor;
		child->getTexture()->setAliasTexParameters();
	}
	tmxMap->setPosition(Vec2(240, 250));
	bgSprite->addChild(tmxMap);

	// 传递地图宽高
	DataManager::getInstance()->tileMapWidth = tmxMap->getMapSize().width - 1;
	DataManager::getInstance()->tileMapHeight = tmxMap->getMapSize().height - 1;

	OffSet.x = 65;
	OffSet.y = 32.5;

// 	bgOrigin = Vec2(bgSprite->getPositionX() - bgSprite->getContentSize().width * bgSprite->getScale() / 2,
// 		bgSprite->getPositionY() - bgSprite->getContentSize().height * bgSprite->getScale() / 2);


 	auto scaleto = ScaleTo::create(0.5f, 0.9f, 0.9f, 1.0f );
	auto callback = CallFunc::create([this]()
	{
		bgOrigin = Vec2(bgSprite->getPositionX() - bgSprite->getContentSize().width * bgSprite->getScale() / 2,
			bgSprite->getPositionY() - bgSprite->getContentSize().height * bgSprite->getScale() / 2);
	});
	bgSprite->runAction(Sequence::create(scaleto, DelayTime::create(0.5f), callback, NULL));	
}

void GameLayer::initHero()
{
	DataManager::getInstance()->heroVector.clear();
	Vec2 leftPoint[8] = {Vec2(2, 7), Vec2(2, 6), Vec2( 3, 7 ), Vec2(3, 6), Vec2(4, 7), Vec2(4, 6), Vec2(5, 7), Vec2(5, 6)};
	Vec2 rightPoint[8] = {Vec2(5, 0), Vec2(5, 1), Vec2(4, 0), Vec2(4, 1), Vec2(3, 0), Vec2(3, 1), Vec2(2, 0), Vec2(2, 1)};

	auto rolePropty = DataManager::getInstance()->getVRolePropty();
	auto roleVector = DataManager::getInstance()->getRoleInBattle();

	if (GameMgr()->getGameFightType() == GFT_WiFi)
	{
		for (unsigned int i = 0; i < roleVector.size(); ++i)
		{
			auto roleLocal = rolePropty[roleVector[i] - RoleStarFlag];
			auto role = Hero::createRole(roleLocal.sR_RoleType, roleLocal.sR_RolePossess);
			role->setRoleHDA(roleLocal.sR_Attack, roleLocal.sR_HP, roleLocal.sR_Agility, roleLocal.sR_ChinaName);
			role->setRoleID(roleLocal.sR_ID);
			tmxMap->addChild(role, 10);
			if (GameMgr()->getIsServer())
			{
				role->setRoleCoord(leftPoint[i]);
				auto redTile = coverToScreenCoord(leftPoint[i]);
				role->setPosition(redTile);
				role->setRoleDirAndSkill(S_Standby, D_Up);
				role->setSkinColor(GameMgr()->getLocalColor());
			}
			else
			{
				role->setRoleCoord(rightPoint[i]);
				auto redTile = coverToScreenCoord(rightPoint[i]);
				role->setPosition(redTile);
				role->setRoleDirAndSkill(S_Standby, D_Down);
				role->setSkinColor(GameMgr()->getRemoteColor());
			}
			DataManager::getInstance()->heroVector.push_back(role);
		}

		for (unsigned int i = 0; i < m_remoteTeam.size(); ++i)
		{
			if (m_remoteTeam[i] == 0)
				continue;
			auto roleLocal = rolePropty[m_remoteTeam[i] - RoleStarFlag];
			auto role = Hero::createRole(roleLocal.sR_RoleType, roleLocal.sR_RolePossess);
			role->setRoleHDA(roleLocal.sR_Attack, roleLocal.sR_HP, roleLocal.sR_Agility, roleLocal.sR_ChinaName);
			tmxMap->addChild(role, 10);
			role->setRoleID(roleLocal.sR_ID);
			if (GameMgr()->getIsServer())
			{
				role->setRoleCoord(rightPoint[i]);
				auto redTile = coverToScreenCoord(rightPoint[i]);
				role->setPosition(redTile);
				role->setRoleDirAndSkill(S_Standby, D_Down);
				role->setSkinColor(GameMgr()->getRemoteColor());
			}
			else
			{
				role->setRoleCoord(leftPoint[i]);
				auto redTile = coverToScreenCoord(leftPoint[i]);
				role->setPosition(redTile);
				role->setRoleDirAndSkill(S_Standby, D_Up);
				role->setSkinColor(GameMgr()->getLocalColor());
			}
			DataManager::getInstance()->heroVector.push_back(role);
		}
	}
	else
	{
		vector<int > mainLand;
		mainLand.clear();
		vector<int > sky;
		sky.clear();
		vector<int > geocenter;
		geocenter.clear();
		for (auto itor : roleVector)
		{
			auto poss = rolePropty[itor - RoleStarFlag].sR_RolePossess;
			if (poss == P_MainLand)
			{
				mainLand.push_back(itor);
			}
			else if (poss == P_Geocenter)
			{
				geocenter.push_back(itor);
			}
			else if (poss == P_Sky)
			{
				sky.push_back(itor);
			}
		}

		if (mainLand.size() > 0 && sky.size() > 0)
		{
			for (unsigned int i = 0; i < mainLand.size(); ++i)
			{
				auto role1 = rolePropty[mainLand[i] - RoleStarFlag];
				auto role = Hero::createRole(role1.sR_RoleType, role1.sR_RolePossess);
				role->setRoleHDA(role1.sR_Attack, role1.sR_HP, role1.sR_Agility, role1.sR_ChinaName);
				role->setSkinColor(3);
				role->setRoleID(role1.sR_ID);
				tmxMap->addChild(role, 10);

				role->setRoleDirAndSkill(S_Standby, D_Up);
				role->setRoleCoord(leftPoint[i]);
				auto redTile = coverToScreenCoord(leftPoint[i]);
				role->setPosition(redTile);
				DataManager::getInstance()->heroVector.push_back(role);
			}

			for (unsigned int i = 0; i < sky.size(); ++i)
			{
				auto role1 = rolePropty[sky[i] - RoleStarFlag];
				auto role = Hero::createRole(role1.sR_RoleType, role1.sR_RolePossess);
				role->setRoleHDA(role1.sR_Attack, role1.sR_HP, role1.sR_Agility, role1.sR_ChinaName);
				role->setSkinColor(0);
				tmxMap->addChild(role, 10);
				role->setRoleID(role1.sR_ID);

				role->setRoleDirAndSkill(S_Standby, D_Down);
				role->setRoleCoord(rightPoint[i]);
				auto redTile = coverToScreenCoord(rightPoint[i]);
				role->setPosition(redTile);
				DataManager::getInstance()->heroVector.push_back(role);
			}
		}
		else if (geocenter.size() > 0 && sky.size() > 0)
		{
			for (unsigned int i = 0; i < geocenter.size(); ++i)
			{
				auto role1 = rolePropty[geocenter[i] - RoleStarFlag];
				auto role = Hero::createRole(role1.sR_RoleType, role1.sR_RolePossess);
				role->setRoleHDA(role1.sR_Attack, role1.sR_HP, role1.sR_Agility, role1.sR_ChinaName);
				role->setSkinColor(3);
				tmxMap->addChild(role, 10);
				role->setRoleID(role1.sR_ID);

				role->setRoleDirAndSkill(S_Standby, D_Up);
				role->setRoleCoord(leftPoint[i]);
				auto redTile = coverToScreenCoord(leftPoint[i]);
				role->setPosition(redTile);
				DataManager::getInstance()->heroVector.push_back(role);
			}

			for (unsigned int i = 0; i < sky.size(); ++i)
			{
				auto role1 = rolePropty[sky[i] - RoleStarFlag];
				auto role = Hero::createRole(role1.sR_RoleType, role1.sR_RolePossess);
				role->setRoleHDA(role1.sR_Attack, role1.sR_HP, role1.sR_Agility, role1.sR_ChinaName);
				role->setSkinColor(0);
				tmxMap->addChild(role, 10);
				role->setRoleID(role1.sR_ID);

				role->setRoleDirAndSkill(S_Standby, D_Down);
				role->setRoleCoord(rightPoint[i]);
				auto redTile = coverToScreenCoord(rightPoint[i]);
				role->setPosition(redTile);
				DataManager::getInstance()->heroVector.push_back(role);
			}
		}
		else if (geocenter.size() > 0 && mainLand.size() > 0)
		{
			for (unsigned int i = 0; i < geocenter.size(); ++i)
			{
				auto role1 = rolePropty[geocenter[i] - RoleStarFlag];
				auto role = Hero::createRole(role1.sR_RoleType, role1.sR_RolePossess);
				role->setRoleHDA(role1.sR_Attack, role1.sR_HP, role1.sR_Agility, role1.sR_ChinaName);
				role->setSkinColor(3);
				tmxMap->addChild(role, 10);
				role->setRoleID(role1.sR_ID);

				role->setRoleDirAndSkill(S_Standby, D_Up);
				role->setRoleCoord(leftPoint[i]);
				auto redTile = coverToScreenCoord(leftPoint[i]);
				role->setPosition(redTile);
				DataManager::getInstance()->heroVector.push_back(role);
			}

			for (unsigned int i = 0; i < mainLand.size(); ++i)
			{
				auto role1 = rolePropty[mainLand[i] - RoleStarFlag];
				auto role = Hero::createRole(role1.sR_RoleType, role1.sR_RolePossess);
				role->setRoleHDA(role1.sR_Attack, role1.sR_HP, role1.sR_Agility, role1.sR_ChinaName);
				role->setSkinColor(0);
				tmxMap->addChild(role, 10);
				role->setRoleID(role1.sR_ID);

				role->setRoleDirAndSkill(S_Standby, D_Down);
				role->setRoleCoord(rightPoint[i]);
				auto redTile = coverToScreenCoord(rightPoint[i]);
				role->setPosition(redTile);
				DataManager::getInstance()->heroVector.push_back(role);
			}
		}
	}

	showRolePropty(false);
}

void GameLayer::initBottom()
{
	// 结束回合
	auto spriteNor = Sprite::create("MainFight/FinishNor.png");
	auto spriteClick = Sprite::create("MainFight/FinishSelected.png");
	spriteClick->setScale(0.9f);
	spriteClick->setPosition(Vec2(spriteNor->getPosition().x + spriteNor->getBoundingBox().size.width / 2 - spriteClick->getBoundingBox().size.width / 2,
		spriteNor->getPosition().y + spriteNor->getBoundingBox().size.height / 2 - spriteClick->getBoundingBox().size.height / 2));

	auto startButton = MenuItemSprite::create(spriteNor, spriteClick, CC_CALLBACK_1(GameLayer::passCurRole, this));
	startButton->setPosition(Vec2(SCREEN_WIDTH_HALF, 40));
	startButton->setTag(PassBtnFlag);

	// 投降
	auto surrenderNor = Sprite::create("MainFight/Surrender.png");
	auto surrenderSelect = Sprite::create("MainFight/Surrender.png");
	surrenderSelect->setScale(0.8f);
	surrenderSelect->setPosition(Vec2(surrenderNor->getPosition().x + surrenderNor->getBoundingBox().size.width / 2
		- surrenderSelect->getBoundingBox().size.width / 2,
		surrenderNor->getPosition().y + surrenderNor->getBoundingBox().size.height / 2 - surrenderSelect->getBoundingBox().size.height / 2));
	auto surrenderItem = MenuItemSprite::create(surrenderNor, surrenderSelect, CC_CALLBACK_1(GameLayer::surrenderClick, this));
	surrenderItem->setPosition(Vec2(surrenderNor->getContentSize().width, SCREEN_HEIGTH - surrenderNor->getContentSize().height));

	// 下拉菜单
	auto pullDownNor = Sprite::create("MainFight/PullDown.png");
	auto pullDownSelect = Sprite::create("MainFight/PullDown.png");
	pullDownSelect->setScale(0.8f);
	pullDownSelect->setPosition(Vec2(pullDownNor->getPosition().x + pullDownNor->getBoundingBox().size.width / 2
		- pullDownSelect->getBoundingBox().size.width / 2,
		pullDownNor->getPosition().y + pullDownNor->getBoundingBox().size.height / 2 - pullDownSelect->getBoundingBox().size.height / 2));
	auto pullDownItem = MenuItemSprite::create(pullDownNor, pullDownSelect, CC_CALLBACK_1(GameLayer::pullDownClick, this));
	pullDownItem->setPosition(Vec2(SCREEN_WIDTH - pullDownNor->getContentSize().width, SCREEN_HEIGTH - pullDownNor->getContentSize().height));

	auto pMenu = Menu::create(startButton, surrenderItem, pullDownItem, NULL);
	pMenu->setPosition(Vec2::ZERO);
	pMenu->setTag(PassBtnFlag);
	this->addChild(pMenu, UILayer);

	auto spriteBG = Sprite::create("MainFight/ProgressBG.png");
	spriteBG->setPosition(Point(540, 540 - 10));
	this->addChild(spriteBG, UILayer);

	//auto spriteProgress = Scale9Sprite::create("MainFight/Progress.png");
	auto spriteProgress = Sprite::create("MainFight/Progress.png");
	auto progressBar = ProgressTimer::create((Sprite*)spriteProgress);
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Point::ANCHOR_BOTTOM_LEFT);
	progressBar->setBarChangeRate(Point::ANCHOR_BOTTOM_RIGHT);
	progressBar->setPosition(Point(538, 542 - 10));
	progressBar->setPercentage(100);
	this->addChild(progressBar, UILayer, ProgressFlag);

	//this->schedule(schedule_selector(GameLayer::updateProgress), 0.1f);
}

void GameLayer::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(GameLayer::onTouchesCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, bgSprite);

	auto customEvent = EventListenerCustom::create(Event_String, CC_CALLBACK_1(GameLayer::customEventCall, this));
	_eventDispatcher->addEventListenerWithFixedPriority(customEvent, 1);
}

void GameLayer::onExit()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	Layer::onExit();
}

void GameLayer::customEventCall(EventCustom* _event)
{
	int callString = (uintptr_t)_event->getUserData();

// 	char* buf = static_cast<char*>(event->getUserData());
// 	str += buf;
// 	str += " times";
// 	statusLabel->setString(str.c_str());
	log("onCustomEvent data = %d", callString);
	if (callString == ARROW_HRUT_FLAG)
	{
		auto sequence = Sequence::create(CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ArrowAttackRole)), NULL);
		this->runAction(sequence);
	}
	else if (callString == ARROW_COUNTER_FLAG)
	{
		auto sequence = Sequence::create(CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ArrowCounterRoleA)), NULL);
		this->runAction(sequence);
	}
	else if (callString == IS_PASSTOUCH_FLAG)
	{
		isPassTouche(true, true);
	}
}

void GameLayer::update(float delta)
{
	if (GameMgr()->getGameFightType() == GFT_WiFi)
	{
// 		ProtMoveAttack prot = { 0 };
// 		prot.head.protoId = PROT_MOVE_ATTACK;
// 		prot.tileCoord = m_ServersCoord;
// 		prot.times = delta;
// 		NK()->send(&prot, sizeof(prot));
	}
}

void GameLayer::exitCallBack(Ref* pSender)
{
	NK()->stop();
	Director::getInstance()->replaceScene(StarLayer::scene());
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (touches.size() > 1 || !isTouchScreen)
	{
		return;
	}

	if ( GameMgr()->getGameFightType() == GFT_WiFi )
	{
		if ((GameMgr()->getIsServer() && GameMgr()->getIsServerBout()) || ((!GameMgr()->getIsServer() && !GameMgr()->getIsServerBout())))
		{
			auto itor = touches.begin();
			auto pTouch = (Touch*)(*itor);
			auto touchPos = pTouch->getLocation();

			autoFindPath(touchPos, false);
		}
		else
		{
			return;
		}
	}
	else
	{
		auto itor = touches.begin();
		auto pTouch = (Touch*)(*itor);
		auto touchPos = pTouch->getLocation();

		autoFindPath(touchPos, false);
	}
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if ( isSelectCurRole )
	{
		return;
	}
	auto winSize = Director::getInstance()->getWinSize();
	if (touches.size() > 1)
	{
		auto point1 = touches[0]->getLocation();
		auto point2 = touches[1]->getLocation();

		auto currDistance = point1.distance(point2);
		auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());

		auto pointVec1 = point1 - bgOrigin;
		auto pointVec2 = point2 - bgOrigin;

		auto relMidx = (pointVec1.x + pointVec2.x) / 2;
		auto relMidy = (pointVec1.y + pointVec2.y) / 2;

		auto anchorX = relMidx / bgSprite->getBoundingBox().size.width;
		auto anchorY = relMidy / bgSprite->getBoundingBox().size.height;

		auto absMidx = (point2.x + point1.x) / 2;
		auto absMidy = (point2.y + point1.y) / 2;

		if (bgOrigin.x > 0)
		{
			absMidx -= bgOrigin.x;
			bgOrigin.x = 0;
		}
		if (bgOrigin.x < -bgSprite->getBoundingBox().size.width + winSize.width)
		{
			absMidx += -bgSprite->getBoundingBox().size.width + winSize.width - bgOrigin.x;
			bgOrigin.x = -bgSprite->getBoundingBox().size.width + winSize.width;
		}
		if (bgOrigin.y > 0)
		{
			absMidy -= bgOrigin.y;
			bgOrigin.y = 0;
		}
		if (bgOrigin.y < -bgSprite->getBoundingBox().size.height + winSize.height)
		{
			absMidy += -bgSprite->getBoundingBox().size.height + winSize.height - bgOrigin.y;
			bgOrigin.y = -bgSprite->getBoundingBox().size.height + winSize.height;
		}

		bgSprite->setAnchorPoint(Vec2(anchorX, anchorY));
		bgSprite->setPosition(Vec2(absMidx, absMidy));

		auto scale = bgSprite->getScale() * (currDistance / prevDistance);
		scale = MIN(TXM_MAX_SCALE, MAX(TXM_MIN_SCALE, scale));
		bgSprite->setScale(scale);

		bgOrigin = Vec2(absMidx, absMidy) - Vec2(bgSprite->getBoundingBox().size.width * anchorX, 
			bgSprite->getBoundingBox().size.height * anchorY);
	}
	else if (touches.size() == 1)
	{
		auto touch = touches[0];
		auto diff = touch->getDelta();
		auto currentPos = bgSprite->getPosition();
		auto pos = currentPos + diff;
		auto bgSpriteCurrSize = bgSprite->getBoundingBox().size;

		pos.x = MIN(pos.x, bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
		pos.x = MAX(pos.x, -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);

		pos.y = MIN(pos.y, bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
		pos.y = MAX(pos.y, -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);

		bgSprite->setPosition(pos);
		Vec2 off = pos - currentPos;
		bgOrigin += off;
	}
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	if ( pathRole != hero )
	{
		removePath();
	}
}

void GameLayer::autoFindPath(Vec2 position, bool isCall)
{
	auto mapSize = tmxMap->getMapSize();
	auto tilePos = Vec2::ZERO;
	if (isCall)
	{
		tilePos = position;
	}
	else
	{
		Vec2 tempPoint;
		tempPoint.x = (position.x - bgOrigin.x) / bgSprite->getScale();
		tempPoint.y = (position.y - bgOrigin.y) / bgSprite->getScale();
		tilePos = coverToTileCoord(tempPoint);
	}
	if (tilePos == Vec2(-1, -1))
	{
		return;
	}
	if (tilePos.x >= 0 && tilePos.x <= mapSize.width - 1 && tilePos.y >= 0 && tilePos.y <= mapSize.height - 1)
	{
		if (hero && _chessRule->judgeRule(hero, hero->getRoleCoord(), tilePos) && isSelectCurRole )
		{
			if ( GameMgr()->getGameFightType() == GFT_WiFi )
			{
				if (!isCall)
				{
					if (GameMgr()->getIsServer())
					{
						if (!GameMgr()->getIsServerBout())
						{
							return;
						}
					}
					else
					{
						if (GameMgr()->getIsServerBout())
						{
							return;
						}
					}

					ProtMoveAttack prot = { 0 };
					prot.head.protoId = PROT_MOVE_ATTACK;
					prot.tileCoord = tilePos;
					NK()->send(&prot, sizeof(prot));
					isCall = false;
				}
			}		

			removePath();
			roleRemoveFlag();
			if (hero->getRoleState() == S_Standby)
			{
				_spOpenSteps.clear();
				_spClosedSteps.clear();

				this->insertOpenPath(aStartInstance->getInstance(hero->getRoleCoord()));

				do
				{
					AStartPath* currentStep = _spOpenSteps.at(0);
					_spClosedSteps.pushBack(currentStep);
					_spOpenSteps.erase(0);
					if (currentStep->getPosition() == tilePos)
					{
						this->constructPathAndStartAnimationFromStep(currentStep, _chessRule->isEatChess());

						_spOpenSteps.clear();
						_spClosedSteps.clear();
						break;
					}

					auto adjSteps = aStartInstance->walkAbleCoord(currentStep->getPosition(),
						hero->getRoleType(), tilePos, hero->isIngnoreCollide());
					for (ssize_t i = 0; i < adjSteps->count(); ++i)
					{
						auto step = AStartPath::getInstance(adjSteps->getControlPointAtIndex(i));

						if (aStartInstance->getStepIndex(_spOpenSteps, step) != -1)
						{
							continue;
						}
						int moveCost = aStartInstance->calcGScore(currentStep, step);
						ssize_t index = aStartInstance->getStepIndex(_spOpenSteps, step);

						if (index == -1)
						{
							step->setParent(currentStep);
							step->setGScore(currentStep->getGScore() + moveCost);
							step->setHScore(aStartInstance->calcHScore(step->getPosition(), tilePos));
							this->insertOpenPath(step);
						}
						else
						{
							step = _spOpenSteps.at(index);
							if ((currentStep->getGScore() + moveCost) < step->getGScore())
							{
								step->setGScore(currentStep->getGScore() + moveCost);
								step->retain();
								_spOpenSteps.erase(index);
								this->insertOpenPath(step);
								step->release();
							}
						}
					}
				} while (_spOpenSteps.size() > 0);
			}
		}
		else
		{
			isSelectCurRole = false;

			auto roleVector = DataManager::getInstance()->getRoleInfo();
			if ( !isCall )
			{
				removePath();
			}
			
			for (auto iter : roleVector)
			{
				auto tempRole = (Hero*)iter;
				if (tempRole->getRoleCoord() == tilePos)
				{
					pathRole = tempRole;
					if ( pathRole == hero )
					{
						isSelectCurRole = true;
					}
					else
					{
						isSelectCurRole = false;
					}

					if ( GameMgr()->getGameFightType() == GFT_WiFi )
					{
						if ( !isCall )
						{
							showRolePropty(true);
							focusOnRole(pathRole);

							switch (pathRole->getRoleType())
							{
							case RoleArrow:
								showArrowPath(pathRole->getRoleCoord(), pathRole);
								break;
							case RoleAssassin:
								showAssassinPath(pathRole->getRoleCoord(), pathRole);
								break;
							case RoleKing:
								showKingPath(pathRole->getRoleCoord(), pathRole);
								break;
							case RoleKrazySoldier:
								showKrazySoldierPath(pathRole->getRoleCoord(), pathRole);
								break;
							case RoleSoldier:
								showSoldierPath(pathRole->getRoleCoord(), pathRole);
								break;
							}
							if ( !isCall )
							{
								ProtMoveAttack prot = { 0 };
								prot.head.protoId = PROT_MOVE_ATTACK;
								prot.tileCoord = tilePos;
								NK()->send(&prot, sizeof(prot));
								this->scheduleUpdate();
							}
							
						}
					}
					else if ( GameMgr()->getGameFightType() == GFT_AI )
					{
						showRolePropty(true);
						focusOnRole(pathRole);

						switch (pathRole->getRoleType())
						{
						case RoleArrow:
							showArrowPath(pathRole->getRoleCoord(), pathRole);
							break;
						case RoleAssassin:
							showAssassinPath(pathRole->getRoleCoord(), pathRole);
							break;
						case RoleKing:
							showKingPath(pathRole->getRoleCoord(), pathRole);
							break;
						case RoleKrazySoldier:
							showKrazySoldierPath(pathRole->getRoleCoord(), pathRole);
							break;
						case RoleSoldier:
							showSoldierPath(pathRole->getRoleCoord(), pathRole);
							break;
						}
					}
				}
			}
		}
	}
}

void GameLayer::showKingPath(Vec2 originalPoint, Hero* _heroInfo)
{
	tipsCount = TipsFlag;
	auto tileWidth = tmxMap->getMapSize().width - 1;
	auto tileHeight = tmxMap->getMapSize().height - 1;

	auto roleInfo = DataManager::getInstance()->getRoleInfo();

	for (unsigned int i = 0; i < roleInfo.size(); i++)
	{
		auto info = (Hero*)roleInfo[i];
		if (info->getRoleCoord() == _heroInfo->getRoleCoord())
		{
			roleInfo.erase(roleInfo.begin() + i);
		}
	}

	int pathXY[8][2] =
	{
		{  0, -1 }, // 1
		{  1, -1 }, // 2
		{  1,  0 }, // 3
		{  1,  1 }, // 4
		{  0,  1 }, // 5
		{ -1,  1 }, // 6
		{ -1,  0 }, // 7
		{ -1, -1 }, // 8
	};

	bool isPath[8] = {false};

    for ( int i = 0; i < 8; i ++ )
    {
        if ( ( originalPoint.x + pathXY[ i ][ 0 ] >= 0 ) && ( originalPoint.x + pathXY[ i ][ 0 ] <= tileWidth )
            && ( originalPoint.y + pathXY[ i ][ 1 ] >= 0 ) && ( originalPoint.y + pathXY[ i ][ 1 ] <= tileHeight ) )
        {
            for ( unsigned int z = 0; z < roleInfo.size(); z ++ )
            {
                auto info = (Hero*)roleInfo[ z ];
                if ( Vec2(originalPoint + Vec2( pathXY[ i ][ 0 ], pathXY[ i ][ 1 ] )) == info->getRoleCoord() )
                {
					if (info->getSkinColor() != _heroInfo->getSkinColor())
					{						
						auto sprite = Sprite::create("PreUI/Select.png");
						sprite->setColor(Color3B::RED);
						auto scalt = ScaleTo::create(0.5f, 0.7f);
						auto scalt2 = ScaleTo::create(0.5f, 1.0f);
						sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
						auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i][0], originalPoint.y + pathXY[i][1]));
						sprite->setPosition(ss - offPoint);
						sprite->setTag(tipsCount++);
						tmxMap->addChild(sprite, TipsRolePath);
					}
					isPath[i] = true;
					continue;
                }
            }
        }
    }

	for (int i = 0; i < 8; i ++)
	{
		if ( !isPath[ i ] )
		{
			if ((originalPoint.x + pathXY[i][0] >= 0) && (originalPoint.x + pathXY[i][0] <= tileWidth)
				&& (originalPoint.y + pathXY[i][1] >= 0) && (originalPoint.y + pathXY[i][1] <= tileHeight))
			{
				auto sprite = Sprite::create("PreUI/UI_Path.png");
				auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i][0], originalPoint.y + pathXY[i][1]));
				sprite->setPosition(ss - offPoint);
				sprite->setTag(tipsCount++);
				tmxMap->addChild(sprite, TipsRolePath);
			}			
		}
	}
}

void GameLayer::showAssassinPath(Vec2 originalPoint, Hero* _heroInfo)
{
	tipsCount = TipsFlag;
    auto tileWidth = tmxMap->getMapSize().width - 1;
    auto tileHeight = tmxMap->getMapSize().height - 1;
    
    auto roleInfo = DataManager::getInstance()->getRoleInfo();
    // 移除自身的坐标
    for ( unsigned int i = 0; i < roleInfo.size(); i ++ )
    {
        auto info = (Hero*)roleInfo[ i ];
		if (info->getRoleCoord() == _heroInfo->getRoleCoord())
        {
            roleInfo.erase( roleInfo.begin() + i );
        }
    }
    
    int pathXY[ 8 ][ 2 ] =
    {
        {  1, -2 }, {  2, -1 },
        {  2,  1 }, {  1,  2 },
        { -1,  2 }, { -2,  1 },
        { -2, -1 }, { -1, -2 },
    };
    
	bool isPath[8] = {false};

    for ( int i = 0; i < 8; i ++ )
    {
        if ( ( originalPoint.x + pathXY[ i ][ 0 ] >= 0 ) && ( originalPoint.x + pathXY[ i ][ 0 ] <= tileWidth )
            && ( originalPoint.y + pathXY[ i ][ 1 ] >= 0 ) && ( originalPoint.y + pathXY[ i ][ 1 ] <= tileHeight ) )
        {
            for ( unsigned int z = 0; z < roleInfo.size(); z ++ )
            {
                auto info = (Hero*)roleInfo[ z ];
                if ( Vec2(originalPoint + Vec2( pathXY[ i ][ 0 ], pathXY[ i ][ 1 ] )) == info->getRoleCoord() )
                {
					if (info->getSkinColor() != _heroInfo->getSkinColor())
					{
						auto sprite = Sprite::create("PreUI/Select.png");
						sprite->setColor(Color3B::RED);
						auto scalt = ScaleTo::create(0.5f, 0.7f);
						auto scalt2 = ScaleTo::create(0.5f, 1.0f);
						sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
						auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i][0], originalPoint.y + pathXY[i][1]));
						sprite->setPosition(ss - offPoint);
						sprite->setTag(tipsCount++);
						tmxMap->addChild(sprite, TipsRolePath);
					}
					isPath[ i ] = true;
                    break;
                }
            }
        }
    }

	for (int i = 0; i < 8; i ++)
	{
		if ( !isPath[ i ] )
		{
			if ((originalPoint.x + pathXY[i][0] >= 0) && (originalPoint.x + pathXY[i][0] <= tileWidth)
				&& (originalPoint.y + pathXY[i][1] >= 0) && (originalPoint.y + pathXY[i][1] <= tileHeight))
			{
				auto sprite = Sprite::create("PreUI/UI_Path.png");
				auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i][0], originalPoint.y + pathXY[i][1]));
				sprite->setPosition(ss - offPoint);
				sprite->setTag(tipsCount++);
				tmxMap->addChild(sprite, TipsRolePath);
			}		
		}
	}
}

void GameLayer::showSoldierPath(Vec2 originalPoint, Hero* _heroInfo)
{
	tipsCount = TipsFlag;
    auto tileWidth = tmxMap->getMapSize().width - 1;
    auto tileHeight = tmxMap->getMapSize().height - 1;
    
    auto roleInfo = DataManager::getInstance()->getRoleInfo();
    // 移除自身的坐标
    for ( unsigned int i = 0; i < roleInfo.size(); i ++ )
    {
        auto info = (Hero*)roleInfo[ i ];
		if (info->getRoleCoord() == _heroInfo->getRoleCoord())
        {
            roleInfo.erase( roleInfo.begin() + i );
        }
    }

	int pathN[9][2] =
	{
		{ -1, -1 }, // 8
		{ 0, -1 },
		{ 1, -1 }, // 2
		{ 1, 0 },
		{ 1, 1 }, // 4
		{ 0, 1 }, // 5
		{ -1, 1 }, // 6
		{ -1, 0 },
		{ -1, -1 }, // 8
	};

	int pathF[4][2] =
	{
		{ 0, -2 }, // 1
		{ 2, 0 }, // 3
		{ 0, 2 }, // 5
		{ -2, 0 }, // 7
	};
    
	bool isPathN[9] = { false };
	bool isPathF[4] = { false };

	for (int i = 0; i < 9; ++ i)
	{
		if ((originalPoint.x + pathN[i][0] >= 0) && (originalPoint.x + pathN[i][0] <= tileWidth)
			&& (originalPoint.y + pathN[i][1] >= 0) && (originalPoint.y + pathN[i][1] <= tileHeight))
		{
			for ( auto itor : roleInfo )
			{
				if ( Vec2(originalPoint + Vec2(pathN[i][0], pathN[i][1])) == itor->getRoleCoord())
				{
					if (itor->getSkinColor() != _heroInfo->getSkinColor())
					{
						auto sprite = Sprite::create("PreUI/Select.png");
						sprite->setColor(Color3B::RED);
						auto scalt = ScaleTo::create(0.5f, 0.7f);
						auto scalt2 = ScaleTo::create(0.5f, 1.0f);
						sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
						auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathN[i][0], originalPoint.y + pathN[i][1]));
						sprite->setPosition(ss - offPoint);
						sprite->setTag(tipsCount++);
						tmxMap->addChild(sprite, TipsRolePath);
					}
					isPathN[i] = true;
				}
			}
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if ((originalPoint.x + pathF[i][0] >= 0) && (originalPoint.x + pathF[i][0] <= tileWidth)
			&& (originalPoint.y + pathF[i][1] >= 0) && (originalPoint.y + pathF[i][1] <= tileHeight))
		{
			for (auto itor : roleInfo)
			{
				if (Vec2(originalPoint + Vec2(pathF[i][0], pathF[i][1])) == itor->getRoleCoord())
				{
					if (i == 0)
					{
						if ((originalPoint.x == 0 && originalPoint.y == tileHeight)
							|| (originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if ((originalPoint.x == tileWidth && originalPoint.y == tileWidth)
							|| (originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if (
							(originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight)
							|| (originalPoint.x > 0 && originalPoint.y > 0 && originalPoint.x < tileWidth && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
					}
					else if (i == 1)
					{
						if ((originalPoint.x == 0 && originalPoint.y == 0)
							|| (originalPoint.y == 0 && originalPoint.x > 0 && originalPoint.x < tileHeight))
						{
							if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if ((originalPoint.x == 0 && originalPoint.y == tileHeight)
							|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if (
							(originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight)
							|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
					}
					else if (i == 2)
					{
						if ((originalPoint.x == 0 && originalPoint.y == 0)
							|| (originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if ((originalPoint.x == tileWidth && originalPoint.y == 0)
							|| (originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if ((originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == 0)
							|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
					}
					else if (i == 3)
					{
						if ((originalPoint.x == tileWidth && originalPoint.y == 0)
							|| (originalPoint.y == 0 && originalPoint.x > 0 && originalPoint.x < tileWidth))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if ((originalPoint.x == tileWidth && originalPoint.y == tileHeight)
							|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight))
						{
							if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
						else if ((originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight)
							|| (originalPoint.x > 0 && originalPoint.y > 0 && originalPoint.x < tileWidth && originalPoint.y < tileHeight))
						{
							if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
							{
								if (itor->getSkinColor() != _heroInfo->getSkinColor())
								{
									auto sprite = Sprite::create("PreUI/Select.png");
									sprite->setColor(Color3B::RED);
									auto scalt = ScaleTo::create(0.5f, 0.7f);
									auto scalt2 = ScaleTo::create(0.5f, 1.0f);
									sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
									auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
									sprite->setPosition(ss - offPoint);
									sprite->setTag(tipsCount++);
									tmxMap->addChild(sprite, TipsRolePath);
								}
								isPathF[i] = true;
							}
						}
					}
				}
			}
		}		
	}

	for (int i = 0; i < 8; i++)
	{
		if ((originalPoint.x + pathN[i][0] >= 0) && (originalPoint.x + pathN[i][0] <= tileWidth)
			&& (originalPoint.y + pathN[i][1] >= 0) && (originalPoint.y + pathN[i][1] <= tileHeight))
		{
			if (!isPathN[i])
			{
				auto sprite = Sprite::create("PreUI/UI_Path.png");
				auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathN[i][0], originalPoint.y + pathN[i][1]));
				sprite->setPosition(ss - offPoint);
				sprite->setTag(tipsCount++);
				tmxMap->addChild(sprite, TipsRolePath);
			}
		}
	}
  

	for (int i = 0; i < 4; i++)
	{
		if ((originalPoint.x + pathF[i][0] >= 0) && (originalPoint.x + pathF[i][0] <= tileWidth)
			&& (originalPoint.y + pathF[i][1] >= 0) && (originalPoint.y + pathF[i][1] <= tileHeight))
		{
			if (!isPathF[i])
			{
				if (i == 0)
				{
					if ((originalPoint.x == 0 && originalPoint.y == tileHeight) 
						||(originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight))
					{
						if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if ((originalPoint.x == tileWidth && originalPoint.y == tileWidth)
						|| (originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight) )
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if (
						(originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight)
						|| (originalPoint.x > 0 && originalPoint.y > 0 && originalPoint.x < tileWidth && originalPoint.y < tileHeight))
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
				}
				else if (i == 1)
				{
					if ((originalPoint.x == 0 && originalPoint.y == 0) 
						|| (originalPoint.y == 0 && originalPoint.x > 0 && originalPoint.x < tileHeight) )
					{
						if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if ((originalPoint.x == 0 && originalPoint.y == tileHeight)
						|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight) )
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if (
						(originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight)
						|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
				}
				else if (i == 2)
				{
					if ((originalPoint.x == 0 && originalPoint.y == 0) 
						|| (originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight))
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if ((originalPoint.x == tileWidth && originalPoint.y == 0) 
						|| (originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight) )
					{
						if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if ((originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == 0)
						|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
				}
				else if (i == 3)
				{
					if ((originalPoint.x == tileWidth && originalPoint.y == 0) 
						|| (originalPoint.y == 0 && originalPoint.x > 0 && originalPoint.x < tileWidth))
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if ((originalPoint.x == tileWidth && originalPoint.y == tileHeight) 
						|| ( originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight ))
					{
						if (!isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
					else if ((originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight)
						|| (originalPoint.x > 0 && originalPoint.y > 0 && originalPoint.x < tileWidth && originalPoint.y < tileHeight))
					{
						if (!isPathN[i * 2] || !isPathN[i * 2 + 1] || !isPathN[i * 2 + 2])
						{
							auto sprite = Sprite::create("PreUI/UI_Path.png");
							auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathF[i][0], originalPoint.y + pathF[i][1]));
							sprite->setPosition(ss - offPoint);
							sprite->setTag(tipsCount++);
							tmxMap->addChild(sprite, TipsRolePath);
						}
					}
				}
			}
		}
	}
}

void GameLayer::showArrowPath(Vec2 originalPoint, Hero* _heroInfo)
{
	tipsCount = TipsFlag;
    auto tileWidth = tmxMap->getMapSize().width - 1;
    auto tileHeight = tmxMap->getMapSize().height - 1;
    
    auto roleInfo = DataManager::getInstance()->getRoleInfo();
    // 移除自身的坐标
    for ( unsigned int i = 0; i < roleInfo.size(); i ++ )
    {
        auto info = (Hero*)roleInfo[ i ];
        if ( info->getRoleCoord() == hero->getRoleCoord() )
        {
            roleInfo.erase( roleInfo.begin() + i );
        }
    }
    
    int pathXY[ 16 ][ 2 ] =
    {
        { 0, -1 }, { 0, -2 }, { 0, -3 }, { 0, -4 }, // 1
        { 1,  0 }, { 2,  0 }, { 3,  0 }, { 4,  0 }, // 3
        { 0,  1 }, { 0,  2 }, { 0,  3 }, { 0,  4 }, // 5
        {-1,  0 }, {-2,  0 }, {-3,  0 }, {-4,  0 }, // 7
        
    };
    
	bool isPath[16] = {false};

    for ( int i = 0; i < 4; i ++ )
    {
        for ( int k = 0; k < 4; k ++ )
        {
            if ( ( originalPoint.x + pathXY[ i * 4 + k ][ 0 ] >= 0 ) && ( originalPoint.x + pathXY[ i * 4 + k ][ 0 ] <= tileWidth )
                && ( originalPoint.y + pathXY[ i * 4 + k ][ 1 ] >= 0 ) && ( originalPoint.y + pathXY[ i * 4 + k ][ 1 ] <= tileHeight ) )
            {
                for ( unsigned int z = 0; z < roleInfo.size(); z ++ )
                {
                    auto info = (Hero* )roleInfo[ z ];
                    if ( k == 0 )
                    {
                        if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor())
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
                            continue;
                        }
                    }
                    else if ( k == 1 )
                    {
						if (Vec2(originalPoint + Vec2(pathXY[i * 4 + k - 1][0], pathXY[i * 4 + k - 1][1])) == info->getRoleCoord())
                        {
							isPath[ i * 4 + k ] = true;
							continue;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor()
								&& !isPath[i * 4 + k - 1])
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
							continue;
                        }
                    }
                    else if ( k == 2 )
                    {
						if (Vec2(originalPoint + Vec2(pathXY[i * 4 + k - 2][0], pathXY[i * 4 + k - 2][1])) == info->getRoleCoord())
                        {
							isPath[i * 4 + k] = true;
							continue;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 1][ 0 ], pathXY[i * 4 + k - 1][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
							continue;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor()
								&& !isPath[i * 4 + k] && !isPath[i * 4 + k - 1] )
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
							continue;
                        }
                    }
                    else if ( k == 3 )
                    {
                        if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 3][ 0 ], pathXY[i * 4 + k - 3][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
							continue;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 2][ 0 ], pathXY[i * 4 + k - 2][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
							continue;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 1][ 0 ], pathXY[i * 4 + k - 1][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
							continue;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor()
								&& !isPath[i * 4 + k - 1] && !isPath[i * 4 + k - 2] && !isPath[i * 4 + k] )
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));

								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
							continue;
                        }
                    }
                }
            }
        }
    }

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			if ((originalPoint.x + pathXY[i * 4 + k][0] >= 0) && (originalPoint.x + pathXY[i * 4 + k][0] <= tileWidth)
				&& (originalPoint.y + pathXY[i * 4 + k][1] >= 0) && (originalPoint.y + pathXY[i * 4 + k][1] <= tileHeight))
			{
				if (!isPath[ i * 4 + k ])
				{
					auto sprite = Sprite::create("PreUI/UI_Path.png");
					auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
					sprite->setPosition(ss - offPoint);
					sprite->setTag(tipsCount++);
					tmxMap->addChild(sprite, TipsRolePath);
				}
			}
		}
	}
}

void GameLayer::showKrazySoldierPath(Vec2 originalPoint, Hero* _heroInfo)
{
	tipsCount = TipsFlag;
    auto tileWidth = tmxMap->getMapSize().width - 1;
    auto tileHeight = tmxMap->getMapSize().height - 1;
    
    auto roleInfo = DataManager::getInstance()->getRoleInfo();
    // 移除自身的坐标
    for ( unsigned int i = 0; i < roleInfo.size(); i ++ )
    {
        auto info = (Hero*)roleInfo[ i ];
		if (info->getRoleCoord() == _heroInfo->getRoleCoord())
        {
            roleInfo.erase( roleInfo.begin() + i );
        }
    }
    
    int pathXY[ 16 ][ 2 ] =
    {
        { 1, -1 }, { 2, -2 }, { 3, -3 }, { 4, -4 }, // 2
        { 1,  1 }, { 2,  2 }, { 3,  3 }, { 4,  4 }, // 4
        {-1,  1 }, {-2,  2 }, {-3,  3 }, {-4,  4 }, // 6
        {-1, -1 }, {-2, -2 }, {-3, -3 }, {-4, -4 }, // 8
    };
    
	bool isPath[16] = {false};
    for ( int i = 0; i < 4; i ++ )
    {
        for ( int k = 0; k < 4; k ++ )
        {
            if ( ( originalPoint.x + pathXY[ i * 4 + k ][ 0 ] >= 0 ) && ( originalPoint.x + pathXY[ i * 4 + k ][ 0 ] <= tileWidth )
                && ( originalPoint.y + pathXY[ i * 4 + k ][ 1 ] >= 0 ) && ( originalPoint.y + pathXY[ i * 4 + k ][ 1 ] <= tileHeight ) )
            {
                for ( unsigned int z = 0; z < roleInfo.size(); z ++ )
                {
                    auto info = (Hero* )roleInfo[ z ];
                    if ( k == 0 )
                    {
                        if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor())
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], 
									originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
                            break;
                        }
                    }
                    else if ( k == 1 )
                    {
                        if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 1][ 0 ], pathXY[i * 4 + k - 1][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
                            break;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor()
								&& !isPath[i * 4 + k] && !isPath[i * 4 + k - 1])
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
                            break;
                        }
                    }
                    else if ( k == 2 )
					{
						if (Vec2(originalPoint + Vec2(pathXY[i * 4 + k - 2][0], pathXY[i * 4 + k - 2][1])) == info->getRoleCoord())
						{
							isPath[i * 4 + k] = true;
                            break;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 1][ 0 ], pathXY[i * 4 + k - 1][ 1 ] ) ) == info->getRoleCoord() )
						{
							isPath[i * 4 + k] = true;
                            break;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor()
								&& !isPath[i * 4 + k - 1] && !isPath[i * 4 + k ])
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], 
									originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
                            break;
                        }
                    }
                    else if ( k == 3 )
                    {
                        if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 3][ 0 ], pathXY[i * 4 + k - 3][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
                            break;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 2][ 0 ], pathXY[i * 4 + k - 2][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
                            break;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k - 1][ 0 ], pathXY[i * 4 + k - 1][ 1 ] ) ) == info->getRoleCoord() )
                        {
							isPath[i * 4 + k] = true;
                            break;
                        }
                        else if ( Vec2(originalPoint + Vec2( pathXY[i * 4 + k][ 0 ], pathXY[i * 4 + k][ 1 ] ) ) == info->getRoleCoord() )
                        {
							if (info->getSkinColor() != _heroInfo->getSkinColor()
								&& !isPath[i * 4 + k - 1] && !isPath[i * 4 + k - 2] && !isPath[i * 4 + k])
							{
								auto sprite = Sprite::create("PreUI/Select.png");
								sprite->setColor(Color3B::RED);
								auto scalt = ScaleTo::create(0.5f, 0.7f);
								auto scalt2 = ScaleTo::create(0.5f, 1.0f);
								sprite->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
								auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], 
									originalPoint.y + pathXY[i * 4 + k][1]));
								sprite->setPosition(ss - offPoint);
								sprite->setTag(tipsCount++);
								tmxMap->addChild(sprite, TipsRolePath);
							}
							isPath[i * 4 + k] = true;
                            break;
                        }
                    }
                }
            }
        }
    }

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			if ((originalPoint.x + pathXY[i * 4 + k][0] >= 0) && (originalPoint.x + pathXY[i * 4 + k][0] <= tileWidth)
				&& (originalPoint.y + pathXY[i * 4 + k][1] >= 0) && (originalPoint.y + pathXY[i * 4 + k][1] <= tileHeight))
			{
				if ( !isPath[ i * 4 + k ] )
				{
					auto sprite = Sprite::create("PreUI/UI_Path.png");
					auto ss = coverToScreenCoord(Vec2(originalPoint.x + pathXY[i * 4 + k][0], originalPoint.y + pathXY[i * 4 + k][1]));
					sprite->setPosition(ss - offPoint);
					sprite->setTag(tipsCount++);
					tmxMap->addChild(sprite, TipsRolePath);
				}
			}
		}
	}
}

void GameLayer::removePath()
{	
    for ( int i = TipsFlag; i < tipsCount; i ++)
    {
        tmxMap->removeChildByTag(TipsFlag + i - TipsFlag );
    }
	showRolePropty(false);
}

void GameLayer::roleRemoveFlag()
{
	isSelectCurRole = false;
	hero->removeChildByTag(RedFlag);
}

void GameLayer::showRolePropty(bool isVisable)
{
	for ( auto itor : DataManager::getInstance()->getRoleInfo() )
	{
		auto role = (Hero*)itor;
		role->setLabelVisable(isVisable);
	}
}

void GameLayer::isPassTouche(bool isEnable, bool m_isTouchScreen)
{
	isTouchScreen = m_isTouchScreen;
	auto passButton = (Menu*)this->getChildByTag(PassBtnFlag);
	auto pass = (MenuItemSprite*)passButton->getChildByTag(PassBtnFlag);
	pass->setEnabled(isEnable);

	if (!isEnable)
	{
		auto disAble = Sprite::create("MainFight/AnTurn.png");
		pass->setDisabledImage(disAble);
	}
	if ( GameMgr()->getGameFightType() == GFT_AI )
	{
		auto disAble = Sprite::create("FinishClose.png");
		pass->setDisabledImage(disAble);
	}	
}

void GameLayer::updateProgress(float dt)
{
	auto progress = (ProgressTimer*)this->getChildByTag(ProgressFlag);
	auto curPercent = progress->getPercentage();

	int ind = curPercent / 100 * BoutTime - dt;
	if ( curPercent > 0 )
	{
		auto percent = curPercent / BoutTime * 100;
		progress->setPercentage(ind / BoutTime * 100);
	}
	else
	{
// 		passCurRole(NULL);
// 		progress->setPercentage(100);
	}
}

void GameLayer::focusOnRole(Hero* _hero)
{
	auto size = _hero->getReaction();
	auto point = _hero->getRoleCoord();
	for (auto iter : DataManager::getInstance()->getRoleInfo())
	{
		auto role = (Hero*)iter;
		for (int i = size; i >= -size; -- i)
		{
			for (int z = size; z >= -size; -- z)
			{
				if ( Vec2( point + Vec2(i, z)) == role->getRoleCoord() )
				{
					if ( role != pathRole )
					{
						//role->setRoleDirAndSkill(S_Standby, ToolFunc::calcRoleDirection(role->getRoleCoord(), point));
						roleSpin(role, ToolFunc::calcRoleDirection(role->getRoleCoord(), point));
						role->setRoleCoord(role->getRoleCoord());
					}
				}
			}
		}
	}
}

Vec2 GameLayer::coverToTileCoord(Vec2 _point)
{
	// 触摸的位置信息必须减去瓷砖地图的位置信息,因为地图的位置可能在滚动变化
	auto pos = _point - tmxMap->getPosition();
	float halfMapWidth = tmxMap->getMapSize().width * 0.5f;
	float mapHeight = tmxMap->getMapSize().height;
	float tileWidth = tmxMap->getTileSize().width;
	float tileHeight = tmxMap->getTileSize().height;
	auto tilePosDiv = Vec2(pos.x / tileWidth, pos.y / tileHeight);
	float inverseTileY = mapHeight - tilePosDiv.y;
	// 将得到的计算结果转换成 int,以确保得到的是整数
	float posX = (float)(inverseTileY + tilePosDiv.x - halfMapWidth);
	float posY = (float)(inverseTileY - tilePosDiv.x + halfMapWidth);
	if (posX < 0 || posY < 0)
	{
		return Vec2(-1, -1);
	}
	else
	{
		posX = (int)(inverseTileY + tilePosDiv.x - halfMapWidth);
		posY = (int)(inverseTileY - tilePosDiv.x + halfMapWidth);
	}
	// 确保坐标在地图的边界之内
	if (posX < 0 || posX > tmxMap->getMapSize().width - 1)
	{
		posX = -1;
	}
	if (posY < 0 || posY > tmxMap->getMapSize().height - 1)
	{
		posY = -1;
	}
	return Vec2(posX, posY);
}

Vec2 GameLayer::coverToScreenCoord(Vec2 _point)
{
	auto mapSize = tmxMap->getMapSize();
	auto tileSize = tmxMap->getTileSize();
	auto tileWidth = tmxMap->getBoundingBox().size.width / tmxMap->getMapSize().width;
	auto tileHeight = tmxMap->getBoundingBox().size.height / tmxMap->getMapSize().height;

	auto variable1 = (_point.x + mapSize.width / 2 - mapSize.height) * tileWidth * tileHeight;
	auto variable2 = (-_point.y + mapSize.width / 2 + mapSize.height) * tileWidth * tileHeight;

	int posx = (variable1 + variable2) / 2 / tileHeight;
	int posy = (variable2 - variable1) / 2 / tileWidth;

	return Point(posx, posy);
}

Vec2 GameLayer::calcNestPoint(Hero* control, Hero* hitted)
{
    auto offSetX = hitted->getRoleCoord().x - control->getRoleCoord().x;
    auto offSetY = hitted->getRoleCoord().y - control->getRoleCoord().y;
    
    Vec2 targetPoint;
    
    if ( offSetX == 0 && offSetY < 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( 0, 1 );
    }
    else if ( offSetX > 0 && offSetY < 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( -1, 1 );
    }
    else if ( offSetX > 0 && offSetY == 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( -1, 0 );
    }
    else if ( offSetX > 0 && offSetY > 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( -1, -1 );
    }
    else if ( offSetX == 0 && offSetY > 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( 0, -1 );
    }
    else if ( offSetX < 0 && offSetY > 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( 1, -1 );
    }
    else if ( offSetX < 0 && offSetY == 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( 1, 0 );
    }
    else if ( offSetX < 0 && offSetY < 0 )
    {
        targetPoint = hitted->getRoleCoord() + Vec2( 1, 1 );
    }
    return targetPoint;
}

void GameLayer::passCurRole(Ref* pSender)
{
	removePath();
	roleRemoveFlag();
	showCurRole(ComOrder);

	if ( pSender != NULL && GameMgr()->getGameFightType() == GFT_WiFi)
	{
		ProtHead prot = { 0 };
		prot.protoId = PROT_PASS_CLICK;
		NK()->send(&prot, sizeof(prot));
	}
}

void GameLayer::surrenderClick(Ref* pSender)
{
	NK()->stop();
	Director::getInstance()->replaceScene(StarLayer::scene());
}

void GameLayer::pullDownClick(Ref* pSender)
{
	log("PULL Donw");
}

void GameLayer::notCallBack(Ref* pSender)
{
	auto callString = (__String*)pSender;
	if ( callString->compare(AorrwNotCall.c_str()) == 0 )
	{
		auto sequence = Sequence::create(CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ArrowAttackRole)), NULL);
		this->runAction(sequence);
	}
	else if ( callString->compare(ArrowCounter.c_str()) == 0 )
	{
		auto sequence = Sequence::create(CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ArrowCounterRoleA)), NULL);
		this->runAction(sequence);
	}
	else if ( callString->compare(PassTouch.c_str()) == 0 )
	{
		if ( GameMgr()->getGameFightType() == GFT_WiFi )
		{
			if (GameMgr()->getIsServer())
			{
				if (hero->getSkinColor() == GameMgr()->getLocalColor())
				{
					isPassTouche(true, true);
				}
				else
				{
					isPassTouche(false, true);
				}
			}
			else
			{
				if (hero->getSkinColor() == GameMgr()->getRemoteColor())
				{
					isPassTouche(true, true);
				}
				else
				{
					isPassTouche(false, true);
				}
			}
		}
		else
		{
			isPassTouche(true, true);
		}		
	}
}

void GameLayer::sortByAgility()
{
    curVector.clear();
    curVector = DataManager::getInstance()->getRoleInfo();
    Hero* iTemp = nullptr;
    for ( unsigned int i = 1; i < curVector.size(); i ++ )
    {
        for ( unsigned int z = curVector.size() - 1; z >= i; z-- )
        {
            auto role1 = (Hero* )curVector[ z ];
            auto role2 = (Hero* )curVector[ z - 1 ];
			
            if ( role1->getRoleAgility() < role2->getRoleAgility() )
            {
                iTemp = curVector[ z - 1 ];
                curVector[ z - 1 ] = curVector[ z ];
                curVector[ z ] = iTemp;
            }
        }
    }
    
    vector<Hero* > tempHero;
    tempHero.clear();
    
    for(vector<Hero* >::reverse_iterator rit = curVector.rbegin();
        rit!=curVector.rend(); rit ++ )
    {
        auto role1 = (Hero* )(*rit);
        tempHero.push_back(role1);
    }
    curVector.clear();
    curVector = tempHero;


	if ( GameMgr()->getGameFightType() == GFT_WiFi && GameMgr()->getIsServer() )
	{
		string idStr;
		string colorStr;

		ProtAgality prot = { 0 };
		prot.head.protoId = PROT_SEND_TEAM;
		for ( auto itor : curVector )
		{
			auto idcstring = __String::createWithFormat("%d#", itor->getRoleID());
			idStr.append(idcstring->getCString());
			auto colorcstring = __String::createWithFormat("%d#", itor->getSkinColor());
			colorStr.append(colorcstring->getCString());
		}
		memset(prot.idStr, 0, sizeof(prot.idStr));
		memset(prot.colorStr, 0, sizeof(prot.colorStr));

		memcpy(prot.idStr, idStr.c_str(), idStr.length());
		memcpy(prot.colorStr, colorStr.c_str(), sizeof(prot.colorStr));
		NK()->send(&prot, sizeof(prot));
	}	
}

void GameLayer::showCurRole(OrderRole _orderRole)
{
	if (_orderRole == ComOrder && _orderRole != DeathOrder)
	{
		curVector.erase(curVector.begin());
		preVectot.push_back(hero);
	}

	if (curVector.size() <= 0)
	{
		curVector.clear();
		curVector = preVectot;
		preVectot.clear();
	}

	for (unsigned int i = 0; i < curVector.size(); i++)
	{
		auto role = (Hero*)curVector[i];
		if ( i == 0 )
		{
			hero = role;
			auto selectPng = Sprite::create("PreUI/Select.png");
			auto scalt = ScaleTo::create(0.5f, 0.7f);
			auto scalt2 = ScaleTo::create(0.5f, 1.0f);
			selectPng->runAction(RepeatForever::create(Sequence::create(scalt, scalt2, NULL)));
			selectPng->setTag(RedFlag);
			hero->addChild(selectPng, -1);
			selectPng->setPosition(5, -30);
		}
	}
	switch (_orderRole)
	{
	case ComOrder:
		isPassTouche(false, false);
		// Show Role Frame Photo
		m_RolePosMagr->showRoleOrder(curVector, preVectot, ComOrder);
		break;
	case DeathOrder:
		isPassTouche(false, false);
		m_RolePosMagr->showRoleOrder(curVector, preVectot, DeathOrder);
		break;
	}
	

	if ( _orderRole == FirstOrder )
	{
		if (GameMgr()->getGameFightType() == GFT_WiFi)
		{
			// 当前为服务端
			if (GameMgr()->getIsServer())
			{
				// 当前可行走角色颜色等于 远程角色颜色
				if (hero->getSkinColor() == GameMgr()->getLocalColor())
				{
					GameMgr()->setServerBout(true);
					isPassTouche(true, true);
				}
				else
				{
					GameMgr()->setServerBout(false);
					isPassTouche(false, true);
				}
			}
			else
			{
				if (hero->getSkinColor() == GameMgr()->getRemoteColor())
				{
					GameMgr()->setServerBout(false);
					isPassTouche(true, true);
				}
				else
				{
					GameMgr()->setServerBout(true);
					isPassTouche(false, true);
				}
			}
		}
	}
	// 是否为联机对战
	if ( GameMgr()->getGameFightType() == GFT_WiFi )
	{
		Sprite* spriteTips = nullptr;
		// 当前为服务端
		if ( GameMgr()->getIsServer() )
		{
			// 当前可行走角色颜色等于 远程角色颜色
			if ( hero->getSkinColor() == GameMgr()->getLocalColor() )
			{
				GameMgr()->setServerBout(true);
				spriteTips = Sprite::create("T_NiDeHuiHe.png");
				spriteTips->setPosition(Vec2(480, 120));
				this->addChild(spriteTips, UILayer);
			}
			else
			{
				GameMgr()->setServerBout(false);
				spriteTips = Sprite::create("T_DiFangHuiHe.png");
				spriteTips->setPosition(Vec2(480, 120));
				this->addChild(spriteTips, UILayer);
			}
		}
		else
		{
			if ( hero->getSkinColor() == GameMgr()->getRemoteColor() )
			{
				GameMgr()->setServerBout(false);
				//isPassTouche(false, true);
				spriteTips = Sprite::create("T_NiDeHuiHe.png");
				spriteTips->setPosition(Vec2(480, 120));
				this->addChild(spriteTips, UILayer);
			}
			else
			{
				GameMgr()->setServerBout(true);
				//isPassTouche(false, true);
				spriteTips = Sprite::create("T_DiFangHuiHe.png");
				spriteTips->setPosition(Vec2(480, 120));
				this->addChild(spriteTips, UILayer);
			}
		}

		auto calll = CallFunc::create([=]()
		{
			this->removeChild(spriteTips);
		});
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), calll));
	}
	else if ( GameMgr()->getGameFightType() == GFT_AI )
	{
		Sprite* spriteTips = nullptr;
		auto passButton = (Menu*)this->getChildByTag(PassBtnFlag);
		auto pass = (MenuItemSprite*)passButton->getChildByTag(PassBtnFlag);
		// 当前可行走角色颜色等于 远程角色颜色
		if (hero->getSkinColor() == 0)
		{
			spriteTips = Sprite::create("BluePos.png");
			spriteTips->setPosition(Vec2(480, 120));
			this->addChild(spriteTips, UILayer);

			auto disAble = Sprite::create("MainFight/FinishNor.png");
			pass->setNormalImage(disAble);
		}
		else if (hero->getSkinColor() == 3)
		{
			spriteTips = Sprite::create("RedPos.png");
			spriteTips->setPosition(Vec2(480, 120));
			this->addChild(spriteTips, UILayer);
			auto disAble = Sprite::create("RedBUU.png"); 
			pass->setNormalImage(disAble);
		}
		auto calll = CallFunc::create([=]()
		{
			this->removeChild(spriteTips);
		});
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), calll));
	}

	auto progress = (ProgressTimer*)this->getChildByTag(ProgressFlag);
	progress->setPercentage(100);
}

void GameLayer::roleSpin(Hero* _targetRole, RoleDirection _targetDir)
{
	auto callBack = CallFuncN::create(CC_CALLBACK_1(GameLayer::spinCallBack, this, _targetDir));
	auto sequence = Sequence::create(DelayTime::create(TurnTime), callBack, NULL);
	_targetRole->runAction(sequence);
}

void GameLayer::spinCallBack(Ref* pSender, RoleDirection _targetDir)
{
	auto targetRole = (Hero*)pSender;
	if ( targetRole->getRoleDirection() == _targetDir )
	{
		return;
	}

	int curDir = targetRole->getRoleDirection();
	int tarDir = _targetDir;
	int iTemp = tarDir - curDir;
	if (abs(iTemp) > 4)
	{
		if (iTemp > 0)
		{
			curDir--;
			if (curDir < 0)
				curDir = 7;
		}
		else
		{
			curDir++;
			if (curDir > 7)
				curDir = 0;
		}
	}
	else if (abs(iTemp) < 4)
	{
		if (iTemp < 0)
		{
			curDir--;
			if (curDir < 0)
				curDir = 7;
		}
		else
		{
			curDir++;
			if (curDir > 7)
				curDir = 0;
		}
	}
	else if (abs(iTemp) == 4)
	{
		curDir++;
		if (curDir > 7)
			curDir = 0;
	}
	targetRole->setRoleDirAndSkill(S_Standby, (RoleDirection)curDir);
	auto callBack = CallFuncN::create(CC_CALLBACK_1(GameLayer::spinCallBack, this, _targetDir));
	auto sequence = Sequence::create(DelayTime::create(TurnTime), callBack, NULL);
	targetRole->runAction(sequence);
}

void GameLayer::constructPathAndStartAnimationFromStep(AStartPath *step, bool isAttack)
{
	if (isAttack)
	{
		for (auto iter : DataManager::getInstance()->getRoleInfo())
		{
			auto role = (Hero*)iter;
			if (role->getRoleCoord() == step->getPosition())
			{
				hittedHero1 = hero;
				hittedHero2 = role;
				hittedHero1->setFirstPoint(hero->getRoleCoord());
			}
		}
	}
	this->_shortestPath.clear();
	do
	{
		if (step->getParent())
		{
			this->_shortestPath.insert(0, step);
		}
		step = step->getParent();
	} while (step);
	if ( isAttack )
	{
		this->_shortestPath.erase(_shortestPath.size() - 1);
	}
	popStepAndAnimate((int)isAttack);
}

void GameLayer::popStepAndAnimate(int isAttack)
{
	isPassTouche(false, false);
	if (this->_shortestPath.size() == 0)
	{
		isRunning = false;
		if (isAttack)
		{
			runHurtAnimation(hittedHero1, hittedHero2);
		}
		else
		{
			isPassTouche(true, true);
			focusOnRole(hero);
			removePath();
			roleRemoveFlag();
			hero->setRoleDirAndSkill(S_Standby, hero->getRoleDirection());
			showCurRole(ComOrder);
		}
		return;
	}
	if ( isAttack && hero->getRoleType() == RoleArrow )
	{
		runHurtAnimation(hittedHero1, hittedHero2);
	}
	else
	{
		auto stepEnd = this->_shortestPath.at(_shortestPath.size() - 1);
		auto firstStep = this->_shortestPath.at(0);
		int offX = abs(stepEnd->getPosition().x - hero->getRoleCoord().x);
		int offY = abs(stepEnd->getPosition().y - hero->getRoleCoord().y);

		float roleSpeed = 0.0f;
		if (offY > NorDirRun || offX > NorDirRun 
			|| (offX > BiasDirRun && offY > BiasDirRun) 
			|| ( isAttack && offX >= BiasDirRun && offY >= BiasDirRun )
			|| (hero->getRoleType() == RoleSoldier && isAttack && (offY >= NorDirRun || offX >= NorDirRun)))
		{
			isRunning = true;
		}
		if (isRunning || hero->getRoleType() == RoleAssassin)
		{
			hero->setRoleDirAndSkill(S_Sprint, ToolFunc::calcRoleDirection(hero->getRoleCoord(), firstStep->getPosition()));
			roleSpeed = hero->getRoleSpeed() * SprintRate;
		}
		else
		{
			hero->setRoleDirAndSkill(S_Move, ToolFunc::calcRoleDirection(hero->getRoleCoord(), firstStep->getPosition()));
			roleSpeed = hero->getRoleSpeed();
		}
		auto len = sqrt(pow(abs(hero->getRoleCoord().x - firstStep->getPosition().x), 2) +
			pow(abs(hero->getRoleCoord().y - firstStep->getPosition().y), 2));

		// 时间  TODO  WARRING 
		auto moveAction = MoveTo::create(len / roleSpeed, coverToScreenCoord(firstStep->getPosition()));
		
		auto moveCall = CallFunc::create([=]()
		{
			popStepAndAnimate(isAttack);
		});
		if ( hero->getRoleType() == RoleAssassin )
		{
			if ( hero->getRoleState() != S_Sprint )
			{
				focusOnRole(hero);
			}
		}
		else
		{
			focusOnRole(hero);
		}	
		hero->setRoleCoord(firstStep->getPosition());
		this->_shortestPath.erase(0);
		hero->setRoleDir(hero->getRoleDirection());
		hero->runAction(Sequence::create(moveAction, moveCall, nullptr));
	}	
}

void GameLayer::insertOpenPath(AStartPath* step)
{
	int stepFScore = step->getFScore();
	ssize_t count = _spOpenSteps.size();
	ssize_t i = 0;

	for (; i < count; ++i)
	{
		if (stepFScore <= _spOpenSteps.at(i)->getFScore())
		{
			break;
		}
	}
	_spOpenSteps.insert(i, step);
}

void GameLayer::runHurtAnimation(Hero* control, Hero* hitted)
{
	hittedHero1 = control;
	hittedHero2 = hitted;
	isPassTouche(false, false);
	if (hittedHero1->getRoleType() == RoleArrow)		// 弓箭手攻击
	{		
		hittedHero1->setRoleDirAndSkill(S_Attack, ToolFunc::calcRoleDirection(hittedHero1->getRoleCoord(), hittedHero2->getRoleCoord()));
		auto fly = FlyProps::createProps(hittedHero1, hittedHero2, Attack);
		this->addChild(fly);
	}
	else
	{
		// 控制角色移动到被攻击角色格子边缘，被攻击角色移动到相应的格子边缘
		hittedHero1->setRoleDirAndSkill(hittedHero1->getRoleState(), 
			ToolFunc::calcRoleDirection(hittedHero1->getRoleCoord(), hittedHero2->getRoleCoord()));

		auto controlRoleMovePoint = ToolFunc::calcRoleBackPoint(
			ToolFunc::calcRoleDirection(hittedHero1->getRoleCoord(), hittedHero2->getRoleCoord()), Attack);
		Vec2 point = coverToScreenCoord(hittedHero2->getRoleCoord()) + controlRoleMovePoint;

		auto fSpeed = 0.0f;
		auto iDelatX = abs(hittedHero1->getRoleCoord().x - hittedHero2->getRoleCoord().x);
		auto iDelatY = abs(hittedHero1->getRoleCoord().y - hittedHero2->getRoleCoord().y);
		if ( hittedHero1->getRoleState() == S_Sprint )
		{
			fSpeed = hittedHero1->getRoleSpeed() * SprintRate;
		}
		else
		{
			hittedHero1->setRoleDirAndSkill(S_Move, 
				ToolFunc::calcRoleDirection(hittedHero1->getRoleCoord(), hittedHero2->getRoleCoord()));
			fSpeed = hittedHero1->getRoleSpeed();
		}
		auto len = sqrt(pow(abs(hittedHero1->getRoleCoord().x - hittedHero2->getRoleCoord().x), 2) +
			pow(abs(hittedHero1->getRoleCoord().y - hittedHero2->getRoleCoord().y), 2));

		auto sequence = Sequence::create(MoveTo::create(len / fSpeed, point),
			CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_JumpBack)), NULL);
		hittedHero1->runAction(sequence);
	}
}

void GameLayer::comRoleCounterCallBack(Node* pSender, RoleAttackCounter _attack)
{
	if ( _attack == ArrowAttackRole )				// 攻击者：弓箭手
	{
		hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
		hittedHero2->setRoleDirAndSkill(S_Hit, ToolFunc::calcRoleDirection(hittedHero2->getRoleCoord(), hittedHero1->getRoleCoord()));
		hittedHero2->hurt(hittedHero1->getRoleAttack());

		auto moveto = MoveBy::create(BackUp, Vec2(Vec2(ToolFunc::calcRoleBackPoint(
			ToolFunc::calcRoleDirection(hittedHero2->getRoleCoord(), hittedHero1->getRoleCoord()), Attack))	));
		auto moveBack = moveto->reverse();
		auto callBack = CallFunc::create([this]()
		{
			if (hittedHero2->getRoleHP() <= 0)
			{
				auto moveSequence = Sequence::create(
					CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_Death)), NULL);
				hittedHero1->runAction(moveSequence);
			}
			else
			{
				// 被攻击角色为弓箭手时，反击
				if (RoleArrow == hittedHero2->getRoleType())
				{
					auto sequence = Sequence::create(DelayTime::create(hittedHero2->getHittedTime()),
						CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ArrowCounterRole)), NULL);
					hittedHero2->runAction(sequence);
				}
				else
				{
					auto sequence = Sequence::create(DelayTime::create(hittedHero2->getHittedTime()),
						CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ArrowAttackRoleB)), NULL);
					hittedHero2->runAction(sequence);
				}
			}
		});
		hittedHero2->runAction(Sequence::create(moveto, moveBack, DelayTime::create(BackUp), callBack, NULL));
	}
	else if ( ArrowAttackRoleB == _attack )
	{
		hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
		hittedHero2->setRoleDirAndSkill(S_Standby, hittedHero2->getRoleDirection());

		removePath();
		roleRemoveFlag();

		showCurRole(ComOrder);
	}
	else if ( ArrowCounterRole == _attack )
	{
		hittedHero2->setRoleDirAndSkill(S_Attack, hittedHero2->getRoleDirection());
		auto fly = FlyProps::createProps(hittedHero2, hittedHero1, CounterBack);
		this->addChild(fly);
	}
	else if ( ArrowCounterRoleA == _attack )
	{
		hittedHero2->setRoleDirAndSkill(S_Standby, hittedHero2->getRoleDirection());
		hittedHero1->setRoleDirAndSkill(S_Hit, hittedHero1->getRoleDirection());
		hittedHero1->hurt(hittedHero2->getRoleAttack());

		auto moveto = MoveBy::create(BackUp, Vec2(Vec2(ToolFunc::calcRoleBackPoint(
			ToolFunc::calcRoleDirection(hittedHero2->getRoleCoord(), hittedHero1->getRoleCoord()), CounterBack))));
		auto moveBack = moveto->reverse();
		auto callBack = CallFunc::create([this]()
		{
			if (hittedHero1->getRoleHP() <= 0)
			{
				hittedHero1->setRoleDirAndSkill(S_Death, hittedHero1->getRoleDirection());
				auto sequence = Sequence::create(DelayTime::create(hittedHero1->getDeathTime()),
					CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleDeathCallBack, this)), hittedHero1, NULL);
				hittedHero1->runAction(sequence);
			}
			else
			{
				auto sequence = Sequence::create(DelayTime::create(hittedHero1->getDeathTime()),
					CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_Arrow_Counter)), hittedHero1, NULL);
				hittedHero1->runAction(sequence);
			}
		});
		auto sequecne = Sequence::create(moveto, moveBack, callBack, NULL);
		hittedHero1->runAction(sequecne);
	}
	else if ( ComHittedRole == _attack )			// 1 A 2 , 2 hit
	{
		hittedHero2->setRoleDirAndSkill(S_Hit, hittedHero2->getRoleDirection());
		hittedHero2->hurt(hittedHero1->getRoleAttack());

		auto moveto = MoveBy::create(BackUp, Vec2(Vec2(ToolFunc::calcRoleBackPoint(
			ToolFunc::calcRoleDirection(hittedHero2->getRoleCoord(), hittedHero1->getRoleCoord()), Attack)) ));
		auto moveBack = moveto->reverse();

		auto callBack = CallFunc::create([this]()
		{
			if (hittedHero2->getRoleHP() <= 0)
			{
				hittedHero2->setRoleDirAndSkill(S_Death, hittedHero2->getRoleDirection());
				auto callBack = CallFunc::create([this]()
				{
					auto moveSequence = Sequence::create(DelayTime::create(hittedHero2->getDeathTime()),
						CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_Standy)), hittedHero1, NULL);
					hittedHero1->runAction(moveSequence);
				});
				hittedHero2->runAction(callBack);
			}
			else
			{
				hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
				auto sequence = Sequence::create(DelayTime::create(hittedHero2->getAttackTime()),
					CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ComCounterRole)), NULL);
				hittedHero2->runAction(sequence);
			}
		});
		auto sequece = Sequence::create(moveto, moveBack, callBack, NULL);
		hittedHero2->runAction(sequece);
	}
	else if ( ComCounterRole == _attack )			// 2 A 1
	{
		hittedHero2->setRoleDirAndSkill(S_Attack, hittedHero2->getRoleDirection());

		auto sequence = Sequence::create(DelayTime::create(hittedHero2->getAttackTime()),
			CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ComCounterRoleA)), NULL);
		this->runAction(sequence);
	}
	else if ( ComCounterRoleA == _attack )			// 2 A 1, 1 hit
	{
		hittedHero2->setRoleDirAndSkill(S_Standby, hittedHero2->getRoleDirection());
		if ( hittedHero1->getRoleType() == RoleAssassin )
		{
			auto callBack = CallFunc::create([this]()
			{
				hittedHero1->setVisible(false);
				auto animat = AnimationManager::getInstance();
				animat->setPosition(coverToScreenCoord(hittedHero1->getRoleCoord()) + Vec2(-60, 40));
				this->addChild(animat, 10);
				animat->playAssMiss();			

				auto sequence = Sequence::create(DelayTime::create(animat->assMissDelat),
					CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_R2Hitt)), NULL);
				this->runAction(sequence);
			});
			auto sequence = Sequence::create(DelayTime::create(hittedHero1->getHittedTime()), callBack, NULL);
			hittedHero1->runAction(sequence);
		}
		else
		{
			hittedHero1->setLocalZOrder(hittedHero1->getLocalZOrder());
			hittedHero1->setRoleDirAndSkill(S_Hit, hittedHero1->getRoleDirection());
			hittedHero1->hurt(hittedHero2->getRoleAttack());

			auto moveto = MoveTo::create(BackUp, coverToScreenCoord( calcNestPoint(hittedHero1, hittedHero2)));
			auto callBack = CallFunc::create([this]()
			{
				if ( hittedHero1->getRoleHP() <= 0 )
				{
					auto sequence = Sequence::create(DelayTime::create(hittedHero1->getDeathTime()),
						CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleDeathCallBack, this)), NULL);
					hittedHero1->runAction(sequence);
					hittedHero1->setRoleDirAndSkill(S_Death, hittedHero1->getRoleDirection());
				}
				else
				{
					auto sequence = Sequence::create(DelayTime::create(hittedHero1->getHittedTime()),
						CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_R2Hitt)), NULL);
					this->runAction(sequence);
				}
			});
			auto sequence = Sequence::create(moveto, callBack, NULL);
			hittedHero1->runAction(sequence);
		}
	}
}

void GameLayer::comRoleMoveCallBack(Node* pSender, MoveData index)
{
	if (index == M_Death)							// 弓箭手攻击，敌人死亡
	{
		hittedHero2->setRoleDirAndSkill(S_Death, hittedHero2->getRoleDirection());
		
		auto delat = DelayTime::create(hittedHero2->getDeathTime());
		auto se = CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_Fuck));
		hittedHero1->runAction(Sequence::create(delat, se, NULL));
	}
	else if (index == M_Fuck)
	{
		auto sequence = Sequence::create(CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_Nothing)), NULL);
		hittedHero1->runAction(sequence);
	}
	else if (index == M_Nothing )
	{
		auto sequence = Sequence::create(CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_Arrow_Attack_Death)), NULL);
		hittedHero1->runAction(sequence);
	}
	else if (index == M_Arrow_Attack_Death)
	{
		roleRemoveFlag();

		auto offx = abs(hittedHero1->getRoleCoord().x - hittedHero2->getRoleCoord().x);
		auto offy = abs(hittedHero1->getRoleCoord().y - hittedHero2->getRoleCoord().y);
		// 定义速度变量
		float roleSpeed = 0.0f;
		if (offx > NorDirRun || offy > NorDirRun || (offx > BiasDirRun && offy > BiasDirRun))
		{
			hittedHero1->setRoleDirAndSkill(S_Sprint, hittedHero1->getRoleDirection());
			roleSpeed = hittedHero1->getRoleSpeed() * SprintRate;
		}
		else
		{
			hittedHero1->setRoleDirAndSkill(S_Move, hittedHero1->getRoleDirection());
			roleSpeed = hittedHero1->getRoleSpeed();
		}
		auto roleA = hittedHero1->getRoleCoord();
		auto roleB = hittedHero2->getRoleCoord();
		auto lenth = sqrt(pow(abs(roleA.x - roleB.x), 2) + pow(abs(roleA.y - roleB.y), 2));
		auto moveto = MoveTo::create(lenth / roleSpeed, coverToScreenCoord(hittedHero2->getRoleCoord()));
		auto sequence = Sequence::create(moveto, CallFuncN::create(
			CC_CALLBACK_1(GameLayer::comRoleMoveCallBack, this, M_CallMove)), hittedHero1, NULL);
		hittedHero1->runAction(sequence);
		hittedHero1->setRoleCoord(hittedHero2->getRoleCoord());
	}
	else if (index == M_CallMove)
	{
		hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
		comRoleDeathCallBack(hittedHero2);
		hittedHero2->destry();
	}
	else if (M_JumpBack == index)					// 1 A 2
	{
		hittedHero2->setRoleDirAndSkill(S_Standby, ToolFunc::calcRoleDirection(hittedHero2->getRoleCoord(), hittedHero1->getRoleCoord()));
		hittedHero1->setRoleDirAndSkill(S_Attack, ToolFunc::calcRoleDirection(hittedHero1->getRoleCoord(), hittedHero2->getRoleCoord()));
		switch (hittedHero1->getRoleDirection())
		{
		case D_Up:
		case D_LeftUp:
		case D_Left:
			hittedHero1->setLocalZOrder(hittedHero2->getLocalZOrder() + 1);
			break;
		case D_Right:
		case D_RightDown:
		case D_Down:
			hittedHero1->setLocalZOrder(hittedHero2->getLocalZOrder() - 1);
		}

		auto point = calcNestPoint(hittedHero1, hittedHero2);	
		auto sequence = Sequence::create(DelayTime::create(hittedHero1->getAttackTime()),
			CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleCounterCallBack, this, ComHittedRole)), NULL);
		hittedHero1->runAction(sequence);
	}
	else if (M_Standy == index)
	{
		if ( hittedHero1->getRoleType() == RoleAssassin )
		{
			hittedHero1->setRoleDirAndSkill(S_Sprint, hittedHero1->getRoleDirection());
		}
		else
		{
			hittedHero1->setRoleDirAndSkill(S_Move, hittedHero1->getRoleDirection());
		}		

		auto len = sqrt( pow( abs(hittedHero1->getRoleCoord().x - hittedHero2->getRoleCoord().x), 2 ) +
			pow( abs(hittedHero1->getRoleCoord().y - hittedHero2->getRoleCoord().y), 2 ) );

		auto move = MoveTo::create(len / hittedHero1->getRoleSpeed(), Vec2(coverToScreenCoord(hittedHero2->getRoleCoord())));
		auto callBack = CallFunc::create([this]()
		{
			hittedHero1->setRoleCoord(hittedHero2->getRoleCoord());
			hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
			comRoleDeathCallBack(hittedHero2);
		});
		hittedHero1->runAction(Sequence::create(move, callBack, NULL));
	}
	else if (M_R2Hitt == index)						// 2 hit
	{
		if (hittedHero1->getRoleType() == RoleAssassin)
		{
			hittedHero1->setPosition(coverToScreenCoord(hittedHero1->getFirstPoint()));
			auto animat = AnimationManager::getInstance();
			animat->setPosition(hittedHero1->getPosition() + Vec2(-40, 40));
			this->addChild(animat, 10);
			animat->playAssMiss();
			
			auto callBack = CallFunc::create([this]()
			{
				hittedHero1->setVisible(true);
				hittedHero1->setRoleDirAndSkill(S_Hit, hittedHero1->getRoleDir());
				hittedHero1->hurt(hittedHero2->getRoleAttack());
				auto callBack2 = CallFunc::create([this]()
				{
					auto role1CallBack = CallFunc::create([this]()
					{
						hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
						hittedHero1->setRoleCoord(hittedHero1->getFirstPoint());
						roleRemoveFlag();

						showCurRole(ComOrder);
					});
					if (hittedHero1->getRoleHP() <= 0)
					{
						auto sequence = Sequence::create(DelayTime::create(hittedHero1->getDeathTime()),
							CallFuncN::create(CC_CALLBACK_1(GameLayer::comRoleDeathCallBack, this)), NULL);
						hittedHero1->runAction(sequence);
						hittedHero1->setRoleDirAndSkill(S_Death, hittedHero1->getRoleDirection());
						//hittedHero1->runAction(Sequence::create(DelayTime::create( hittedHero1->getDeathTime() ), role1CallBack, NULL));
					}
					else
					{
						hittedHero1->runAction(role1CallBack);
					}				
				});
				hittedHero1->runAction( Sequence::create(DelayTime::create(hittedHero1->getHittedTime()), callBack2, NULL) );
			});
			hittedHero1->runAction( Sequence::create(DelayTime::create(animat->assMissDelat), callBack, NULL) );
		}
		else
		{
			auto role1CallBack = CallFunc::create([this]()
			{
				
				roleRemoveFlag();

				showCurRole(ComOrder);
			});
			hittedHero1->runAction(role1CallBack);
			hittedHero1->setRoleDirAndSkill(S_Standby, ToolFunc::calcRoleDirection(hittedHero1->getRoleCoord(), hittedHero2->getRoleCoord()));
			hittedHero1->setRoleCoord(calcNestPoint(hittedHero1, hittedHero2));
		}
		removePath();
	}
	else if (M_Arrow_Counter == index)
	{
		hittedHero1->setRoleDirAndSkill(S_Standby, hittedHero1->getRoleDirection());
		removePath();
		roleRemoveFlag();
		showCurRole(ComOrder);
	}
}

void GameLayer::comRoleDeathCallBack(Node* pSender)
{
	auto deathRole = (Hero*)pSender;
	curVector.erase(curVector.begin());
	preVectot.push_back(hero);
	auto roleTmpVector = DataManager::getInstance()->getRoleInfo();

	// 先移除头像、再移除数组中的角色
	if ( m_RolePosMagr->deathRemove(curVector, preVectot, deathRole) )
	{
		for (unsigned int i = 0; i < roleTmpVector.size(); i++)
		{
			auto iter = (Hero*)roleTmpVector[i];
			if (iter == deathRole)
			{
				DataManager::getInstance()->heroVector.erase(DataManager::getInstance()->heroVector.begin() + i);
			}
		}

		for (unsigned int i = 0; i < curVector.size(); i++)
		{
			auto iter = (Hero*)curVector[i];
			if (iter == deathRole)
			{
				curVector.erase(curVector.begin() + i);
			}
		}

		for (unsigned int i = 0; i < preVectot.size(); i++)
		{
			auto iter = (Hero*)preVectot[i];
			if (iter == deathRole)
			{
				preVectot.erase(preVectot.begin() + i);
			}
		}
		auto callBack = CallFunc::create([=]()
		{
			showCurRole(DeathOrder);
		});

		this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.2f), callBack));

		if ( GameMgr()->getGameFightType() == GFT_WiFi )
		{
			auto roleInfo = DataMgr()->getRoleInfo();
			if ( GameMgr()->getRoomMode() == Mode_Kill_All )
			{
				int serversTeamSize = 0;
				int clientTeamSize = 0;
				for ( auto itor : roleInfo )
				{
					if ( itor->getSkinColor() == GameMgr()->getLocalColor() )
					{
						serversTeamSize += 1;
					}
					else if ( itor->getSkinColor() == GameMgr()->getRemoteColor() )
					{
						clientTeamSize += 1;
					}
				}

				if (serversTeamSize == 0)
				{
					if ( GameMgr()->getIsServer() )
					{
						gameResult(true, false);
					}
					else
					{
						gameResult(true, false);
					}
				}
				else if ( clientTeamSize == 0 )
				{
					if ( GameMgr()->getIsServer() )
					{
						gameResult(false, true);
					}
					else
					{
						gameResult(false, true);
					}
				}
			}
			else if (GameMgr()->getRoomMode() == Mode_Kill_King)
			{
				if ( deathRole->getRoleType() == RoleKing )
				{
					if (deathRole->getSkinColor() == GameMgr()->getLocalColor())
					{
						// servers king death
						if ( GameMgr()->getIsServer() )
						{
							gameResult(true, false);
						}
						else
						{
							gameResult(false, true);
						}
					}
					else if (deathRole->getSkinColor() == GameMgr()->getRemoteColor())
					{
						// client king death
						if (GameMgr()->getIsServer())
						{
							gameResult(false, true);
						}
						else
						{
							gameResult(true, false);
						}
					}
				}				
			}
		}
		else if ( GameMgr()->getGameFightType() == GFT_AI )
		{			
			if (deathRole->getRoleType() == RoleKing)
			{
				Sprite* sprite = nullptr;
				auto layerColor = LayerColor::create(Color4B(80, 80, 80, 80));
				this->addChild(layerColor, UILayer + 10);
				// hong
				if (deathRole->getSkinColor() == 3)
				{
					sprite = Sprite::create("Blueeee.png");
					sprite->setPosition(Vec2(480, 320));
					layerColor->addChild(sprite);
				}
				else if (deathRole->getSkinColor() == 0)
				{
					sprite = Sprite::create("Redddd.png"); 
					sprite->setPosition(Vec2(480, 320));
					layerColor->addChild(sprite);
				}
				auto callll = CallFunc::create([=]()
				{
					Director::getInstance()->replaceScene(StarLayer::scene());
				});
				this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), callll));
			}			
		}
	}
}

void GameLayer::gameResult(bool isClient, bool isServers)
{
	auto cloltLayer = LayerColor::create(Color4B(80, 80, 80, 80));
	this->addChild(cloltLayer, UILayer + 10);

	if ( GameMgr()->getIsServer() )
	{
		if ( isServers )
		{
			auto sprite = Sprite::create("Success.png");
			sprite->setPosition(Vec2( 480, 320 ));
			cloltLayer->addChild(sprite);
		}
		else
		{
			auto sprite = Sprite::create("Fail.png");
			sprite->setPosition(Vec2(480, 320));
			cloltLayer->addChild(sprite);
		}
	}
	else
	{
		if ( isClient )
		{
			auto sprite = Sprite::create("Success.png");
			sprite->setPosition(Vec2(480, 320));
			cloltLayer->addChild(sprite);
		}
		else
		{
			auto sprite = Sprite::create("Fail.png");
			sprite->setPosition(Vec2(480, 320));
			cloltLayer->addChild(sprite);
		}
	}

	auto callBack = CallFunc::create([=]()
	{
		Director::getInstance()->replaceScene(StarLayer::scene());
	});
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), callBack));
}

void GameLayer::onServerMessage(void* data)
{
	switch (((ProtHead*)data)->protoId)
	{
		case PROT_EXCHANGE_TEAM:
		{
			auto remoteTeam = (ProtExchangeTeam*)data;
			m_remoteTeam.clear();
			auto v_remote = ToolFunc::splitString(remoteTeam->c_team, "#");
			for ( auto itor : v_remote )
			{
				m_remoteTeam.push_back( atoi(itor.c_str()) );
			}
			initHero();
			sortByAgility();
			showCurRole(FirstOrder);

			// init role frame photo
			m_RolePosMagr = RolePosManager::create();
			this->addChild(m_RolePosMagr, UILayer - 10);
			m_RolePosMagr->showRoleOrder(curVector, preVectot, FirstOrder);

			break;
		}
		case PROT_MOVE_ATTACK:
		{
			auto prot = (ProtMoveAttack*)data;
			autoFindPath(prot->tileCoord, true);

			auto str = __String::createWithFormat("PROT_MOVE_ATTACK Client %.3f", 0);
			labelsss->setString(str->getCString());

			auto cal = CallFunc::create([=]()
			{
				labelsss->setString("");
			});
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), cal));

			ProtMoveAttack protrt = { 0 };
			protrt.head.protoId = PROT_SEDN_STOP;
			NK()->send(&protrt, sizeof(protrt));
			break;
		}
		case PROT_PASS_CLICK:
		{
			passCurRole(NULL);
			break;
		}
		case PROT_SEDN_STOP:
		{
			this->unscheduleUpdate();

			auto str = __String::createWithFormat(" Server Recive Message");
			labelsss->setString(str->getCString());

			auto cal = CallFunc::create([=]()
			{
				labelsss->setString("");
			});
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), cal));

			break;
		}
	}
}

void GameLayer::onClientMessage(void* data)
{
	switch (((ProtHead*)data)->protoId)
	{
		case PROT_EXCHANGE_TEAM:
		{
			auto remoteTeam = (ProtExchangeTeam*)data;
			m_remoteTeam.clear();
			auto v_remote = ToolFunc::splitString(remoteTeam->c_team, "#");
			v_remote.pop_back();
			for (auto itor : v_remote)
			{
				m_remoteTeam.push_back(atoi(itor.c_str()));
			}
			// 客户端不需要，重新排序敏捷度
			//sortByAgility();
			//showCurRole(1);
			break;
		}
		case PROT_MOVE_ATTACK:
		{
			auto prot = (ProtMoveAttack*)data;
			autoFindPath(prot->tileCoord, true);

			auto str = __String::createWithFormat("PROT_MOVE_ATTACK Server %.3f", 0);
			labelsss->setString(str->getCString());

			auto cal = CallFunc::create([=]()
			{
				labelsss->setString("");
			});
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), cal));

			ProtMoveAttack protrt = { 0 };
			protrt.head.protoId = PROT_SEDN_STOP;
			NK()->send(&protrt, sizeof(protrt));
			break;
		}
		case PROT_PASS_CLICK:
		{
			passCurRole(NULL);
			break;
		}
		case PROT_SEND_TEAM:
		{
			initHero();
			auto prot = (ProtAgality*)data;
			auto m_fuckID = Utils::split_string(prot->idStr, "#");
			m_fuckID.pop_back();
			auto m_fuckColor = Utils::split_string(prot->colorStr, "#");
			m_fuckColor.pop_back();


			vector<Hero* > tempRole;
			tempRole.clear();
			curVector.clear();
			curVector = DataManager::getInstance()->getRoleInfo();
			int colorTag = 0;
			for (auto itorID : m_fuckID)
			{
				for (auto itor : curVector)
				{
					if (itor->getRoleID() == atoi(itorID.c_str()))
					{
						if (atoi(m_fuckColor[colorTag].c_str()) == itor->getSkinColor())
						{
							colorTag++;
							tempRole.push_back(itor);
							break;
						}
					}
				}				
			}

			curVector.clear();
			curVector = tempRole;

			// init role frame photo
			m_RolePosMagr = RolePosManager::create();
			this->addChild(m_RolePosMagr, UILayer - 10);
			m_RolePosMagr->showRoleOrder(curVector, preVectot, FirstOrder);

			showCurRole(FirstOrder);
			break;
		}
		case PROT_SEDN_STOP:
		{
			this->unscheduleUpdate();

			auto str = __String::createWithFormat(" Client Recive Message");
			labelsss->setString(str->getCString());

			auto cal = CallFunc::create([=]()
			{
				labelsss->setString("");
			});
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), cal));
			break;
		}
	}
}

void GameLayer::onServerDisconnect()
{
	onDisconnect(true);
}

void GameLayer::onClientDisconnect()
{
	onDisconnect(false);
}

void GameLayer::onDisconnect(bool isClient)
{
	NK()->stop();
	auto overLayer = GameOverLayer::create();
	overLayer->setResult(isClient);
	this->addChild(overLayer, UILayer + 1);
}