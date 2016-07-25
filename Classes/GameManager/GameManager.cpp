#include "GameManager.h"
#include "WriteManager/WriteManager.h"

GameManager* GameManager::mpGameManager = nullptr;

GameManager::GameManager(void)
{
	m_clientServerDtTime = 0;
	_nbHead = 0;
	_nbTitle = 0;
	_nbOppoHead = 0;
	_nbOppoTitle = 0;
	_nbOppoShade = 0;
	m_mapId = 1;
	m_isServer = false;
	m_pjPing = 0;
	m_mapID = 0;
	m_roomSize = 0;
	m_colorID = 0;
	m_remoteColorIdx = 0;
	m_isServerBout = false;
}

GameManager::~GameManager(void)
{

}

GameManager* GameManager::getInstance()
{
	if (!mpGameManager)
	{
		mpGameManager = new GameManager();
		mpGameManager->init();
	}
	return mpGameManager;
}

bool GameManager::init()
{
	return true;
}

long GameManager::getCurrentTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	long curt = (now.tv_sec % 1000000) * 1000 + now.tv_usec / 1000;
	return curt;
}

void GameManager::setClientServerDeltaTime(long dt)
{
	m_clientServerDtTime = dt;
}

void GameManager::setNameBoard(NameBoardType type, int Id)
{
	switch (type)
	{
		case kHead:
		{
			_nbHead = Id;
			break;
		}
		case kTitle:
		{
			_nbTitle = Id;
			break;
		}
	}
	WriteMgr->saveNameBoard(type, Id);
}

int GameManager::getNameBoard(NameBoardType type)
{
	if (type == kHead)
		return _nbHead;
	else if (type == kTitle)
		return _nbTitle;
	return 1;
}

void GameManager::setOppoNameBoard(NameBoardType type, int Id)
{
	if (Id == 0)
	{
		//log("Id == 0");
		Id = 1;
	}
	switch (type)
	{
		case kHead:
			_nbOppoHead = Id;
			break;
		case kTitle:
			_nbOppoTitle = Id;
			break;
		case kShade:
			_nbOppoShade = Id;
			break;
	}
}

int GameManager::getOppoNameBoard(NameBoardType type)
{
	if (type == kHead)
		return _nbOppoHead;
	else if (type == kTitle)
		return _nbOppoTitle;
	else if (type == kShade)
		return _nbOppoShade;
	return 1;
}

void GameManager::setMapId(int mapId)
{
	m_mapId = mapId;
}

int GameManager::getMapId() const
{
	return m_mapId;
}

void GameManager::setOppoName(std::string string)
{
	m_oppoName = string;
}

std::string GameManager::getOppoName()
{
	return m_oppoName;
}

void GameManager::setIsServer(bool server)
{
	m_isServer = server;
}

bool GameManager::getIsServer()
{
	return m_isServer;
}

void GameManager::setPjPing(int ping)
{
	m_pjPing = ping;
}

int GameManager::getPjPing()
{
	return m_pjPing;
}

void GameManager::setGameFightType(GameFightType _type)
{
	m_gameFightType = _type;
}

GameFightType GameManager::getGameFightType()
{
	return m_gameFightType;
}

void GameManager::setRoomName(std::string nameStr)
{
	m_nameString.clear();
	m_nameString.append(nameStr.c_str());
}

std::string GameManager::getRoomName()
{
	return m_nameString;
}

void GameManager::setRoomSize(int count)
{
	m_roomSize = count;
}

int GameManager::getRoomSize()
{
	return m_roomSize;
}

void GameManager::setRoomMode(FightingMode fightMode)
{
	m_fightingMode = fightMode;
}

FightingMode GameManager::getRoomMode()
{
	return m_fightingMode;
}

void GameManager::setRoomView(ViewGameState viewState)
{
	m_viewState = viewState;
}

ViewGameState GameManager::getRoomView()
{
	return m_viewState;
}

void GameManager::setRoomMap(int mapid)
{
	m_mapID = mapid;
}

int GameManager::getRoomMap()
{
	return m_mapID;
}

void GameManager::setLocalColor(int colorid)
{
	m_colorID = colorid;
}

int GameManager::getLocalColor()
{
	return m_colorID;
}

void GameManager::setRemoteColor(int coloridx)
{
	m_remoteColorIdx = coloridx;
}

int GameManager::getRemoteColor()
{
	return m_remoteColorIdx;
}

Color3B GameManager::getTeamColor(int colorid)
{
	int colorArr[8][3] =
	{
		{  51, 102, 255 },
		{ 102,   0, 102 },
		{ 204, 102, 153 },
		{ 255,   0,   0 },
		{ 255, 153,   0 },
		{ 255, 255,   0 },
		{ 102, 204,   0 },
		{  51, 255, 255 }
	};

	return Color3B(colorArr[colorid][0], colorArr[colorid][1], colorArr[colorid][2]);
}

void GameManager::setServerBout(bool serverBout)
{
	m_isServerBout = serverBout;
}

bool GameManager::getIsServerBout()
{
	return m_isServerBout;
}
