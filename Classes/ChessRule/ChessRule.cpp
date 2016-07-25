#include "ChessRule.h"

#include "DataManager/DataManager.h"
#include "Hero/Hero.h"

ChessRule* ChessRule::mpChessRule = NULL;

ChessRule::ChessRule()
{
	bEatChess = false;
}

ChessRule::~ChessRule()
{

}

ChessRule* ChessRule::getInstance()
{
	if (!mpChessRule)
	{
		mpChessRule = new ChessRule();
	}

	return mpChessRule;
}

bool ChessRule::judgeRule(Hero* _hero, cocos2d::Vec2 originalPoint, cocos2d::Vec2 targetPoint)
{
	bool bRet = false;
	switch (_hero->getRoleType())
	{
	case RoleSoldier:
		bRet = judgeSoldier(_hero, originalPoint, targetPoint);
		break;
	case RoleAssassin:
		bRet = judgeAssassin(_hero, originalPoint, targetPoint);
		break;
	case RoleArrow:
		bRet = judgeArrow(_hero, originalPoint, targetPoint);
		break;
	case RoleKrazySoldier:
		bRet = judgeKrazySoldier(_hero, originalPoint, targetPoint);
		break;
	case RoleKing:
		bRet = judgeKing(_hero, originalPoint, targetPoint);
		break;
	}
	return bRet;
}

