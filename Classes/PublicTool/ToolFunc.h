#ifndef _Tool_Func_
#define _Tool_Func_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "Hero/Hero.h"
#include "Hero/RoleDef.h"

#define SCREEN_WIDTH	960
#define SCREEN_HEIGTH	640

#define SCREEN_WIDTH_HALF	( 960 >> 1 )
#define SCREEN_HEIGHT_HALF	( 640 >> 1 )

#define TXM_MAX_SCALE 1.5f		// 地图最大放大比例
#define TXM_MIN_SCALE 0.9f		// 地图最小缩放比例

enum AttackOrCounter
{
	Attack,							// 主动攻击
	CounterBack,					// 反击
};

class ToolFunc
{
public:
	static int calcRandomNum( int _star, int _end );							// 计算随机数

	static vector<string > splitString(string str, string pattern);             // 分割字符串

	static RoleDirection calcRoleDirection(Vec2 orin, Vec2 tar);				// 计算角色方向

	static Vec2 coverToScreenCoord(TMXTiledMap* tileMap, Vec2 point);			// 地图坐标 转 屏幕坐标

	static Vec2 coverToTileCoord(TMXTiledMap* tileMap, Vec2 point);				// 屏幕坐标转 地图坐标

	static Vec2 calcRoleBackPoint(RoleDirection _roleDirection, AttackOrCounter _type);				// 角色后退位置

	static void outLineSprite(Sprite* sprite);									// 图片描边
																			
	static void graySprite(Sprite* sprite);										// 图片变为灰白色
		
	static void ungraySprite(Sprite* sprite);									// 把图片变为正常颜色

	static void vectorSort(vector<Hero* >& original, vector<Hero* >& target, int kind);
};
#endif