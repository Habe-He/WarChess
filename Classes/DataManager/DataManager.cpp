#include "DataManager.h"

static DataManager* mpInstance = NULL;

DataManager::DataManager(void) :tileMapHeight(0), tileMapWidth(0)
{

}

DataManager::~DataManager(void)
{
	CC_SAFE_DELETE(mpInstance);
	mpInstance = NULL;
}

DataManager* DataManager::getInstance()
{
	if (!mpInstance)
	{
		mpInstance = new DataManager();
		mpInstance->init();
	}

	return mpInstance;
}

bool DataManager::init()
{
	return true;
}

void DataManager::sortByAgility()
{

}

vector<Hero* > DataManager::getRoleInfo()
{
	return heroVector;
}

int DataManager::tileWidth()
{
	return tileMapWidth;
}

int DataManager::tileHeight()
{
	return tileMapHeight;
}

vector<int > DataManager::getRoleInBattle()
{
	return roleInBattle;
}

void DataManager::rolePropryDP()
{
	string filename = "JsonData/RolePropty.json";
	rapidjson::Document doc;
	if (!FileUtils::getInstance()->isFileExist(filename))
	{
		CCASSERT(false, "Json File IS NOT FIND");
		return;
	} 
	string data = FileUtils::getInstance()->getStringFromFile(filename);
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	if (doc.HasParseError() || !doc.IsArray())
	{
		CCASSERT(false, "Get Json Data Error");
		return;
	}

	RoleProprty rolePropty;
	memset(&rolePropty, 0, sizeof(rolePropty));
	dm_RolePro.clear();

	for (unsigned int i = 1; i < doc.Size(); i++)
	{
		rapidjson::Value &value = doc[i];
		int a = 0;

		rolePropty.sR_ID = value[a++].GetInt();
		std::string str = value[a++].GetString();
		memset(rolePropty.sR_ChinaName, 0, sizeof(rolePropty.sR_ChinaName));
		memcpy(rolePropty.sR_ChinaName, str.c_str(), str.length());
		str = value[a++].GetString();
		memset(rolePropty.sR_EnName, 0, sizeof(rolePropty.sR_EnName));
		memcpy(rolePropty.sR_EnName, str.c_str(), str.length());

		rolePropty.sR_RolePossess = (RolePossess)value[a++].GetInt();
		rolePropty.sR_RoleType = (RoleType)value[a++].GetInt();
		rolePropty.sR_Attack = value[a++].GetInt();
		rolePropty.sR_HP = value[a++].GetInt();
		rolePropty.sR_Agility = value[a++].GetInt();
		rolePropty.sR_Marshal = value[a++].GetInt();

		dm_RolePro.push_back(rolePropty);
	}
}

vector<RoleProprty > DataManager::getVRolePropty()
{
	if ( dm_RolePro.size() == 0 )
	{
		rolePropryDP();
	}
	return dm_RolePro;
}