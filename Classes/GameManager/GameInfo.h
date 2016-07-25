#pragma once
using namespace std;

const string Event_String	=   "Event_String";					// 共用
// const string ARROW_HRUT		=   "Arrow_Shot_Enemy";				// 弓箭攻击到敌人
// const string ARROW_COUNTER	=	"Arrow_counter_Back";			// 弓箭手反击
// const string IS_PASSTOUCH	=	"Is_PassTouch_Able";			// 跳过按钮是否可用

const int ARROW_HRUT_FLAG		= 0;
const int ARROW_COUNTER_FLAG	= 1;
const int IS_PASSTOUCH_FLAG		= 2;


enum Game_States
{
	Game_Running,
	Game_Stop,
	Game_Over,
	Game_Win,
};

enum Game_Role
{
	Role_Black,
	Role_Red,
};

enum NameBoardType
{
	kHead = 1,
	kTitle,

	kShade,
	kAttribute,
};

enum GameFightType
{
	GFT_AI,				// 单机模式
	GFT_Internet,		// 网络对战
	GFT_WiFi,			// WiFi联机对战
};

// 选择的战斗模式
enum FightingMode		
{
	Mode_Kill_King,			// 弑君
	Mode_Kill_All,			// 全歼
};

// 观战是否开启
enum ViewGameState
{
	View_On,				// 开启
	View_Off,
};

// WiFi 对战时双方所在位置
enum PossessPos
{
	Pos_Up,			// 地图上方
	Pos_Down,		// 地图下方
};