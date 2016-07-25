#ifndef _WarChess_GameLayer_
#define _WarChess_GameLayer_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "AStarPath/AStartPath.h"
#include "ChessRule/ChessRule.h"
#include "Hero/Hero.h"
#include "Hero/RoleDef.h"

#include "GameDef.h"
#include "Hero/AnimationManager/AnimationManager.h"
#include "NetKit/NetKit.h"
#include "RolePosition/RolePosition.h"

const int	LocalZ			= 10;						// 角色基础层级
const int	TipsFlag		= 1000;						// 界面可走位置提示 做多16个位置
const int	RedFlag			= 11;						// 人物脚下旋转的光圈
const int	PassBtnFlag		= 12;						// 跳过按钮Tag
const int	ProgressFlag	= 13;						// 进度条标志

const int	FrameLayer		= 98;						// 显示人物头像Layer
const int	DotFlag			= 99;						// 一回合结束标志
const int	NorDirRun		= 1;						// 正方向大于一个格子冲刺
const int	BiasDirRun		= 1;						// 斜着大于等于一个格子 冲刺

const float skillTime		= 1.0f;						// 技能释放间隔
const float SprintRate		= 2.0f;						// 角色冲刺倍率
const float BackUp			= 0.2f;						// 角色击退后移时间
const float TurnTime		= 0.05f;					// 角色转身间隔时间
const float	BoutTime		= 600.0f;					// 每一回合时间

const Vec2	offPoint		= Vec2(0, 30);				// 提示路线偏移

class RolePosManager;
class GameLayer : public Layer, public NetKitDelegate
{
public:	
	GameLayer(void);
	virtual ~GameLayer(void);

	static Scene* scene();

	/*通信函数            Star */
	virtual void onServerMessage(void* data);
	virtual void onClientMessage(void* data);
	virtual void onServerDisconnect();
	virtual void onClientDisconnect();
	void onDisconnect(bool isClient);
	/*通信函数            End */

	bool init();

	void loadMap();                                                     // 加载TMX地图

	void initHero();													// 角色初始化

	virtual void onEnter();
	virtual void onExit();

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event){};

	CREATE_FUNC(GameLayer);

	void exitCallBack(Ref* pSender);										// 暂定退出按钮

	Vec2 coverToTileCoord(Vec2 _point);										// 触摸点转换为瓦片地图上面坐标

	Vec2 coverToScreenCoord(Vec2 _point);									// 地图坐标转化为屏幕坐标

	void showKingPath(Vec2 originalPoint, Hero* _heroInfo);

	void showAssassinPath(Vec2 originalPoint, Hero* _heroInfo);

	void showKrazySoldierPath(Vec2 originalPoint, Hero* _heroInfo);

	void showArrowPath(Vec2 originalPoint, Hero* _heroInfo);

	void showSoldierPath(Vec2 originalPoint, Hero* _heroInfo);

	void removePath();														// 移除提示路线

	Vec2 calcNestPoint(Hero* control, Hero* hitted);						// 计算攻击移动最近点

	void initBottom();														// 临时初始化底部按钮

	void sortByAgility();													// 根据敏捷排序

	void showCurRole(OrderRole _orderRole);									// 显示当前可行走的角色

	void runHurtAnimation(Hero* control, Hero* hitted);						// 双方角色执行受击动画

	/*
	param 1：	F 值最小点坐标
	param 2：	是否可以攻击
	*/
	void constructPathAndStartAnimationFromStep(AStartPath *step, bool isAttack);

	/*
	brief  :	执行位移以及角色转身
	param 1:	位移 / 攻击
	*/
	void popStepAndAnimate(int isAttack);

	void insertOpenPath(AStartPath* step);									// 可行走的位置添加列表

	void comRoleCounterCallBack(Node* pSender, RoleAttackCounter _attack);	// 所有角色反击回调
	void comRoleDeathCallBack(Node* pSender);								// 所有角色死亡回调（包括控制方角色移动）
	void comRoleMoveCallBack(Node* pSender, MoveData index);				// 所有角色移动回调

	void roleRemoveFlag();													// 角色移除移动标志

	void focusOnRole(Hero* _hero);											// 角色移动时周围角色关注

	void passCurRole(Ref* pSender);											// 跳过

	void surrenderClick(Ref* pSender);										// 投降

	void pullDownClick(Ref* pSender);										// 下拉菜单

	void showRolePropty(bool isVisable);									// 是否显示所有角色属性

	void notCallBack(Ref* pSender);											// 消息中心回调

	void roleSpin(Hero* _targetRole, RoleDirection _targetDir);				// 角色转身

	void spinCallBack(Ref* pSender, RoleDirection _targetDir);				// 角色转身回调

	void isPassTouche(bool isEnable, bool m_isTouchScreen);					// 跳过按钮是否可用, Screen can Touch?

	void updateProgress(float dt);											// 更新进度条

	void customEventCall(EventCustom* _event);

	void update(float delta);												// 更新WiFi联机状态

	void autoFindPath(Vec2 position, bool isCall);							// 角色路径显示、以及寻路、是否是联机下远程回调

	void gameResult(bool isClient, bool isServers);							// 游戏结束----- 客户端获胜 ----- 服务端获胜
	
private:
	bool isSelectCurRole;													// 是否点击当前可行走的角色
	bool isRunning;															// 控制角色是否冲刺
	bool isTouchScreen;														// 界面是否否可触摸
	Vec2 OffSet;
	Vec2 m_ServersCoord;													// 联机对战相互传递的坐标
	AStartPath* aStartInstance;												// A星单例
	TMXTiledMap* tmxMap;													// 瓦片地图
	Sprite* bgSprite;														// 界面背景图
	Vec2 bgOrigin;															// 坐标点变量
	int tipsCount;															// 提示图片的数量
	ChessRule* _chessRule;													// 规则

	vector<Hero* > curVector;												// 当前正在执行的序列
	vector<Hero* > preVectot;												// 已经执行过的序列

	Hero* pathRole;															// 非当前操作角色
	Hero* hero;																// 存放选中的角色
	Hero* hittedHero1;														// 主动攻击的
	Hero* hittedHero2;														// 被攻击的

	Vector<AStartPath* > _spOpenSteps;										// 开放表
	Vector<AStartPath* > _spClosedSteps;									// 关闭表
	Vector<AStartPath* > _shortestPath;										// 最短路径表

	vector<int > m_remoteTeam;												// 对方玩家战队

	RolePosManager* m_RolePosMagr;											// 角色头像

	Label* labelsss;
};

#endif