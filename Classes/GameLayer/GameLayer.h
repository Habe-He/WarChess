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

const int	LocalZ			= 10;						// ��ɫ�����㼶
const int	TipsFlag		= 1000;						// �������λ����ʾ ����16��λ��
const int	RedFlag			= 11;						// ���������ת�Ĺ�Ȧ
const int	PassBtnFlag		= 12;						// ������ťTag
const int	ProgressFlag	= 13;						// ��������־

const int	FrameLayer		= 98;						// ��ʾ����ͷ��Layer
const int	DotFlag			= 99;						// һ�غϽ�����־
const int	NorDirRun		= 1;						// ���������һ�����ӳ��
const int	BiasDirRun		= 1;						// б�Ŵ��ڵ���һ������ ���

const float skillTime		= 1.0f;						// �����ͷż��
const float SprintRate		= 2.0f;						// ��ɫ��̱���
const float BackUp			= 0.2f;						// ��ɫ���˺���ʱ��
const float TurnTime		= 0.05f;					// ��ɫת����ʱ��
const float	BoutTime		= 600.0f;					// ÿһ�غ�ʱ��

const Vec2	offPoint		= Vec2(0, 30);				// ��ʾ·��ƫ��

class RolePosManager;
class GameLayer : public Layer, public NetKitDelegate
{
public:	
	GameLayer(void);
	virtual ~GameLayer(void);

	static Scene* scene();

	/*ͨ�ź���            Star */
	virtual void onServerMessage(void* data);
	virtual void onClientMessage(void* data);
	virtual void onServerDisconnect();
	virtual void onClientDisconnect();
	void onDisconnect(bool isClient);
	/*ͨ�ź���            End */

	bool init();

	void loadMap();                                                     // ����TMX��ͼ

	void initHero();													// ��ɫ��ʼ��

	virtual void onEnter();
	virtual void onExit();

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event){};

	CREATE_FUNC(GameLayer);

	void exitCallBack(Ref* pSender);										// �ݶ��˳���ť

	Vec2 coverToTileCoord(Vec2 _point);										// ������ת��Ϊ��Ƭ��ͼ��������

	Vec2 coverToScreenCoord(Vec2 _point);									// ��ͼ����ת��Ϊ��Ļ����

	void showKingPath(Vec2 originalPoint, Hero* _heroInfo);

	void showAssassinPath(Vec2 originalPoint, Hero* _heroInfo);

	void showKrazySoldierPath(Vec2 originalPoint, Hero* _heroInfo);

	void showArrowPath(Vec2 originalPoint, Hero* _heroInfo);

	void showSoldierPath(Vec2 originalPoint, Hero* _heroInfo);

	void removePath();														// �Ƴ���ʾ·��

	Vec2 calcNestPoint(Hero* control, Hero* hitted);						// ���㹥���ƶ������

	void initBottom();														// ��ʱ��ʼ���ײ���ť

	void sortByAgility();													// ������������

	void showCurRole(OrderRole _orderRole);									// ��ʾ��ǰ�����ߵĽ�ɫ

	void runHurtAnimation(Hero* control, Hero* hitted);						// ˫����ɫִ���ܻ�����

	/*
	param 1��	F ֵ��С������
	param 2��	�Ƿ���Թ���
	*/
	void constructPathAndStartAnimationFromStep(AStartPath *step, bool isAttack);

	/*
	brief  :	ִ��λ���Լ���ɫת��
	param 1:	λ�� / ����
	*/
	void popStepAndAnimate(int isAttack);

	void insertOpenPath(AStartPath* step);									// �����ߵ�λ������б�

	void comRoleCounterCallBack(Node* pSender, RoleAttackCounter _attack);	// ���н�ɫ�����ص�
	void comRoleDeathCallBack(Node* pSender);								// ���н�ɫ�����ص����������Ʒ���ɫ�ƶ���
	void comRoleMoveCallBack(Node* pSender, MoveData index);				// ���н�ɫ�ƶ��ص�

	void roleRemoveFlag();													// ��ɫ�Ƴ��ƶ���־

	void focusOnRole(Hero* _hero);											// ��ɫ�ƶ�ʱ��Χ��ɫ��ע

	void passCurRole(Ref* pSender);											// ����

	void surrenderClick(Ref* pSender);										// Ͷ��

	void pullDownClick(Ref* pSender);										// �����˵�

	void showRolePropty(bool isVisable);									// �Ƿ���ʾ���н�ɫ����

	void notCallBack(Ref* pSender);											// ��Ϣ���Ļص�

	void roleSpin(Hero* _targetRole, RoleDirection _targetDir);				// ��ɫת��

	void spinCallBack(Ref* pSender, RoleDirection _targetDir);				// ��ɫת��ص�

	void isPassTouche(bool isEnable, bool m_isTouchScreen);					// ������ť�Ƿ����, Screen can Touch?

	void updateProgress(float dt);											// ���½�����

	void customEventCall(EventCustom* _event);

	void update(float delta);												// ����WiFi����״̬

	void autoFindPath(Vec2 position, bool isCall);							// ��ɫ·����ʾ���Լ�Ѱ·���Ƿ���������Զ�̻ص�

	void gameResult(bool isClient, bool isServers);							// ��Ϸ����----- �ͻ��˻�ʤ ----- ����˻�ʤ
	
private:
	bool isSelectCurRole;													// �Ƿ�����ǰ�����ߵĽ�ɫ
	bool isRunning;															// ���ƽ�ɫ�Ƿ���
	bool isTouchScreen;														// �����Ƿ��ɴ���
	Vec2 OffSet;
	Vec2 m_ServersCoord;													// ������ս�໥���ݵ�����
	AStartPath* aStartInstance;												// A�ǵ���
	TMXTiledMap* tmxMap;													// ��Ƭ��ͼ
	Sprite* bgSprite;														// ���汳��ͼ
	Vec2 bgOrigin;															// ��������
	int tipsCount;															// ��ʾͼƬ������
	ChessRule* _chessRule;													// ����

	vector<Hero* > curVector;												// ��ǰ����ִ�е�����
	vector<Hero* > preVectot;												// �Ѿ�ִ�й�������

	Hero* pathRole;															// �ǵ�ǰ������ɫ
	Hero* hero;																// ���ѡ�еĽ�ɫ
	Hero* hittedHero1;														// ����������
	Hero* hittedHero2;														// ��������

	Vector<AStartPath* > _spOpenSteps;										// ���ű�
	Vector<AStartPath* > _spClosedSteps;									// �رձ�
	Vector<AStartPath* > _shortestPath;										// ���·����

	vector<int > m_remoteTeam;												// �Է����ս��

	RolePosManager* m_RolePosMagr;											// ��ɫͷ��

	Label* labelsss;
};

#endif