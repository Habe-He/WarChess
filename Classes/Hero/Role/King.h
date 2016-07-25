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

	virtual void hurt(float _damage);								// �ܵ��˺�
	virtual void destry();											// ����
	virtual Rect collideRect();										// ��ɫ��
	virtual void releaseSkill();									// �ͷż���

	CREATE_FUNC(King);
};
#endif