/********************************************************************
 Copyright(C), 2012-2013,
 ��  ��: AnimationManager.h
 ��  ��: ��ɫ��Ч����
*********************************************************************/

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio;
using namespace ui;

const int AnimationFlag		= 12;
const float AnimationScale	= 0.8f;			// �������ű���

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
	int assMissDelat;						// �̿���ʧ����ʱ��
};