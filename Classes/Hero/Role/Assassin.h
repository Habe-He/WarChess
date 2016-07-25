#ifndef _Role_Assassin_
#define _Role_Assassin_

#include "Hero/Hero.h"

class Assassin : public Hero
{
public:
	Assassin(void);
	virtual ~Assassin(void);

	virtual bool init();

	virtual void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);

	virtual void hurt(float _damage);								// 受到伤害
	virtual void destry();											// 死亡
	virtual Rect collideRect();										// 角色框
	virtual void releaseSkill();									// 释放技能

	CREATE_FUNC(Assassin);
};
#endif