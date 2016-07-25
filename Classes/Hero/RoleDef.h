#ifndef _Role_Def_
#define _Role_Def_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

enum RoleType
{
	RoleAssassin,		// 刺客
	RoleKrazySoldier,	// 狂战
	RoleKing,			// 国王
	RoleArrow,			// 弓箭手
	RoleSoldier,		// 防战
	
	RoleMax,			// 角色类型最大值
};

enum RoleDirection      // 角色的方向
{
	D_RightDown,
	D_Down,
	D_LeftDown,
	D_Left,
	D_LeftUp,
	D_Up,
	D_RightUp,
	D_Right,

	D_Max,              // 角色方向最大值
};

enum RoleSkill          // 角色技能
{
	S_Standby,          // 待机
	S_Move,             // 移动
	S_Sprint,           // 冲刺
	S_Attack,           // 攻击
	S_Hit,              // 受击
	S_Death,            // 死亡
	S_Win,              // 胜利
	S_Failure,          // 失败

	S_Max,              // 技能最大值
};

enum RolePossess        // 角色所有方
{
	P_MainLand,			// 大陆
	P_Geocenter,		// 地心
	P_Sky,				// 天空

	P_Max,				// 阵营最大值
};

struct RolePropty       // 角色属性
{
	float attack;       // 攻击
	float HP;           // 血量
	float agility;      // 敏捷
	float speed;        // 移动速度
};

// 定义人物名称字符串 根据角色枚举
const string nameString[] = 
{ 
	"CiKe", "KrazySoldier", "King", "GongJian", "Soldier"
};

const string chinaName[] = 
{
	"刺客", "狂战", "国王", "射手", "防战"
};

const int RoleAHA[5][4] = 
{
	// 攻击、生命值、敏捷、速度
	{ 5,  3,  10, 1 },		// 刺客
	{ 2,  5,   4, 1 },		// 狂战
	{ 4, 12,   7, 1 },		// 国王
	{ 3,  1,  10, 1 },		// 射手
	{ 3,  6,   5, 1 },		// 防战
};

const string SkillName[S_Max][D_Max] =
{
	{ "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8" },     // 1
	{ "Y1", "Y2", "Y3", "Y4", "Y5", "Y6", "Y7", "Y8" },     // 2
	{ "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8" },     // 3
	{ "G1", "G2", "G3", "G4", "G5", "G6", "G7", "G8" },     // 4
	{ "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8" },     // 5
	{ "W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8" },     // 6
	{ "L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8" },     // 7
	{ "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8" },     // 8
};

#endif