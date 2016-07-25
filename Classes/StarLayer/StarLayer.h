#ifndef _Star_Layer_
#define _Star_Layer_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace ui;

class StarLayer : public Layer
{
public:
	static Scene* scene();

	bool init();

	void starMenuCallBack(Ref* pSender);			// AI 单机对战

	void wifiFightingCall(Ref* pSender);			// WiFi联机对战
	
	CREATE_FUNC(StarLayer);
};
#endif