bool ChessRule::judgeAssassin(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint)
{
	bEatChess = false;
	auto offsetX = abs(targetPoint.x - originalPoint.x);
	auto offsetY = abs(targetPoint.y - originalPoint.y);

	if (offsetX > 3 || offsetY > 3 || (targetPoint == originalPoint))
	{
		return false;
	}

	int pathXY[8][2] =
	{
		{  1, -2 }, {  2, -1 },
		{  2,  1 }, {  1,  2 },
		{ -1,  2 }, { -2,  1 },
		{ -2, -1 }, { -1, -2 },
	};

	auto infoRole = DataManager::getInstance()->getRoleInfo();

	for (int i = 0; i < 8; i++)
	{
		if (targetPoint == Vec2(originalPoint + Vec2(pathXY[i][0], pathXY[i][1])))
		{
			for (unsigned int z = 0; z < infoRole.size(); z++)
			{
				auto info = (Hero*)infoRole[z];
				if (Vec2(originalPoint + Vec2(pathXY[i][0], pathXY[i][1])) == info->getRoleCoord())
				{
					if (info->getSkinColor() != _hero->getSkinColor())
					{
						bEatChess = true;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool ChessRule::judgeKing(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint)
{
	bEatChess = false;
	auto offX = abs(targetPoint.x - originalPoint.x);
	auto offY = abs(targetPoint.y - originalPoint.y);

	if ((offX > 2) || (offY > 2) || (targetPoint == originalPoint))
	{
		return false;
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

	auto roleInfo = DataManager::getInstance()->getRoleInfo();
	for (int i = 0; i < 8; i++)
	{
		if (targetPoint == Vec2(originalPoint + Vec2(pathXY[i][0], pathXY[i][1])))
		{
			for (unsigned int z = 0; z < roleInfo.size(); z++)
			{
				auto info = (Hero*)roleInfo[z];
				if (Vec2(originalPoint + Vec2(pathXY[i][0], pathXY[i][1])) == info->getRoleCoord())
				{
					if (info->getSkinColor() != _hero->getSkinColor())
					{
						bEatChess = true;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool ChessRule::judgeSoldier(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint)
{
	bEatChess = false;
	bool bRet = false;
	auto offsetX = abs(targetPoint.x - originalPoint.x);
	auto offsetY = abs(targetPoint.y - originalPoint.y);

	if (offsetX > 2 || offsetY > 2 || (targetPoint == originalPoint))
	{
		return false;
	}

	int pathT[9][2] = 
	{
		{ -1, -1 }, // 8
		{  0, -1 },
		{  1, -1 }, // 2
		{  1,  0 },
		{  1,  1 }, // 4
		{  0,  1 }, // 5
		{ -1,  1 }, // 6
		{ -1,  0 },
		{ -1, -1 }, // 8
	};

	int pathF[4][2] =
	{
		{  0, -2 }, // 1
		{  2,  0 }, // 3
		{  0,  2 }, // 5
		{ -2,  0 }, // 7
	};
	bool isPathT[9] = { false };  // 为true的时候表示
	auto roleInfo = DataManager::getInstance()->getRoleInfo();

	for ( auto itor : roleInfo )
	{
		for (int i = 0; i < 9; ++ i)
		{
			if (itor->getRoleCoord() == Vec2( originalPoint + Vec2( pathT[i][0], pathT[i][1] ) ) )
			{
				isPathT[i] = true;
			}
		}
	}

	for (int i = 0; i < 8; ++ i)
	{
		if (targetPoint == Vec2( originalPoint + Vec2( pathT[i][0], pathT[i][1] ) ) )
		{
			for ( auto itor : roleInfo )
			{
				if ( targetPoint == itor->getRoleCoord() )
				{
					if (_hero->getSkinColor() != itor->getSkinColor())
					{
						bEatChess = true;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			bRet = true;
		}
	}

	auto point = _hero->getRoleCoord();
	auto tileWidth = DataManager::getInstance()->tileWidth();
	auto tileHeight = DataManager::getInstance()->tileHeight();
	for (int i = 0; i < 4; i ++)
	{
		if ( targetPoint == Vec2( originalPoint + Vec2( pathF[i][0], pathF[i][1] ) ) )
		{
			if (i == 0)
			{
				if ((originalPoint.x == 0 && originalPoint.y == tileHeight)
					|| (originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
				else if ((originalPoint.x == tileWidth && originalPoint.y == tileWidth)
					|| (originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
					{
						bRet = true;
					}
				}
				else if (
					(originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight)
					|| (originalPoint.x > 0 && originalPoint.y > 0 && originalPoint.x < tileWidth && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
			}
			else if (i == 1)
			{
				if ((originalPoint.x == 0 && originalPoint.y == 0)
					|| (originalPoint.y == 0 && originalPoint.x > 0 && originalPoint.x < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
				else if ((originalPoint.x == 0 && originalPoint.y == tileHeight)
					|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
					{
						bRet = true;
					}
				}
				else if (
					(originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight)
					|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
			}
			else if (i == 2)
			{
				if ((originalPoint.x == 0 && originalPoint.y == 0)
					|| (originalPoint.x == 0 && originalPoint.y > 0 && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
					{
						bRet = true;
					}
				}
				else if ((originalPoint.x == tileWidth && originalPoint.y == 0)
					|| (originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
				else if ((originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == 0)
					|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
			}
			else if (i == 3)
			{
				if ((originalPoint.x == tileWidth && originalPoint.y == 0)
					|| (originalPoint.y == 0 && originalPoint.x > 0 && originalPoint.x < tileWidth))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1])
					{
						bRet = true;
					}
				}
				else if ((originalPoint.x == tileWidth && originalPoint.y == tileHeight)
					|| (originalPoint.x > 0 && originalPoint.x < tileWidth && originalPoint.y == tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
				else if ((originalPoint.x == tileWidth && originalPoint.y > 0 && originalPoint.y < tileHeight)
					|| (originalPoint.x > 0 && originalPoint.y > 0 && originalPoint.x < tileWidth && originalPoint.y < tileHeight))
				{
					for (auto itor : roleInfo)
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if (_hero->getSkinColor() != itor->getSkinColor())
							{
								if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
								{
									bEatChess = true;
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
					if (!isPathT[i * 2] || !isPathT[i * 2 + 1] || !isPathT[i * 2 + 2])
					{
						bRet = true;
					}
				}
			}			
		}
	}
	return bRet;
}

bool ChessRule::judgeArrow(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint)
{
	bEatChess = false;
	bool bRet = false; // 是否可以行走
	auto offsetX = abs(targetPoint.x - originalPoint.x);
	auto offsetY = abs(targetPoint.y - originalPoint.y);

	if (offsetX > 4 || offsetY > 4 || (targetPoint == originalPoint))
	{
		return false;
	}

	int pathXY[16][2] =
	{
		{  0, -1 }, {  0, -2 }, {  0, -3 }, {  0, -4 }, // 1
		{  1,  0 }, {  2,  0 }, {  3,  0 }, {  4,  0 }, // 3
		{  0,  1 }, {  0,  2 }, {  0,  3 }, {  0,  4 }, // 5
		{ -1,  0 }, { -2,  0 }, { -3,  0 }, { -4,  0 }, // 7

	};

	bool isPath[16] = { false };
	auto roleInfo = DataManager::getInstance()->getRoleInfo();

	for ( auto itor : roleInfo )
	{
		for (int i = 0; i < 4; ++ i)
		{
			for (int k = 0; k < 4; ++ k)
			{
				if (itor->getRoleCoord() == Vec2(originalPoint + Vec2(pathXY[i * 4 + k][0], pathXY[i * 4 + k][1])))
				{
					isPath[i * 4 + k] = true;
				}
			}
		}
	}

	for (int i = 0; i < 4; ++ i)
	{
		for (int k = 0; k < 4; ++ k)
		{
			if (targetPoint == Vec2(originalPoint + Vec2(pathXY[i * 4 + k][0], pathXY[i * 4 + k][1])))
			{
				for (auto itor : roleInfo)
				{
					if ( k == 0 )
					{
						if (targetPoint == itor->getRoleCoord())
						{
							if ( _hero->getSkinColor() != itor->getSkinColor())
							{
								bEatChess = true;
								return true;
							}
						}
						else if ( !isPath[ i * 4 + k ] )
						{
							bRet = true;
						}
					}
					else if ( k == 1 )
					{
						if ( targetPoint == itor->getRoleCoord() )
						{
							if ( !isPath[ i * 4 + k - 1 ] )
							{
								if (_hero->getSkinColor() != itor->getSkinColor())
								{
									bEatChess = true;
									return true;
								}								
							}
						}
						else if (!isPath[i * 4 + k - 1] && !isPath[i * 4 + k])
						{
							bRet = true;
						}
					}
					else if ( k == 2 )
					{
						if ( targetPoint == itor->getRoleCoord())
						{
							if ( !isPath[ i * 4 + k - 2] && !isPath[ i * 4 + k - 1 ] )
							{
								if (_hero->getSkinColor() != itor->getSkinColor())
								{
									bEatChess = true;
									return true;
								}								
							}
						}
						else if (!isPath[i * 4 + k - 2] && !isPath[i * 4 + k - 1] && !isPath[ i * 4 + k ] )
						{
							bRet = true;
						}
					}
					else if ( k == 3 )
					{
						if ( targetPoint == itor->getRoleCoord() )
						{
							if (!isPath[i * 4 + k - 3] && !isPath[i * 4 + k - 2] && !isPath[i * 4 + k - 1])
							{
								if (_hero->getSkinColor() != itor->getSkinColor())
								{
									bEatChess = true;
									return true;
								}							
							}
						}
						else if (!isPath[i * 4 + k - 3] && !isPath[i * 4 + k - 2] && !isPath[i * 4 + k - 1] && !isPath[i * 4 + k] )
						{
							bRet = true;
						}
					}
				}
			}
		}
	}
	return bRet;
}

bool ChessRule::judgeKrazySoldier(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint)
{
	bEatChess = false;
	auto offsetX = abs(targetPoint.x - originalPoint.x);
	auto offsetY = abs(targetPoint.y - originalPoint.y);

	if (offsetX > 4 || offsetY > 4 || (targetPoint == originalPoint))
	{
		return false;
	}
	int pathThreeXY[16][2] =
	{
		{  1,  -1 }, {  2, -2 }, {  3, -3 }, {  4, -4 }, // 2
		{  1,   1 }, {  2,  2 }, {  3,  3 }, {  4,  4 }, // 4
		{ -1,   1 }, { -2,  2 }, { -3,  3 }, { -4,  4 }, // 6
		{ -1,  -1 }, { -2, -2 }, { -3, -3 }, { -4, -4 }, // 8

	};

	auto roleInfo = DataManager::getInstance()->getRoleInfo();
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			if (targetPoint == Vec2(originalPoint + Vec2(pathThreeXY[i * 4 + k][0], pathThreeXY[i * 4 + k][1])))
			{
				for (unsigned int z = 0; z < roleInfo.size(); z++)
				{
					auto info = (Hero*)roleInfo[z];

					if (k == 0)
					{
						if (Vec2(originalPoint + Vec2(pathThreeXY[i * 4 + k][0], pathThreeXY[i * 4 + k][1])) == info->getRoleCoord())
						{
							if (info->getSkinColor() != _hero->getSkinColor())
							{
								bEatChess = true;
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					else if (k == 1)
					{
						if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k - 1][0], originalPoint.y + pathThreeXY[i * 4 + k - 1][1]) == info->getRoleCoord())
						{
							return false;
						}
						else if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k][0], originalPoint.y + pathThreeXY[i * 4 + k][1]) == info->getRoleCoord())
						{
							if (info->getSkinColor() != _hero->getSkinColor())
							{
								bEatChess = true;
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					else if (k == 2)
					{
						if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k - 1][0], originalPoint.y + pathThreeXY[i * 4 + k - 1][1]) == info->getRoleCoord())
						{
							return false;
						}
						else if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k - 2][0], originalPoint.y + pathThreeXY[i * 4 + k - 2][1]) == info->getRoleCoord())
						{
							return false;
						}
						else if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k][0], originalPoint.y + pathThreeXY[i * 4 + k][1]) == info->getRoleCoord())
						{
							if (info->getSkinColor() != _hero->getSkinColor())
							{
								bEatChess = true;
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					else if (k == 3)
					{
						if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k - 1][0], originalPoint.y + pathThreeXY[i * 4 + k - 1][1]) == info->getRoleCoord())
						{
							return false;
						}
						else if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k - 2][0], originalPoint.y + pathThreeXY[i * 4 + k - 2][1]) == info->getRoleCoord())
						{
							return false;
						}
						else if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k - 3][0], originalPoint.y + pathThreeXY[i * 4 + k - 3][1]) == info->getRoleCoord())
						{
							return false;
						}
						else if (Vec2(originalPoint.x + pathThreeXY[i * 4 + k][0], originalPoint.y + pathThreeXY[i * 4 + k][1]) == info->getRoleCoord())
						{
							if (info->getSkinColor() != _hero->getSkinColor())
							{
								bEatChess = true;
								return true;
							}
							else
							{
								return false;
							}
						}
					}
				}
				return true;
			}
		}
	}
	return false;
}

bool ChessRule::isEatChess()
{
	return bEatChess;
}