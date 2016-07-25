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

	virtual void hurt(float _damage);								// �ܵ��˺�
	virtual void destry();											// ����
	virtual Rect collideRect();										// ��ɫ��
	virtual void releaseSkill();									// �ͷż���

	CC_SYNTHESIZE(float, attackDelat, AttackDelat);					// ��������ӳ�

	CREATE_FUNC(GongJian);
};
#endif