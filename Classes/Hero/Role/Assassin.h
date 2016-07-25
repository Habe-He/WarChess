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

	virtual void hurt(float _damage);								// �ܵ��˺�
	virtual void destry();											// ����
	virtual Rect collideRect();										// ��ɫ��
	virtual void releaseSkill();									// �ͷż���

	CREATE_FUNC(Assassin);
};
#endif