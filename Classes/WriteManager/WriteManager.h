#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "GameManager/GameInfo.h"
#include "base/CCUserDefault.h"

static const char* nameboard_head_save_formar = "nameboardhead";
static const char* nameboard_title_save_formar = "nameboardtitle";
static const char* nameboard_shade_save_formar = "nameboardshade";

#define WriteMgr WriteManager::getInstance()

static UserDefault* pStore;

class WriteManager
{
public:
	static WriteManager* getInstance();

	void saveNameBoard(NameBoardType type, int Id);

	void flush();

	void setIntegerForKey(const char* pKey, int value);

	std::string encodeData(const char* value);
};