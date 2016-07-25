#pragma once

#include "cocos2d.h"
using namespace std;
#include "Hero/RoleDef.h"


// 角色属性
struct RoleProprty
{
	int sR_ID;							// 唯一标识ID
	char sR_ChinaName[20];				// 中文名称
	char sR_EnName[20];					// 中文拼音
	RolePossess sR_RolePossess;			// 阵营
	RoleType sR_RoleType;				// 职业
	int sR_Attack;						// 攻击力
	int sR_HP;							// 生命值
	int sR_Agility;						// 敏捷
	int sR_Marshal;						// 统帅
};