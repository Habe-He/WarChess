#ifndef _Soldier_Hero_
#define _Soldier_Hero_

#include "cocos2d.h"
USING_NS_CC;

#include "Hero/Hero.h"

class Soldier : public Hero
{
public:
	Soldier(void);
	virtual ~Soldier(void);

	virtual bool init();

	virtual void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);

	virtual void hurt(float _damage);								// 受到伤害
	virtual void destry();											// 死亡
	virtual Rect collideRect();										// 角色框
	virtual void releaseSkill();									// 释放技能

	CREATE_FUNC(Soldier);
};
#endif