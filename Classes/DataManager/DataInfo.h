#pragma once

#include "cocos2d.h"
using namespace std;
#include "Hero/RoleDef.h"


// ��ɫ����
struct RoleProprty
{
	int sR_ID;							// Ψһ��ʶID
	char sR_ChinaName[20];				// ��������
	char sR_EnName[20];					// ����ƴ��
	RolePossess sR_RolePossess;			// ��Ӫ
	RoleType sR_RoleType;				// ְҵ
	int sR_Attack;						// ������
	int sR_HP;							// ����ֵ
	int sR_Agility;						// ����
	int sR_Marshal;						// ͳ˧
};