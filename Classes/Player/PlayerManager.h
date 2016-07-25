#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Player.h"

#define PLAYER_MGR() PlayerManager::getInstance()


class PlayerManager
{
public:
	PlayerManager()
	{
		m_localPlayer = nullptr;
		m_remotePlayer = nullptr;
	}
	~PlayerManager();

	static PlayerManager* getInstance();
	void createPlayer(const char* name, bool local);
	void removeRemotePlayer();

	Player* localPlayer();
	Player* remotePlayer();

private:
	static PlayerManager* mpPlayerInstance;

	Player* m_localPlayer;
	Player* m_remotePlayer;
};