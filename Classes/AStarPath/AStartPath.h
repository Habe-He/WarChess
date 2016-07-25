#ifndef AStarPath_hpp
#define AStarPath_hpp

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
#include "Hero/RoleDef.h"

class AStartPath : public Ref
{
public:
    AStartPath();
    ~AStartPath();
    
    static AStartPath* getInstance(const Vec2& pos);
    
    bool initWithPosition(const Vec2& pos);
    
    int getFScore() const;
    bool isEqual(const AStartPath* other ) const;
    string getDescription() const;

	int calcHScore(const Vec2& fromCoord, const Vec2& toCoord);									// 计算移动的H值

	int calcGScore(const AStartPath* fromStep, const AStartPath* toStep);						// 计算G值

	int getStepIndex(const Vector<AStartPath* >& steps, const AStartPath* step);				// 获取当前步数索引

	/*
	参数 1： F 值最小的点坐标
	参数 2： 当前角色的类型
	参数 3： 目标点坐标
	参数 4： 当前角色是否无视角色碰撞
	*/

	PointArray* walkAbleCoord(const Vec2& tileCoord, RoleType roleType, Vec2 targetPoint, bool ingnoreColl) const;

	bool isWallTileCoord(const Vec2& point) const;												// 可移动点是否在地图上或者地图上不可移动的位置

	bool isRoleTileCoord(const Vec2& point, const Vec2 rolePoint, bool& ingnoreColl) const;												// 可移动位置是否有角色
    
    CC_SYNTHESIZE(Vec2, _position, Position);
    CC_SYNTHESIZE(int, _gScore, GScore);														// 开始点到当前的方块数量
    CC_SYNTHESIZE(int, _hScore, HScore);														// 当前点到目标点预估值
    CC_SYNTHESIZE(AStartPath*, _parent, Parent);
} ;

#endif /* AStarPath_hpp */
