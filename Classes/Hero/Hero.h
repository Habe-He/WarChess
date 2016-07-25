#ifndef _Hero_Sprite_
#define _Hero_Sprite_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio;
using namespace ui;

#include "RoleDef.h"

const int SpriteFlag = 100;
const float FontSize = 10.0f;


class Hero : public Node
{
public:
	Hero(void);
	virtual ~Hero(void);

	static Hero* createRole(RoleType roleType, RolePossess rolePossess);

	bool initHero(RoleType roleType, RolePossess rolePossess);

	void setRoleCoord(Vec2 coord);                                                              // 设置人物坐标

	Vec2 getRoleCoord();                                                                        // 获取人物在地图上面的位置

	void setRoleHDA(float attack, float hp, float agility, string name);                        // 设置 攻击、血量、敏捷

	void updateHDA(float attack, float hp, float agility);                                      // 更新角色属性

	float getRoleHP();                                                                          // 获取角色生命值
	float getRoleAttack();                                                                      // 获取角色的攻击力
	float getRoleAgility();                                                                     // 获取角色的敏捷

	void setLabelVisable(bool isVisable);						                                // 设置角色标签是否显示

	virtual void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);              // 设置人物方向he技能

	RoleDirection getRoleDirection();                                                           // 获取角色的方向或者动作

	RoleType getRoleType();                                                                     // 获取角色类型

	string getRoleName(RoleType roleType);                                                      // 获取角色名称

	RolePossess getRolePossess();                                                               // 获取角色的阵营

	void setSkinColor(int colorIndex);															// 设置颜色
	int getSkinColor();

	virtual void hurt(float _damage);                                                           // 受到伤害
	virtual void destry();                                                                      // 死亡
	virtual Rect collideRect();                                                                 // 角色框
	virtual void releaseSkill();                                                                // 释放技能
	bool isIngnoreCollide();																	// 是否无视角色碰撞

	CC_SYNTHESIZE(int, reaction, Reaction);														// 角色感应范围
	CC_SYNTHESIZE(int, roleid, RoleID);															// 角色原始ＩＤ
	CC_SYNTHESIZE(float, roleSpeed, RoleSpeed);                                                 // 角色移动速度变量
	CC_SYNTHESIZE(float, attackTime, AttackTime);												// 攻击时间
	CC_SYNTHESIZE(float, hittedTime, HittedTime);												// 受击时间
	CC_SYNTHESIZE(float, deathTime, DeathTime);													// 死亡时间
	CC_SYNTHESIZE(float, counterTime, CounterTime);												// 反击时间间隔

	CC_SYNTHESIZE(Vec2, firstPoint, FirstPoint);												// 记录开始跑动时坐标
	CC_SYNTHESIZE(RoleDirection, roleDir, RoleDir);												// 记录跑动方向

	RoleSkill getRoleState();
	
protected:
	RoleSkill		_roleSkill;		// 角色当前执行的技能
	RolePropty		_rolePropty;	// 角色属性
	RoleDirection	_direction;		// 角色的方向或者执行的动作
	Armature*		_armature;		// 角色动画
	RoleType		_roleType;		// 角色类型
	Vec2			_roleCoord;		// 角色在地图上位置
	RolePossess		_rolePossess;	// 角色的阵营
	vector<int >	_vDir;   		// 方向数组
	static Hero* mpHero;
	int m_colorInd;					// 角色颜色
};
#endif