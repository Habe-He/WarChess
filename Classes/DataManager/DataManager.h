#ifndef _DataManager_
#define _DataManager_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "Hero/Hero.h"
#include "DataInfo.h"

#include "json/rapidjson.h"
#include "json/document.h"

#define DataMgr() DataManager::getInstance()

const int RoleStarFlag = 90000;				// ��ɫ��ʼID
const int RoleCHStartFlag = 80000;			// ��ɫ������ʼID

class DataManager
{
public:
	DataManager(void);
	virtual ~DataManager(void);

	static DataManager* getInstance();

	vector<Hero* > heroVector;                          // ��ŵ��ҷ�Ӣ��,���ݻ�ȡ�Ľ�ɫ����Ӫ����
	vector<Hero* > getRoleInfo();


	vector<int > roleInBattle;							// 
	vector<int > getRoleInBattle();						// ��Ž���ս���Ľ�ɫ

	bool init();

	void sortByAgility();                               // ���ݽ�ɫ�����ݶ�����

	int tileMapWidth;                                   // ��ͼ���ӿ� ����
	int tileMapHeight;                                  // ��ͼ���Ӹ� ����

protected:
	vector<RoleProprty > dm_RolePro;					// ��ɫ��������
	void rolePropryDP();								// ��ɫ���Ա����

public:
	vector<RoleProprty > getVRolePropty();				// ��ɫ����


public:
	

	int tileWidth();                                    // ���ص�ͼ��
	int tileHeight();
};
#endif