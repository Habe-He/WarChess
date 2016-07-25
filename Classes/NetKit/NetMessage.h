//
//  NetMessage.h
//  ParkourGame
//
//  Created by haibo yin on 4/14/14.
//
//

#ifndef __ParkourGame__NetMessage__
#define __ParkourGame__NetMessage__

#include "Utils.h"
#include "const.h"
using namespace std;
#include "GameManager/GameInfo.h"

#define PROTO_LOGIN			0x01
#define PROTO_START			0x02
#define PROTO_DELAY_START	0x03
#define PROTO_QUERY_ENTER	0x05
#define PROTO_PING_REQ		0x06
#define PROTO_PING_RSP		0x07
#define PROTO_PREPARE		0x08
#define PROTO_ACTOR			0x09
#define PROTO_CLIENT_OK		0x10
#define PROTO_EXIT_ROOM		0x11
#define PROTO_HEART_BEAT	0x12
#define PROTO_DO_SCENE		0x65

#define PROTO_SET_TIME		0x87

#define PROTO_TEST_PING		0x10001

#define PROT_EXCHANGE_TEAM  0x102
#define PROT_MOVE_ATTACK	0x103
#define PROT_CWIFI_INFO     0x104
#define PROT_TEAM_COLOR		0x105
#define PROT_MAP_DATA		0x106
#define PROT_WIFI_OK		0x107
#define PROT_PASS_CLICK		0x108			// 跳过按钮
#define PROT_SEND_TEAM		0x109			// 排好序的敏捷发送给客户端

#define PROT_SEDN_STOP		0x110			// tingzhi fa song shu ju

struct ProtHead
{
    int protoId;
    int len;
};

struct ProtQueryEnter
{
    ProtHead head;
    int ping;
};

struct ProtStart
{
    ProtHead head;
};

struct ProtDelayStart
{
    ProtHead head;
    int stime;
};

struct ProtTestPing
{
    ProtHead head;
    int count;
};

struct ProtPrepare
{
    ProtHead head;
	bool prepare;
};

struct ProtPing
{
    ProtHead head;
    int time;
};

struct ProtSetTime
{
    ProtHead head;
    int ctime;
    int stime;
};

struct ProtLogin
{
    ProtHead head;
    int cReqTime;
    int sRspTime;
    char name[64];
    int mapId;
    int headId;
    int titleId;
    int shadeId;
};

struct ProtReady
{
    ProtHead head;
    long time;
};

struct ProtOtherMaxHp 
{
    ProtHead head;
	int v_int;
};

struct ProtClientOk
{
	ProtHead head;
	bool prepareOK;
};

struct ProtChangeData
{
	ProtHead head;
	int shadeId;
	int headId;
	int titleId;
	int mapId;
};

// 相互发送战队信息
struct ProtExchangeTeam			
{
	ProtHead head;
	char c_team[1024];
};

// 发送移动或者攻击
struct ProtMoveAttack
{
	ProtHead head;
	Vec2 tileCoord;
};

struct CWiFiStruct
{
	ProtHead head;
	char* roomNameStr[400];				// 房间名称
	int mapID;							// 选择的地图
	int roomSize;						// 房间人数
	FightingMode mode;					// 战斗模式
	ViewGameState viewState;			// 观战模式
};

struct RemoteColor
{
	ProtHead head;
	int color;							// team color
	char name[256];
	int mapID;							// select map id
	int roomSize;						// team size
	int roomMode;						// fighting mode
};

// wifi 对战开始游戏
struct ProtWiFiOK
{
	ProtHead head;
	bool isOK;
};

// 服务端发送给客户端敏捷度
struct ProtAgality
{
	ProtHead head;
	char idStr[1024];
	char colorStr[1024];
};

struct SSSSSSSS
{
	ProtHead head;
};

#endif /* defined(__ParkourGame__NetMessage__) */
