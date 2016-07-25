#include "RolePosition.h"
#include "PublicTool/ToolFunc.h"
#include "GameLayer/GameDef.h"
#include "DataManager/DataManager.h"
#include "GameManager/GameManager.h"

const float PhotoScaleBig = 0.25f;			// photo sacle big time
const float PhotoScaleSm = 0.15f;			// photo sacle small
const float PhotoMove = 0.4f;				// photo move time


RolePosition::RolePosition()
{

}

RolePosition::~RolePosition()
{

}

RolePosition* RolePosition::creatInstance(RoleType roleType, int agality, Color3B color)
{
	auto sprite = new (std::nothrow) RolePosition();
	if (sprite && sprite->createRoleFrame(roleType, agality, color))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool RolePosition::createRoleFrame(RoleType roleType, int agality, Color3B color)
{
	if ( roleType > RoleMax)
	{
		return false;
	}

	// 角色头像
	auto name_string = __String::createWithFormat("Role/Photo/%s.png", nameString[roleType].c_str());
	auto rolePhoto = Sprite::create(name_string->getCString());
	rolePhoto->setTag(FrameFlag);
	this->addChild(rolePhoto, -2);

	auto photoSize = rolePhoto->getContentSize();
	auto photoPoint = rolePhoto->getPosition();

	// 头像框
	auto roleFrame = Sprite::create("MainFight/Role.png");
	roleFrame->setColor(color);
	roleFrame->setTag(FrameFlag + 1);
	roleFrame->setPosition(Vec2(photoPoint.x + photoSize.width / 2, photoPoint.y + photoSize.height / 2));
	rolePhoto->addChild(roleFrame, -1);
	// 敏捷度背景
	auto valeuBg = Sprite::create("MainFight/Agality.png");
	valeuBg->setTag(FrameFlag + 2);
	valeuBg->setPosition(Vec2(photoPoint.x + photoSize.width, photoPoint.y));
	rolePhoto->addChild(valeuBg);
	// 敏捷度数字标签
	auto agilityString = __String::createWithFormat("%d", agality);
	auto agilitylabel = Label::createWithTTF("", "fonts/Pixel.TTF", 20.0f);
	agilitylabel->setColor(Color3B(105, 205, 150));
	agilitylabel->setString(agilityString->getCString());
	agilitylabel->setPosition(Vec2(valeuBg->getContentSize().width / 2, valeuBg->getContentSize().height / 2));
	valeuBg->addChild(agilitylabel);

	// name 文本标签
	auto agilitylabel1 = Label::createWithSystemFont("", "", 20.0f);
	agilitylabel1->setColor(Color3B::GREEN);
	agilitylabel1->setTag(112);
	agilitylabel1->setPosition(Vec2(photoSize.width / 2, photoSize.height - 10));
	rolePhoto->addChild(agilitylabel1, 100);

	auto indexLabel = Label::createWithSystemFont("", "", 26.0f);
	indexLabel->setColor(Color3B::BLACK);
	indexLabel->setTag(113);
	indexLabel->setPosition(Vec2( photoSize.width / 2, photoSize.height / 2 ));
	rolePhoto->addChild(indexLabel, 100);

	return true;
}

void RolePosition::setFrameColor(Color3B color)
{
	auto photo = (Sprite*)this->getChildByTag(FrameFlag);
	auto frame = (Sprite*)this->getChildByTag(FrameFlag + 1);

	photo->setColor(color);
	frame->setColor(color);
}

void RolePosition::setFrameVisable(bool isVisable)
{
	auto photo = (Sprite*)this->getChildByTag(FrameFlag);
	auto frame = (Sprite*)photo->getChildByTag(FrameFlag + 1);
	auto value = (Sprite*)photo->getChildByTag(FrameFlag + 2);

	photo->setVisible(isVisable);
	frame->setVisible(isVisable);
	value->setVisible(isVisable);
}

void RolePosition::setFrameTag(int tag)
{
	auto sprite = (Sprite*)this->getChildByTag(FrameFlag);
	auto label = (Label*)sprite->getChildByTag(113);
	auto str = __String::createWithFormat("%d", tag);

	label->setString(str->getCString());
}

void RolePosition::setFrameName(string name)
{
	auto photo = (Sprite*)this->getChildByTag(FrameFlag);
	auto label = (Label*)photo->getChildByTag(112);
	auto str = __String::createWithFormat("%s", name.c_str());

	label->setString(str->getCString());
}





RolePosManager::RolePosManager()
{
	m_isCurVectorPhoto = false;
	m_isPreVectorPhoto = false;
	m_isFirstPhoto = false;
}

RolePosManager::~RolePosManager()
{
	
}

Scene* RolePosManager::scene()
{
	auto scene = Scene::create();
	scene->addChild(RolePosManager::create());

	return scene;
}

bool RolePosManager::init()
{
	auto rolePosNode = CSLoader::createNode("MainFightNode.csb");
	rolePosNode->setPosition(Vec2(130, 520));
	this->addChild(rolePosNode, -10);

	frameLayer = (ScrollView*)rolePosNode->getChildByName("ScrollView_1");
	
	return true;
}

void RolePosManager::showRoleOrder(vector<Hero* >& vCurRole, vector<Hero* >& vPreRole, OrderRole orderRole)
{
	if (vCurRole.size() == 1 && vPreRole.size() == 0)
		return;

	auto roleLocalPropty = DataMgr()->getVRolePropty();
	if ( orderRole == FirstOrder )
	{
		int indexTag = 0;
		for ( auto iter : vCurRole )
		{
			auto roleFrame = RolePosition::creatInstance(iter->getRoleType(), iter->getRolePossess(), GameMgr()->getTeamColor(iter->getSkinColor()));
			roleFrame->setTag(indexTag);
			frameLayer->addChild(roleFrame, RoleFrameZ);
			roleFrame->setScale(67 / 100.0f);
			roleFrame->setPosition(Vec2(70 + indexTag * 64, 60));
			roleFrame->setFrameName(roleLocalPropty[iter->getRoleID() - RoleStarFlag].sR_ChinaName);
			roleFrame->setFrameTag(indexTag);

			// set big photo position
			if ( indexTag == 0 )
			{
				roleFrame->setScale(1.0f);
				roleFrame->setPosition(Vec2(50, 50));
			}
			// set dot sprite position
			else if ( indexTag == vCurRole.size() - 1 )
			{
				auto dotSprite = Sprite::create("MainFight/Dot.png");
				dotSprite->setPosition(Vec2(roleFrame->getPositionX() + 49, roleFrame->getPositionY()));
				dotSprite->setTag(DotSpriteFlag);
				frameLayer->addChild(dotSprite, 10);
			}
			indexTag++;
		}
	}
	else if ( orderRole == ComOrder )
	{
		// get big photo TODO : get main sprite tag
		auto mainTag = vPreRole.size() > 0 ? vPreRole.size() - 1 : vCurRole.size() - 1;
		auto bigPhoto = (RolePosition*)frameLayer->getChildByTag(mainTag);
		bigPhoto->setLocalZOrder(RoleFrameZ - 10);
		auto scaleA = ScaleTo::create(PhotoScaleSm, 1.2f);
		auto scaleB = ScaleTo::create(PhotoScaleBig, 0.0f); // 0.5f
		// main sprite set visable = false
		auto callVisable = CallFunc::create([=]()
		{
			bigPhoto->setFrameVisable(false);
		});
		// get dot sprite
		auto dotSprite = (Sprite*)frameLayer->getChildByTag(DotSpriteFlag);

		// move other photo and display another main sprite, The main sprite set move and set visable
		auto allPhotoMove = CallFunc::create([=]()
		{
			MoveTo* moveTo = nullptr;
			// unmove photo
			for (unsigned index = 0; index < vCurRole.size(); ++ index)
			{
				auto moveSprite = (RolePosition*)frameLayer->getChildByTag(index + vPreRole.size());
				if ( index == 0 )
				{
					moveTo = MoveTo::create(PhotoMove, Vec2(50, 50));
					auto mainScale = ScaleTo::create(PhotoScaleBig, 1.0f);
					moveSprite->runAction(mainScale);

					auto dotSprite = (Sprite*)frameLayer->getChildByTag(DotSpriteFlag);
					dotSprite->runAction(MoveTo::create(PhotoMove, Vec2(dotSprite->getPositionX() - 64, dotSprite->getPositionY())));
				}
				else
				{
					if (vPreRole.size() != 0)
					{
						moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64, moveSprite->getPositionY()));
					}
					else
					{
						moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64 - 34, moveSprite->getPositionY()));
					}
				}
				moveSprite->runAction(moveTo);			
			}

			for (unsigned index = 0; index < vPreRole.size(); ++index)
			{
				auto movedPhoto = (RolePosition*)frameLayer->getChildByTag(index);
				auto moveTo = MoveTo::create(0.4f, Vec2(movedPhoto->getPositionX() - 64, movedPhoto->getPositionY()));
				movedPhoto->runAction(moveTo);
			}
		});
		
		auto moveToDotBehind = CallFunc::create([=]()
		{
			if (vPreRole.size() == 0)
			{
				auto index = vCurRole.size() > 2 ? vCurRole.size() - 2 : 0;
				auto tempSprite = (RolePosition*)frameLayer->getChildByTag(index);
				dotSprite->setPosition(tempSprite->getPosition() + Vec2(15 + 64, 0));
			}
			auto moveTo = MoveTo::create(0.5f, Vec2(dotSprite->getPositionX() + vPreRole.size() * 64 - 15, dotSprite->getPositionY()));
			bigPhoto->runAction(moveTo);

			// 头像可见，放大
			auto callA = CallFunc::create([=]()
			{
				bigPhoto->setFrameVisable(true);
				auto scaleD = ScaleTo::create(PhotoScaleSm, 1.0f);
				auto scaleC = ScaleTo::create(PhotoScaleBig, 0.67f);
				// 把层级调回
				auto callShow = CallFunc::create([=]()
				{
					bigPhoto->setLocalZOrder(RoleFrameZ);
				});
				bigPhoto->runAction(Sequence::create(scaleD, scaleC, DelayTime::create(0.2f), callShow, nullptr));
			});
			bigPhoto->runAction(Sequence::create(DelayTime::create(0.4f), callA, NULL));
		});

