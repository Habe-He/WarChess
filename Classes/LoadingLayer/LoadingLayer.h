/****************************************
// File name: LoadingLayer.h
// 日   期: 2016/01/28
// 作   者: ouyang
// 简   介: loading res UI
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
	int m_CurLoadCount;						// 当前加载的图片数量
	int m_TotalityCount;					// 需要加载的数量
	float m_ProgressPrecent;				// 进度条百分比
	__Array* m_TextureCacheArray;			// 存放资源
};