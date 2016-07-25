/****************************************
// File name: LoadingLayer.h
// ��   ��: 2016/01/28
// ��   ��: ouyang
// ��   ��: loading res UI
*****************************************/

#pragma once
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class LoadingLayer : public Layer
{
public:
	LoadingLayer()
	{
		m_CurLoadCount = 0;
		m_TotalityCount = 0;
		m_ProgressPrecent = 0.0f;
	}

	virtual ~LoadingLayer(){}

	static Scene* createScene();

	bool init();

	void onEnter();
	void onExit();

	CREATE_FUNC(LoadingLayer);

	void addTextureCacheFromFileByName(const char* resName);

	void setChangeToNode(Scene*(*m_Scene));

private:
	int m_CurLoadCount;						// ��ǰ���ص�ͼƬ����
	int m_TotalityCount;					// ��Ҫ���ص�����
	float m_ProgressPrecent;				// �������ٷֱ�
	__Array* m_TextureCacheArray;			// �����Դ
};