#pragma once
using namespace std;

const string Event_String	=   "Event_String";					// ����
// const string ARROW_HRUT		=   "Arrow_Shot_Enemy";				// ��������������
// const string ARROW_COUNTER	=	"Arrow_counter_Back";			// �����ַ���
// const string IS_PASSTOUCH	=	"Is_PassTouch_Able";			// ������ť�Ƿ����

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
	GFT_AI,				// ����ģʽ
	GFT_Internet,		// �����ս
	GFT_WiFi,			// WiFi������ս
};

// ѡ���ս��ģʽ
enum FightingMode		
{
	Mode_Kill_King,			// ߱��
	Mode_Kill_All,			// ȫ��
};

// ��ս�Ƿ���
enum ViewGameState
{
	View_On,				// ����
	View_Off,
};

// WiFi ��սʱ˫������λ��
enum PossessPos
{
	Pos_Up,			// ��ͼ�Ϸ�
	Pos_Down,		// ��ͼ�·�
};