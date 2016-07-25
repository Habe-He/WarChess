/********************************************************************
 Copyright(C), 2012-2013,
 ��  ��: RoleSpeciality.h
 ��  ��: ��ɫ����
*********************************************************************/
#pragma once

#include "cocos2d.h"
#include "Hero/Hero.h"

class RoleSpeciality
{
public:
	RoleSpeciality()
	{
		m_attackRole = nullptr;
		m_hittedRole = nullptr;
	}
	virtual ~RoleSpeciality(){}
	
	static RoleSpeciality* createSpeciality();

	void initSpeciality(Hero* attack, Hero* hitted);

	void lingHunLian();

private:
	static RoleSpeciality* mpSpeciality;
	Hero* m_attackRole;
	Hero* m_hittedRole;
};