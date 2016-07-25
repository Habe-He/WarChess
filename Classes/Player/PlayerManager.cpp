#include "PlayerManager.h"

PlayerManager* PlayerManager::mpPlayerInstance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	if ( !mpPlayerInstance )
	{
		mpPlayerInstance = new PlayerManager();
	}
	return mpPlayerInstance;
}

void PlayerManager::removeRemotePlayer()
{
	if ( m_remotePlayer )
	{
		delete m_remotePlayer;
		m_remotePlayer = nullptr;
	}
}

void PlayerManager::createPlayer(const char* name, bool local)
{
	auto player = new Player();
	player->setLocal(local);
	player->setName(name);

	if ( local )
	{
		if (m_localPlayer)
		{
			delete m_localPlayer;
			m_localPlayer = nullptr;
		}
		m_localPlayer = player;
	}
	else
	{
		if (m_remotePlayer)
		{
			delete m_remotePlayer;
			m_remotePlayer = nullptr;
		}
		m_remotePlayer = player;
	}
}

Player* PlayerManager::localPlayer()
{
	return m_localPlayer;
}

Player* PlayerManager::remotePlayer()
{
	return m_remotePlayer;
}