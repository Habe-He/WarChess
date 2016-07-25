#include "AStartPath.h"
#include "DataManager/DataManager.h"
#include "Hero/Hero.h"



AStartPath::AStartPath():_position( Vec2::ZERO )
{
    _gScore = 0;
    _hScore = 0;
    _parent = nullptr;
    
}

AStartPath::~AStartPath()
{
    
}

AStartPath* AStartPath::getInstance(const cocos2d::Vec2 &pos)
{
    auto mpInstance = new AStartPath();
    if ( mpInstance && mpInstance->initWithPosition(pos) )
    {
        mpInstance->autorelease();
        return mpInstance;
    }
    else
    {
        CC_SAFE_DELETE(mpInstance);
        mpInstance = nullptr;
        return mpInstance;
    }
}

bool AStartPath::initWithPosition(const cocos2d::Vec2 &pos)
{
    bool bRet = false;
    do
    {
        this->setPosition(pos);
        bRet = true;
    } while (0);
    return bRet;
}

int AStartPath::getFScore() const
{
    return this->getGScore() + this->getHScore();
}

int AStartPath::calcHScore(const Vec2& fromCoord, const Vec2& toCoord)
{
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}

int AStartPath::calcGScore(const AStartPath* fromStep, const AStartPath* toStep)
{
	return ((fromStep->getPosition().x != toStep->getPosition().x)
		&& (fromStep->getPosition().y != toStep->getPosition().y)) ? 14 : 10;
}

int AStartPath::getStepIndex(const Vector<AStartPath* >& steps, const AStartPath* step)
{
	for (ssize_t i = 0; i < steps.size(); ++i)
	{
		if (steps.at(i)->isEqual(step))
		{
			return i;
		}
	}
	return -1;
}

PointArray* AStartPath::walkAbleCoord(const Vec2& tileCoord, RoleType roleType, Vec2 targetPoint, bool ingnoreColl) const
{
	PointArray* pointArray = nullptr;
	if (roleType == RoleArrow || roleType == RoleAssassin)
	{
		pointArray = PointArray::create(4);

		// 上
		Point p(tileCoord.x, tileCoord.y - 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 左
		p.setPoint(tileCoord.x - 1, tileCoord.y);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 下
		p.setPoint(tileCoord.x, tileCoord.y + 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 右
		p.setPoint(tileCoord.x + 1, tileCoord.y);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}
	} 
	else
	{
		pointArray = PointArray::create(8);
		// 上
		Point p(tileCoord.x, tileCoord.y - 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 左
		p.setPoint(tileCoord.x - 1, tileCoord.y);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 下
		p.setPoint(tileCoord.x, tileCoord.y + 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 右
		p.setPoint(tileCoord.x + 1, tileCoord.y);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 左上
		p.setPoint(tileCoord.x - 1, tileCoord.y - 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 左下
		p.setPoint(tileCoord.x - 1, tileCoord.y + 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 右上
		p.setPoint(tileCoord.x + 1, tileCoord.y - 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}

		// 右下
		p.setPoint(tileCoord.x + 1, tileCoord.y + 1);
		if (this->isRoleTileCoord(p, targetPoint, ingnoreColl) && this->isWallTileCoord(p))
		{
			pointArray->addControlPoint(p);
		}
	}
	return pointArray;
}

bool AStartPath::isWallTileCoord(const Vec2& point) const
{
	if ( point.x < 0 || point.y < 0 || point.x > DataManager::getInstance()->tileMapWidth || point.y > DataManager::getInstance()->tileMapHeight)
	{
		return false;
	}
	else
	{
//		TODO 这个位置添加地图上障碍物的判断
		return true;
	}
}

bool AStartPath::isRoleTileCoord(const Vec2& point, const Vec2 rolePoint, bool& ingnoreColl) const
{
	bool bRet = true;
	if ( ingnoreColl )
	{
		bRet = true;
	}
	else
	{
		auto roleInfo = DataManager::getInstance()->getRoleInfo();
		// 移除点击点的角色
		for (unsigned int i = 0; i < roleInfo.size(); i++)
		{
			auto role = (Hero*)roleInfo[i];
			if ( role->getRoleCoord() == rolePoint )
			{
				roleInfo.erase(roleInfo.begin() + i);
			}
		}
		for (unsigned int i = 0; i < roleInfo.size(); i++)
		{
			auto role = (Hero*)roleInfo[i];

			if (role->getRoleCoord() == point)
			{
				bRet = false;
			}
		}
	}	
	return bRet;
}

bool AStartPath::isEqual(const AStartPath *other) const
{
    return this->getPosition() == other->getPosition();
}

string AStartPath::getDescription() const
{
    return StringUtils::format("pos=[%.0f;%.0f]  g=%d  h=%d  f=%d",
                               this->getPosition().x, this->getPosition().y,
                               this->getGScore(), this->getHScore(), this->getFScore());
}