#ifndef _Hero_king_
#define _Hero_king_

#include "cocos2d.h"
USING_NS_CC;

#include "Hero/Hero.h"

class King : public Hero
{
public:
	King(void){};
	~King(void){};

	virtual bool init();

	void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);

	void isAnimationDone(Armature *armature, MovementEventType movementType, const std::string& movementID);

	virtual void hurt(float _damage);								// 受到伤害
	virtual void destry();											// 死亡
	virtual Rect collideRect();										// 角色框
	virtual void releaseSkill();									// 释放技能

	CREATE_FUNC(King);
};
#endif