/****************************************
// File name: GameOverLayer.h
// 日   期: 2016/01/15
// 作   者: ouyang
// 简   介: game over show UI layer
*****************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;

class GameOverLayer : public Node
{
public:
	GameOverLayer(){}
	virtual ~GameOverLayer(){}

	bool init();

	void onceAgainClick(Ref* pSender);					// once more

	void exitRoomClick(Ref* pSender);					// exit room

	void setResult(bool isWin);

	CREATE_FUNC(GameOverLayer);

	virtual void onEnter();
	virtual void onExit();

private:
	Label* m_resultLable;
	Menu* pMenu;
};