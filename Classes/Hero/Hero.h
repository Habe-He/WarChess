#ifndef _Hero_Sprite_
#define _Hero_Sprite_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio;
using namespace ui;

#include "RoleDef.h"

const int SpriteFlag = 100;
const float FontSize = 10.0f;


class Hero : public Node
{
public:
	Hero(void);
	virtual ~Hero(void);

	static Hero* createRole(RoleType roleType, RolePossess rolePossess);

	bool initHero(RoleType roleType, RolePossess rolePossess);

	void setRoleCoord(Vec2 coord);                                                              // ������������

	Vec2 getRoleCoord();                                                                        // ��ȡ�����ڵ�ͼ�����λ��

	void setRoleHDA(float attack, float hp, float agility, string name);                        // ���� ������Ѫ��������

	void updateHDA(float attack, float hp, float agility);                                      // ���½�ɫ����

	float getRoleHP();                                                                          // ��ȡ��ɫ����ֵ
	float getRoleAttack();                                                                      // ��ȡ��ɫ�Ĺ�����
	float getRoleAgility();                                                                     // ��ȡ��ɫ������

	void setLabelVisable(bool isVisable);						                                // ���ý�ɫ��ǩ�Ƿ���ʾ

	virtual void setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction);              // �������﷽��he����

	RoleDirection getRoleDirection();                                                           // ��ȡ��ɫ�ķ�����߶���

	RoleType getRoleType();                                                                     // ��ȡ��ɫ����

	string getRoleName(RoleType roleType);                                                      // ��ȡ��ɫ����

	RolePossess getRolePossess();                                                               // ��ȡ��ɫ����Ӫ

	void setSkinColor(int colorIndex);															// ������ɫ
	int getSkinColor();

	virtual void hurt(float _damage);                                                           // �ܵ��˺�
	virtual void destry();                                                                      // ����
	virtual Rect collideRect();                                                                 // ��ɫ��
	virtual void releaseSkill();                                                                // �ͷż���
	bool isIngnoreCollide();																	// �Ƿ����ӽ�ɫ��ײ

	CC_SYNTHESIZE(int, reaction, Reaction);														// ��ɫ��Ӧ��Χ
	CC_SYNTHESIZE(int, roleid, RoleID);															// ��ɫԭʼ�ɣ�
	CC_SYNTHESIZE(float, roleSpeed, RoleSpeed);                                                 // ��ɫ�ƶ��ٶȱ���
	CC_SYNTHESIZE(float, attackTime, AttackTime);												// ����ʱ��
	CC_SYNTHESIZE(float, hittedTime, HittedTime);												// �ܻ�ʱ��
	CC_SYNTHESIZE(float, deathTime, DeathTime);													// ����ʱ��
	CC_SYNTHESIZE(float, counterTime, CounterTime);												// ����ʱ����

	CC_SYNTHESIZE(Vec2, firstPoint, FirstPoint);												// ��¼��ʼ�ܶ�ʱ����
	CC_SYNTHESIZE(RoleDirection, roleDir, RoleDir);												// ��¼�ܶ�����

	RoleSkill getRoleState();
	
protected:
	RoleSkill		_roleSkill;		// ��ɫ��ǰִ�еļ���
	RolePropty		_rolePropty;	// ��ɫ����
	RoleDirection	_direction;		// ��ɫ�ķ������ִ�еĶ���
	Armature*		_armature;		// ��ɫ����
	RoleType		_roleType;		// ��ɫ����
	Vec2			_roleCoord;		// ��ɫ�ڵ�ͼ��λ��
	RolePossess		_rolePossess;	// ��ɫ����Ӫ
	vector<int >	_vDir;   		// ��������
	static Hero* mpHero;
	int m_colorInd;					// ��ɫ��ɫ
};
#endif