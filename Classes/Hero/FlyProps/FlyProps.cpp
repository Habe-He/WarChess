#include "FlyProps.h"
#include "PublicTool/ToolFunc.h"
#include "GameLayer/GameDef.h"
#include "GameManager/GameInfo.h"

FlyProps::FlyProps(void)
{
	flySprite = nullptr;
}

FlyProps::~FlyProps(void)
{
	
}

FlyProps* FlyProps::createProps(Hero* _hero, Hero* _targetRole, AttackOrCounter _type)
{
	auto flyProps = new FlyProps();
	if ( flyProps )
	{
		flyProps->autorelease();
		flyProps->create(_hero, _targetRole, _type);
		return flyProps;
	}
	CC_SAFE_DELETE(flyProps);
	return nullptr;	
}

bool FlyProps::create(Hero* _hero, Hero* _targetRole, AttackOrCounter _type)
{
	auto delat = ((GongJian*)_hero)->getAttackDelat();

	auto callFunc = CallFunc::create([=]()
	{
		auto heroPoint = _hero->getRoleCoord();
		auto targetPoint = _targetRole->getRoleCoord();
		auto len = sqrt(pow(abs(heroPoint.x - targetPoint.x), 2) + pow(abs(heroPoint.y - targetPoint.y), 2));
		if (_hero->getRoleType() == RoleArrow)
		{
			flySprite = Sprite::create("FlyProps/GongJian_0.png");
			_hero->addChild(flySprite, -20);

			auto grayArrow = Sprite::create("FlyProps/GongJian_1.png");
			grayArrow->setPosition(Vec2(20, -20));
			flySprite->addChild(grayArrow);

			auto dir = ToolFunc::calcRoleDirection(_hero->getRoleCoord(), _targetRole->getRoleCoord());
			if (dir == D_Down)
			{
				flySprite->setRotation(180.0f);
				grayArrow->setPosition(Vec2(10, 40));
			}
			else if (dir == D_Left)
			{
				flySprite->setFlippedX(true);
				grayArrow->setFlippedX(true);
			}
			else if (dir == D_Right)
			{
				flySprite->setFlippedY(true);
				grayArrow->setFlippedY(true);
			}

			auto delatXY = Vec2(_targetRole->getPositionX() - _hero->getPositionX(), _targetRole->getPositionY() - _hero->getPositionY());
			auto moveto = MoveTo::create(len / ArrowRate, delatXY);
			auto callBack = CallFunc::create([=]()
			{
				flySprite->removeFromParentAndCleanup(true);

				this->removeAllChildrenWithCleanup(true);
				this->removeFromParentAndCleanup(true);

				if (_type == Attack)
				{
					auto sData = String::create(AorrwNotCall);
					sData->retain();
					NotificationCenter::getInstance()->postNotification(AorrwNotCall, sData);

// 					char* buf = new char[10];
// 					sprintf(buf, "%d", ARROW_HRUT_FLAG);
// 					EventCustom event(Event_String);
// 					event.setUserData(buf);
// 					_eventDispatcher->dispatchEvent(&event);
// 					CC_SAFE_DELETE_ARRAY(buf);
				}
				else if (_type == CounterBack)
				{
					auto sData = String::create(ArrowCounter);
					sData->retain();
					NotificationCenter::getInstance()->postNotification(ArrowCounter, sData);

// 					char* buf = new char[10];
// 					sprintf(buf, "%d", ARROW_COUNTER_FLAG);
// 					EventCustom event(Event_String);
// 					event.setUserData(buf);
// 					_eventDispatcher->dispatchEvent(&event);
// 					CC_SAFE_DELETE_ARRAY(buf);
				}
			});
			flySprite->runAction(Sequence::create(moveto, callBack, NULL));
		}
	});
	this->runAction(Sequence::create(DelayTime::create(delat), callFunc, nullptr));
	
	return true;
}