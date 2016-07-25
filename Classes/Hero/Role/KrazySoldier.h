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

	virtual void hurt(float _damage);								// �ܵ��˺�
	virtual void destry();											// ����
	virtual Rect collideRect();										// ��ɫ��
	virtual void releaseSkill();									// �ͷż���

	CREATE_FUNC(KrazySoldier);
};
#endif