#ifndef __WarChess__GameManager__
#define __WarChess__GameManager__

#include "cocos2d.h"
USING_NS_CC;
#include "GameInfo.h"

#define GameMgr() GameManager::getInstance()

class GameManager
{
public:
	GameManager(void);
	virtual ~GameManager(void);

	static GameManager* getInstance();
	static GameManager* mpGameManager;

	bool init();

	long getCurrentTime();

	void setClientServerDeltaTime(long dt);

	void setNameBoard(NameBoardType type, int Id);
	int getNameBoard(NameBoardType type);

	void setOppoNameBoard(NameBoardType type, int Id);
	int getOppoNameBoard(NameBoardType type);
	
	void setMapId(int scene);
	int  getMapId() const;

	void setOppoName(std::string string);
	std::string getOppoName();

	void setIsServer(bool server);
	bool getIsServer();

	void setPjPing(int ping);
	int getPjPing();

	void setGameFightType(GameFightType _type);
	GameFightType getGameFightType();
	
	// setup wifi fighting room info - begain
	void setRoomName(std::string nameStr);
	std::string getRoomName();

	void setRoomSize(int count);
	int getRoomSize();

	void setRoomMode(FightingMode fightMode);
	FightingMode getRoomMode();

	void setRoomView(ViewGameState viewState);
	ViewGameState getRoomView();

	void setRoomMap(int mapid);
	int getRoomMap();

	void setLocalColor(int colorid);				// setup local team color;
	int getLocalColor();

	void setRemoteColor(int coloridx);
	int getRemoteColor();

	Color3B getTeamColor(int colorid);
	// setup wifi fighting room info - end

	void setServerBout(bool serverBout);				// set current bout is servers
	bool getIsServerBout();								// get current bout is servers



private:
	long m_clientServerDtTime;
	bool m_isServer;
	bool m_isServerBout;
	int _nbHead;
	int _nbTitle;
	int _nbOppoHead;
	int _nbOppoTitle;
	int _nbOppoShade;
	int m_mapId;
	int m_pjPing;

	std::string m_nameString;			// wifi fighting room name
	FightingMode m_fightingMode;		// wifi fighting room mode
	ViewGameState m_viewState;			// wifi fighting room state on / off
	int m_roomSize;						// wifi fighting room size
	int m_mapID;						// wifi fighting room map id
	int m_colorID;						// wifi fighting local team color
	int m_remoteColorIdx;				// wifi remote team color;
	
	GameFightType m_gameFightType;
	std::string m_oppoName;
};
#endif