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

	virtual void hurt(float _damage);								// �ܵ��˺�
	virtual void destry();											// ����
	virtual Rect collideRect();										// ��ɫ��
	virtual void releaseSkill();									// �ͷż���

	CREATE_FUNC(Soldier);
};
#endif