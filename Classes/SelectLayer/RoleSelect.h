#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Hero/RoleDef.h"

class RoleSelect : public Node
{
public:
	static RoleSelect* createInstance(int id);

	bool init(int id);

	Size getFrameRect();

	void setGray(bool isSelect);

	bool isSelect();

	bool isRoleSelect;					// 是否已选中
};