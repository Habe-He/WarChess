/********************************************************************
 Copyright(C), 2012-2013,
 文  件: AnimationManager.h
 描  述: 角色特效管理
*********************************************************************/

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio;
using namespace ui;

const int AnimationFlag		= 12;
const float AnimationScale	= 0.8f;			// 动画缩放比例

class AnimationManager : public Node
{
public:
	AnimationManager(){};
	virtual ~AnimationManager(){};

	static AnimationManager* getInstance();

	bool init();

	void playAssMiss();

	void removeAnimation();

	CREATE_FUNC(AnimationManager);

public:
	int assMissDelat;						// 刺客消失播放时长
};