/********************************************************************
 Copyright(C), 2012-2013,
 ��  ��: Player.h
 ��  ��: ��һ�����Ϣ
*********************************************************************/
#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Player
{
public:
	Player()
	{
		m_bLocal = false;
	}

	~Player(){}

	void setName(const char* name);
	const char* getName();
 
	void setLocal(bool _local);
	bool isLocal();

private:
 	bool m_bLocal;
	std::string m_sName;
};