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

const int RoleStarFlag = 90000;				// 角色开始ID
const int RoleCHStartFlag = 80000;			// 角色特性起始ID

class DataManager
{
public:
	DataManager(void);
	virtual ~DataManager(void);

	static DataManager* getInstance();

	vector<Hero* > heroVector;                          // 存放敌我方英雄,根据获取的角色的阵营区分
	vector<Hero* > getRoleInfo();


	vector<int > roleInBattle;							// 
	vector<int > getRoleInBattle();						// 存放进入战斗的角色

	bool init();

	void sortByAgility();                               // 根据角色的敏捷度排序

	int tileMapWidth;                                   // 地图格子宽 数量
	int tileMapHeight;                                  // 地图格子高 数量

protected:
	vector<RoleProprty > dm_RolePro;					// 角色属性数组
	void rolePropryDP();								// 角色属性表解析

public:
	vector<RoleProprty > getVRolePropty();				// 角色属性


public:
	

	int tileWidth();                                    // 返回地图宽
	int tileHeight();
};
#endif