#include "SelectLayer.h"
#include "DataManager/DataManager.h"
#include "RoleSelect.h"
#include "PublicTool/ComResDef.h"

#include "GameLayer/GameLayer.h"
#include "GameManager/GameManager.h"


bool SelectLayer::init()
{
	mainLandValue = 0;
	skyValue = 0;
	geocenterValue = 0;
	DataManager::getInstance()->roleInBattle.clear();

	auto bg = Sprite::create("MapSprite/Map_1.png");
	bg->setAnchorPoint(Vec2::ZERO);
	this->addChild(bg, -2);
	auto roleVector = DataManager::getInstance()->getVRolePropty();
	vector<RoleProprty > mainLandVector;
	mainLandVector.clear();
	vector<RoleProprty > skyVector;
	skyVector.clear();
	vector<RoleProprty > geocenterVector;
	geocenterVector.clear();

	for ( auto itor : roleVector )
	{
		if ( itor.sR_RolePossess == P_MainLand )
		{
			mainLandVector.push_back(itor);
		}
		else if ( itor.sR_RolePossess == P_Sky )
		{
			skyVector.push_back(itor);
		}
		else if ( itor.sR_RolePossess == P_Geocenter )
		{
			geocenterVector.push_back(itor);
		}
	}

	for (unsigned int i = 0; i < mainLandVector.size(); ++ i)
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
	mainLandLabel->setPosition(Vec2( 150, 600 ));
	this->addChild(mainLandLabel, 2);

	auto sprite = Sprite::create("mainland.png");
	sprite->setPosition(Vec2(150, 560));
	this->addChild(sprite);

	skyLabel = Label::createWithTTF("0 / 450", PixelFont, 20);
	skyLabel->setPosition(Vec2( 450, 600 ));
	this->addChild(skyLabel, 2);

	auto sprite2 = Sprite::create("sky.png");
	sprite2->setPosition(Vec2(450, 560));
	this->addChild(sprite2);

	geocenterLabel = Label::createWithTTF("0 / 450", PixelFont, 20);
	geocenterLabel->setPosition(Vec2( 750, 600 ));
	this->addChild(geocenterLabel, 2);

	auto sprite1 = Sprite::create("ceo.png");
	sprite1->setPosition(Vec2(750, 560));
	this->addChild(sprite1);

	auto spriteNor = Sprite::create("sssssstart.png");
 	auto spriteClick = Sprite::create("sssssstart.png");
	spriteClick->setScale(0.5f);
	spriteClick->setPosition(Vec2(spriteNor->getPosition().x + spriteNor->getBoundingBox().size.width / 2 - spriteClick->getBoundingBox().size.width / 2,
		spriteNor->getPosition().y + spriteNor->getBoundingBox().size.height / 2 - spriteClick->getBoundingBox().size.height / 2));

	auto startButton = MenuItemSprite::create(spriteNor, spriteClick, CC_CALLBACK_1(SelectLayer::starClick, this));
	startButton->setPosition(Vec2(480, 200));

	auto pMenu = Menu::create(startButton, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 10);

	auto spri = Sprite::create("selcelt.png");
	spri->setPosition(Vec2(480, 100));
	this->addChild(spri);

	return true;
}

bool SelectLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto roleVector = DataManager::getInstance()->getVRolePropty();
	auto touchPoint = touch->getLocation();

	for (unsigned int i = 0; i < roleVector.size(); ++ i)
	{
		auto roleFrame = (RoleSelect*)this->getChildByTag(i + RoleStarFlag);
		auto size = roleFrame->getFrameRect() * roleFrame->getScale();
		auto position = roleFrame->getPosition();
		auto rect = Rect(position.x - size.width / 2, position.y - size.height / 2, size.width, size.height);

		if ( rect.containsPoint( touchPoint ) )
		{
			int id = roleFrame->getTag();
			int marshalValue = roleVector[id - RoleStarFlag].sR_Marshal;
			if (roleFrame->isSelect())
			{
				roleFrame->setGray(false);
				calcMarshal(roleVector[ id - RoleStarFlag ].sR_RolePossess, -marshalValue);
				auto roleBattle = DataManager::getInstance()->getRoleInBattle();
				for (vector<int>::iterator iter = roleBattle.begin(); iter != roleBattle.end();)
				{
					if (*iter == id)
						iter = roleBattle.erase(iter);
					else
						iter++;
					DataManager::getInstance()->roleInBattle.clear();
					DataManager::getInstance()->roleInBattle = roleBattle;
				}
			}
			else
			{
				roleFrame->setGray(true);
				calcMarshal(roleVector[id - RoleStarFlag].sR_RolePossess, marshalValue);
				DataManager::getInstance()->roleInBattle.push_back(id);
			}			
		}
	}
	return true;
}

void SelectLayer::calcMarshal(RolePossess _possess, int iMarshal)
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

void SelectLayer::onEnter()
{
	Scene::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SelectLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SelectLayer::onExit()
{
	Scene::onExit();
}

void SelectLayer::starClick(Ref* pSender)
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
		}
		else if (mainLandValue == 0 && skyValue == 0 && geocenterValue == 0)
		{
			return;
		}
		else if (mainLandValue > 450 || skyValue > 450 || geocenterValue > 450)
		{
			return;
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
				Director::getInstance()->replaceScene(GameLayer::scene());
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
			}
		}
	}
	else if ( GameMgr()->getGameFightType() == GFT_WiFi )
	{		
		if (( (mainLandValue > 0 || skyValue > 0) && geocenterValue > 0) 
			|| (mainLandValue > 0 && (skyValue > 0 || geocenterValue > 0))
			|| (skyValue > 0 && ( mainLandValue > 0 || geocenterValue > 0 ))
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
		}
		else if (mainLandValue == 0 && skyValue == 0 && geocenterValue == 0)
		{
			return;
		}
		else if (mainLandValue > 450 || skyValue > 450 || geocenterValue > 450)
		{
			return;
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
				Director::getInstance()->replaceScene(GameLayer::scene());
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
			}
		}
	}	
}