// 		char* buf = new char[10];
// 		sprintf(buf, "%d", IS_PASSTOUCH_FLAG);
// 		EventCustom event(Event_String);
// 		event.setUserData(buf);
// 		_eventDispatcher->dispatchEvent(&event);
// 		CC_SAFE_DELETE_ARRAY(buf);

		auto returnBool = CallFunc::create([=]()
		{
			auto sData = String::create(PassTouch);
			sData->retain();
			NotificationCenter::getInstance()->postNotification(PassTouch, sData);
		});		

		auto squence = Sequence::create(scaleA, scaleB, callVisable, moveToDotBehind, allPhotoMove, DelayTime::create(0.4f), returnBool, nullptr);
		bigPhoto->runAction(squence);
	}
	else if ( orderRole == DeathOrder )
	{
		auto mainTag = vPreRole.size() > 0 ? vPreRole.size() - 1 : vCurRole.size() - 1;
		auto mainSprite = (RolePosition*)frameLayer->getChildByTag(mainTag);
		mainSprite->setLocalZOrder(RoleFrameZ - 10);
		// get dot sprite
		auto dotSprite = (Sprite*)frameLayer->getChildByTag(DotSpriteFlag);

		// 当前头像的放大 、 缩小、 不可见 动作
		auto mian_Call = CallFunc::create([=]()
		{
			auto main_ScaleA = ScaleTo::create(0.5f, 1.2f);
			auto main_ScaleB = ScaleTo::create(0.5f, 0.0f);

			auto visable_Call = CallFunc::create([=]()
			{
				mainSprite->setVisible(false);
			});
			mainSprite->runAction(Sequence::create(main_ScaleA, main_ScaleB, DelayTime::create(1.0f), visable_Call, nullptr));
		});

		// 第二个头像的 放大 前移, 后面的头像跟随移动
		auto sec_Call = CallFunc::create([=]()
		{
			for (unsigned index = 0; index < vCurRole.size(); ++index)
			{
				MoveTo* moveTo = nullptr;
				auto moveSprite = (RolePosition*)frameLayer->getChildByTag(index + vPreRole.size());
				if (index == 0)
				{
					moveTo = MoveTo::create(PhotoMove, Vec2(50, 50));
					auto mainScale = ScaleTo::create(PhotoScaleBig, 1.0f);
					moveSprite->runAction(mainScale);

					auto dotSprite = (Sprite*)frameLayer->getChildByTag(DotSpriteFlag);

					dotSprite->runAction(MoveTo::create(PhotoMove, Vec2(dotSprite->getPositionX() - 64, dotSprite->getPositionY())));
				}
				else
				{		
					if (m_isCurVectorPhoto)
					{
						if (vPreRole.size() != 0)
						{
							moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64, moveSprite->getPositionY()));
						}
						else
						{
							moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64 - 34, moveSprite->getPositionY()));
						}
					}
					else
					{
						if (vPreRole.size() != 0)
						{
							moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64, moveSprite->getPositionY()));
						}
						else
						{
							if ( m_isFirstPhoto )
							{
								moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64, moveSprite->getPositionY()));
							}
							else
							{
								moveTo = MoveTo::create(PhotoMove, Vec2(moveSprite->getPositionX() - 64 - 34, moveSprite->getPositionY()));
							}							
						}
					}
				}
				moveSprite->runAction(moveTo);
			}

			auto call_PreMove = CallFunc::create([=]()
			{
				for (unsigned index = 0; index < vPreRole.size(); ++index)
				{
					auto movedPhoto = (RolePosition*)frameLayer->getChildByTag(index);
					auto moveTo = MoveTo::create(0.4f, Vec2(movedPhoto->getPositionX() - 64, movedPhoto->getPositionY()));
					movedPhoto->runAction(moveTo);
				}
			});

			this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), call_PreMove));			
		});

		// 原先隐藏的大头像显示
		auto show_Call = CallFunc::create([=]()
		{
			if ( m_isFirstPhoto )
			{
				if (vPreRole.size() == 0)
				{
					auto frameRole = (RolePosition*)frameLayer->getChildByTag(vCurRole.size() - 1);
					dotSprite->setPosition(Vec2(frameRole->getPosition() + Vec2( 34 + 15, 0 )));
				}
				return;
			}
			else
			{
				if (vPreRole.size() == 0)
				{
					auto index = vCurRole.size() > 2 ? vCurRole.size() - 2 : 0;
					auto tempSprite = (RolePosition*)frameLayer->getChildByTag(index);
					if ( vCurRole.size() > 1)
					{
						if ( m_isPreVectorPhoto )
						{
							log("This ____ m_isPreVectorPhoto  0");
							log("This is First Role Photo  0");
							dotSprite->setPosition(tempSprite->getPosition() + Vec2(64 - 15 + 64, 0));
						}
						else if ( m_isCurVectorPhoto )
						{
							log("This ____ m_isCurVectorPhoto 1");
							if ( vPreRole.size() == 0 )
							{
								dotSprite->setPosition(tempSprite->getPosition() + Vec2(64 + 64-15, 0));
							}
							else
							{
								dotSprite->setPosition(tempSprite->getPosition() + Vec2(64 + 64, 10));
							}
						}
					}
					else
					{
						log("This ____ 2");
						dotSprite->setPosition(tempSprite->getPosition() + Vec2(64 + 15, 0));
					}					
				}
			}
			
			if ( vPreRole.size() == 0 )
			{
				if ( m_isPreVectorPhoto )
				{
					log("Maybe Error m_isPreVectorPhoto");
					auto moveTo = MoveTo::create(0.5f, Vec2(dotSprite->getPositionX() - 64 + 15, dotSprite->getPositionY()));
					mainSprite->runAction(moveTo);
				}
				else if ( m_isCurVectorPhoto )
				{
					log("Maybe Error m_isCurVectorPhoto");
					auto moveTo = MoveTo::create(0.5f, Vec2(dotSprite->getPositionX() - 64 + 15, dotSprite->getPositionY()));
					mainSprite->runAction(moveTo);
				}
			}
			else
			{
				log("No Error");
				auto moveTo = MoveTo::create(0.5f, Vec2(dotSprite->getPositionX() + vPreRole.size() * 64 - 15, dotSprite->getPositionY()));
				mainSprite->runAction(moveTo);
			}
			
			// 头像可见，放大
			auto callA = CallFunc::create([=]()
			{
				mainSprite->setFrameVisable(true);
				auto scaleD = ScaleTo::create(PhotoScaleSm, 1.0f);
				auto scaleC = ScaleTo::create(PhotoScaleBig, 0.67f);
				// 把层级调回
				auto callShow = CallFunc::create([=]()
				{
					mainSprite->setVisible(true);
					mainSprite->setLocalZOrder(RoleFrameZ);
				});
				mainSprite->runAction(Sequence::create(scaleD, scaleC, DelayTime::create(0.2f), callShow, nullptr));
			});
			mainSprite->runAction(Sequence::create(DelayTime::create(0.4f), callA, NULL));
		});

		auto returnBool = CallFunc::create([=]()
		{
			auto sData = String::create(PassTouch);
			sData->retain();
			NotificationCenter::getInstance()->postNotification(PassTouch, sData);
		});

		if ( m_isFirstPhoto )
		{
			this->runAction(Sequence::create(DelayTime::create(2.0f), sec_Call, DelayTime::create(1.5f), show_Call, DelayTime::create(0.4f), returnBool, nullptr));
		}
		else
		{
			this->runAction(Sequence::create(mian_Call, DelayTime::create(2.0f), sec_Call, DelayTime::create(1.5f), show_Call, DelayTime::create(0.4f), returnBool, nullptr));
		}
	}
}

