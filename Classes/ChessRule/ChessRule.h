#ifndef ChessRule_h_
#define ChessRule_h_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "Hero/Hero.h"

class ChessRule : public Ref
{
public:

	ChessRule();
	virtual ~ChessRule();

	static ChessRule* getInstance();
	static ChessRule* mpChessRule;

	bool judgeRule(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint);

	bool judgeSoldier(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint);

	bool judgeArrow(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint);

	bool judgeAssassin(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint);

	bool judgeKrazySoldier(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint);

	bool judgeKing(Hero* _hero, Vec2 originalPoint, Vec2 targetPoint);

	bool isEatChess();                                                                  // 可以攻击的位置

private:
	bool bEatChess;
};

#endif
