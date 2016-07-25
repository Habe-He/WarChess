#ifndef _GongJian_Hero_
#define _GongJian_Hero_

#include "cocos2d.h"
USING_NS_CC;

#include "Hero/Hero.h"

class GongJian : public Hero
{
public:
	GongJian(void);
	virtual ~GongJian(void);

	virtual bool init();

	virtual void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);

	virtual void hurt(float _damage);								// 受到伤害
	virtual void destry();											// 死亡
	virtual Rect collideRect();										// 角色框
	virtual void releaseSkill();									// 释放技能

	CC_SYNTHESIZE(float, attackDelat, AttackDelat);					// 射击动作延迟

	CREATE_FUNC(GongJian);
};
#endif