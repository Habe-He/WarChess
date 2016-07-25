#ifndef _Game_Def_
#define _Game_Def_

const string AorrwNotCall = "passCurRole";			// 弓箭手弓箭到敌人
const string ArrowCounter = "ArrowCounter";			// 弓箭手反击
const string PassTouch	  = "PassTouchTrue";		// 跳过按钮是否可点击

enum NodeZorder
{
	SpriteBg = -4,			// 背景图片
	TileMapZ = -3,			// 瓦片地图
	TipsRolePath = -2,		// 角色可以移动路线
	UILayer = 9999,			// 最上层UI		
};

enum RoleAttackCounter
{
	ArrowAttackRole,		// 弓箭手攻击敌人
	ArrowCounterRole,		// 弓箭手对弓箭手的反击
	ArrowCounterRoleA,		// 弓箭手反击回调
	ArrowAttackRoleB,		// 弓箭手攻击角色，普通角色回调
	ComHittedRole,			// 普通角色受击
	ComCounterRole,			// 普通角色反击
	ComCounterRoleA,		// 普通角色反击回调
};

enum MoveData
{
	M_Death,				// 攻击角色死亡移动
	M_CallMove,				// 移动到点击位置
	M_JumpBack,				// 角色后跳
	M_Standy,				// 站立
	M_R2Hitt,				// 角色2 反击角色1 ，角色1受击完成跳回原先格子
	M_Arrow_Counter,		// 弓箭手反击完成
	M_Nothing,
	M_Fuck,
	M_Arrow_Attack_Death,	// 弓箭手攻击敌人死亡，占据格子
};

enum BottomButtom			// 底部按钮顺序
{
	Btn_Attack,
	Btn_HP,
	Btn_Agility,
};

// 角色头像排序
enum OrderRole
{
	FirstOrder,				// 第一次排序
	ComOrder,				// 行走后的排序
	InsertOrder,			// 插入排序
	DeathOrder,				// 有角色死亡的排序
};
#endif