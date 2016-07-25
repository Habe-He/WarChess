#ifndef _Role_Def_
#define _Role_Def_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

enum RoleType
{
	RoleAssassin,		// �̿�
	RoleKrazySoldier,	// ��ս
	RoleKing,			// ����
	RoleArrow,			// ������
	RoleSoldier,		// ��ս
	
	RoleMax,			// ��ɫ�������ֵ
};

enum RoleDirection      // ��ɫ�ķ���
{
	D_RightDown,
	D_Down,
	D_LeftDown,
	D_Left,
	D_LeftUp,
	D_Up,
	D_RightUp,
	D_Right,

	D_Max,              // ��ɫ�������ֵ
};

enum RoleSkill          // ��ɫ����
{
	S_Standby,          // ����
	S_Move,             // �ƶ�
	S_Sprint,           // ���
	S_Attack,           // ����
	S_Hit,              // �ܻ�
	S_Death,            // ����
	S_Win,              // ʤ��
	S_Failure,          // ʧ��

	S_Max,              // �������ֵ
};

enum RolePossess        // ��ɫ���з�
{
	P_MainLand,			// ��½
	P_Geocenter,		// ����
	P_Sky,				// ���

	P_Max,				// ��Ӫ���ֵ
};

struct RolePropty       // ��ɫ����
{
	float attack;       // ����
	float HP;           // Ѫ��
	float agility;      // ����
	float speed;        // �ƶ��ٶ�
};

// �������������ַ��� ���ݽ�ɫö��
const string nameString[] = 
{ 
	"CiKe", "KrazySoldier", "King", "GongJian", "Soldier"
};

const string chinaName[] = 
{
	"�̿�", "��ս", "����", "����", "��ս"
};

const int RoleAHA[5][4] = 
{
	// ����������ֵ�����ݡ��ٶ�
	{ 5,  3,  10, 1 },		// �̿�
	{ 2,  5,   4, 1 },		// ��ս
	{ 4, 12,   7, 1 },		// ����
	{ 3,  1,  10, 1 },		// ����
	{ 3,  6,   5, 1 },		// ��ս
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