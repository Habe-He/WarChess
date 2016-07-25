#ifndef _Game_Def_
#define _Game_Def_

const string AorrwNotCall = "passCurRole";			// �����ֹ���������
const string ArrowCounter = "ArrowCounter";			// �����ַ���
const string PassTouch	  = "PassTouchTrue";		// ������ť�Ƿ�ɵ��

enum NodeZorder
{
	SpriteBg = -4,			// ����ͼƬ
	TileMapZ = -3,			// ��Ƭ��ͼ
	TipsRolePath = -2,		// ��ɫ�����ƶ�·��
	UILayer = 9999,			// ���ϲ�UI		
};

enum RoleAttackCounter
{
	ArrowAttackRole,		// �����ֹ�������
	ArrowCounterRole,		// �����ֶԹ����ֵķ���
	ArrowCounterRoleA,		// �����ַ����ص�
	ArrowAttackRoleB,		// �����ֹ�����ɫ����ͨ��ɫ�ص�
	ComHittedRole,			// ��ͨ��ɫ�ܻ�
	ComCounterRole,			// ��ͨ��ɫ����
	ComCounterRoleA,		// ��ͨ��ɫ�����ص�
};

enum MoveData
{
	M_Death,				// ������ɫ�����ƶ�
	M_CallMove,				// �ƶ������λ��
	M_JumpBack,				// ��ɫ����
	M_Standy,				// վ��
	M_R2Hitt,				// ��ɫ2 ������ɫ1 ����ɫ1�ܻ��������ԭ�ȸ���
	M_Arrow_Counter,		// �����ַ������
	M_Nothing,
	M_Fuck,
	M_Arrow_Attack_Death,	// �����ֹ�������������ռ�ݸ���
};

enum BottomButtom			// �ײ���ť˳��
{
	Btn_Attack,
	Btn_HP,
	Btn_Agility,
};

// ��ɫͷ������
enum OrderRole
{
	FirstOrder,				// ��һ������
	ComOrder,				// ���ߺ������
	InsertOrder,			// ��������
	DeathOrder,				// �н�ɫ����������
};
#endif