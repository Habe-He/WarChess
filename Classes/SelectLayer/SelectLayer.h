#pragma once

#include "cocos2d.h"
#include "Hero/RoleDef.h"
USING_NS_CC;

class SelectLayer : public Scene
{
public:
	virtual bool init();
	
	CREATE_FUNC(SelectLayer);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onEnter();
	virtual void onExit();

	void calcMarshal(RolePossess _possess, int iMarshal);

	void starClick(Ref* pSender);

protected:
	Label* mainLandLabel;
	Label* skyLabel;
	Label* geocenterLabel;

	int mainLandValue;
	int skyValue;
	int geocenterValue;
};