bool RolePosManager::deathRemove(vector<Hero* >& vCurRole, vector<Hero* >& vPreRole, Hero* deathRole)
{
	/*
		角色头像移动后，tag并没有重设，只有在死亡以后，角色头像重设
	*/

	m_isCurVectorPhoto = false;
	m_isPreVectorPhoto = false;
	m_isFirstPhoto = false;
	
	int index = 0;
	for (unsigned int i = 0; i < vCurRole.size(); ++i)
	{
		auto role = (Hero*)vCurRole[i];
		if ( deathRole == role )
		{
			m_isCurVectorPhoto = true;
			
			auto removeSprite = (RolePosition*)frameLayer->getChildByTag(i);
			auto callBack = CallFunc::create([=]
			{
				frameLayer->removeChildByTag(vPreRole.size() + i);
			});
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2f), callBack));
			index = vPreRole.size() + i;

			log("vCurRole size = %d, index  = %d",vCurRole.size(), index);
			log("m_isCurVectorPhoto = true");
		}
	}

	for (unsigned int i = 0; i < vPreRole.size(); ++i)
	{		
		auto role = (Hero*)vPreRole[i];
		if (deathRole == role)
		{
			m_isPreVectorPhoto = true;
			frameLayer->removeChildByTag(i);
			index = i;
			if ( index == vPreRole.size() - 1 )
			{
				log("vPreRole size = %d, index  = %d", vCurRole.size(), index);
				log("m_isFirstPhoto = true");
				m_isFirstPhoto = true;
			}
		}
	}

	auto callMove = CallFunc::create([=]()
	{
		if (m_isCurVectorPhoto)
		{
			for (unsigned int i = index + 1; i < vCurRole.size() + vPreRole.size(); ++i)
			{
				auto tempRole = (RolePosition*)frameLayer->getChildByTag(i);
				tempRole->setTag(tempRole->getTag() - 1);
				tempRole->setFrameTag(tempRole->getTag());
				auto moveto = MoveTo::create(0.2f, Vec2(tempRole->getPosition() - Vec2(64, 0)));
				tempRole->runAction(moveto);
			}

			// 移动过的角色头像
			for (unsigned int i = 0; i < vPreRole.size(); ++i)
			{
				auto frameRole = (RolePosition*)frameLayer->getChildByTag(i);
				if ( i != vPreRole.size() - 1 )
				{
					auto moveto = MoveTo::create(0.2f, Vec2(frameRole->getPosition() - Vec2(64, 0)));
					frameRole->runAction(moveto);
				}				
			}

			// 分隔符移动
			auto dotSprite = (Sprite*)frameLayer->getChildByTag(DotSpriteFlag);
			auto moveto = MoveTo::create(0.2f, Vec2(dotSprite->getPosition() - Vec2(64, 0)));
			dotSprite->runAction(moveto);
		}
		else if (m_isPreVectorPhoto)
		{
			// 未位移角色头像位移
			for (unsigned int z = vPreRole.size(); z < vCurRole.size() + vPreRole.size(); ++z)
			{
				auto frameRole = (RolePosition*)frameLayer->getChildByTag(z);
				frameRole->setTag(frameRole->getTag() - 1);
				frameRole->setFrameTag(frameRole->getTag());
				if (index == 0 && vPreRole.size() == 0 && !m_isFirstPhoto)
				{
					auto moveto = MoveTo::create(0.2f, Vec2(frameRole->getPosition() + Vec2(34, 0)));
					frameRole->runAction(moveto);
				}
			}

			// 移动过的角色头像位移
			for (unsigned int z = index + 1; z < vPreRole.size(); ++z)
			{
				auto frameRole = (RolePosition*)frameLayer->getChildByTag(z);
				frameRole->setTag(frameRole->getTag() - 1);
				frameRole->setFrameTag(frameRole->getTag());
				if (!m_isFirstPhoto && z != vPreRole.size() - 1)
				{
					auto moveto = MoveTo::create(0.2f, Vec2(frameRole->getPosition() - Vec2(64, 0)));
					frameRole->runAction(moveto);
				}
			}
		}
	});

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.2f), callMove));	
	
	return true;
}

void RolePosManager::sortRoleOrder(vector<Hero* > vCurRole, OrderRole roleOrder)
{
// 	if ( roleOrder == FirstOrder )
// 	{
// 		for (auto itor : vCurRole)
// 		{
// 
// 		}
// 	}	
}