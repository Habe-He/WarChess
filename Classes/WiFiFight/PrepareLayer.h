/****************************************
// File name: PrepareLayer.h
// ��   ��: 2015/12/25
// ��   ��: ouyang
// ��   ��: �ȴ���Ҽ������
*****************************************/
#include "cocos2d.h"
USING_NS_CC;

#include "NetKit/NetKit.h"
#include "Hero/RoleDef.h"

const int PLMenuFlag = 10;
const int ClientBtnFlag = 11;
const int ServersBtnFlag = 12;

class PrepareLayer : public Layer, public NetKitDelegate
{
public:
	PrepareLayer();
	~PrepareLayer();

	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(PrepareLayer);

	void startButton(Ref* pSender);					// ��ʼ��ť

	void prepareButton(Ref* pSender);				// ׼����ť

	void setClientButtonEnable(bool enable);
	void setServerButtonEnable(bool enable);

	void updateTwoInformation();

protected: // two mode connect
	virtual void onServerMessage(void* data);
	virtual void onClientMessage(void* data);
	virtual void onServerDisconnect();
	virtual void onClientDisconnect();
	void startHeartBeat();
	void stopHeartBeat();
	void heartBeatCall();
	void sendSetTime();
	void afterDisconnect();
	void update(float dt);
	void activation();
	virtual void active();
	virtual void deactive();
	virtual void b4remove();

private:
	bool m_connectFlag;
	bool m_connect;	bool m_disconnect;
	bool m_prepare;
	int m_setTimeNum;
	std::string m_broadcastStr;
	std::vector<int> m_setTimeValues;
	std::vector<int> m_pingValues;
	Action* m_heartAction;


public:
	// һ��Ϊѡ������ͷ�����

	void initRolePhoto();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onEnter();
	virtual void onExit();

	bool starClick();

	void calcMarshal(RolePossess _possess, int iMarshal);

	Label* mainLandLabel;
	Label* skyLabel;
	Label* geocenterLabel;

	int mainLandValue;
	int skyValue;
	int geocenterValue;
};