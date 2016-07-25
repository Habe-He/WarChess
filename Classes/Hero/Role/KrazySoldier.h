#ifndef _KrazySolider_Hero_
#define _KrazySolider_Hero_

#include "cocos2d.h"
USING_NS_CC;

#include "Hero/Hero.h"

class KrazySoldier : public Hero
{
public:
	KrazySoldier(void);
	virtual ~KrazySoldier(void);

	virtual bool init();

	virtual void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);

	void isAnimationDone(Armature *armature, MovementEventType movementType, const std::string& movementID);

	virtual void hurt(float _damage);								// 受到伤害
	virtual void destry();											// 死亡
	virtual Rect collideRect();										// 角色框
	virtual void releaseSkill();									// 释放技能

	CREATE_FUNC(KrazySoldier);
};
#endif