#include "RoleSpeciality.h"

RoleSpeciality* RoleSpeciality::mpSpeciality = nullptr;

RoleSpeciality* RoleSpeciality::createSpeciality()
{
	if ( !mpSpeciality )
	{
		mpSpeciality = new RoleSpeciality();
	}
	return mpSpeciality;
}

void RoleSpeciality::initSpeciality(Hero* attack, Hero* hitted)
{
	m_attackRole = attack;
	m_hittedRole = hitted;
	// 根据角色特新筛选
}
