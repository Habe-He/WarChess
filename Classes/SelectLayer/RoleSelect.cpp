#include "RoleSelect.h"
#include "DataManager/DataManager.h"
#include "PublicTool/ComResDef.h"

RoleSelect* RoleSelect::createInstance(int id)
{
	auto sprite = new (std::nothrow) RoleSelect();
	if (sprite && sprite->init(id))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool RoleSelect::init(int id)
{
	isRoleSelect = false;
	auto roleProptyVector = DataManager::getInstance()->getVRolePropty();

	auto enNameString = __String::createWithFormat("Role/Photo/%s.png", roleProptyVector[id - RoleStarFlag].sR_EnName);
	auto spritePhoto = Sprite::create(enNameString->getCString());
	spritePhoto->setTag(12);
	this->addChild(spritePhoto, -2);

	Sprite* spriteFrame = nullptr;
	switch (roleProptyVector[id - RoleStarFlag].sR_RolePossess)
	{
	case P_MainLand:
		spriteFrame = Sprite::create("MainFight/Role.png");
		spriteFrame->setColor(Color3B::GREEN);
		break;
	case P_Geocenter:
		spriteFrame = Sprite::create("MainFight/Role.png");
		spriteFrame->setColor(Color3B::RED);
		break;
	case P_Sky:
		spriteFrame = Sprite::create("MainFight/Role.png");
		spriteFrame->setColor(Color3B::BLUE);
		break;
	}
	spriteFrame->setTag(11);
	this->addChild(spriteFrame, 0);

	auto nameString = __String::createWithFormat("%s", roleProptyVector[id - RoleStarFlag].sR_ChinaName);
	auto nameLabel = Label::createWithSystemFont(nameString->getCString(), "", 20);
	nameLabel->setPosition(Vec2(0, 60));
	this->addChild(nameLabel);

	auto proptyString = __String::createWithFormat("%d %d %d %d", roleProptyVector[id - RoleStarFlag].sR_Attack, 
		roleProptyVector[id - RoleStarFlag].sR_HP, roleProptyVector[id - RoleStarFlag].sR_Agility, roleProptyVector[id - RoleStarFlag].sR_Marshal );
	auto proptyLabel = Label::createWithTTF(proptyString->getCString(), PixelFont, 14);
	proptyLabel->setPosition(Vec2(0, -55));
	this->addChild(proptyLabel);
	return true;
}

Size RoleSelect::getFrameRect()
{
	auto sprite = (Sprite*)this->getChildByTag(11);
	return sprite->getContentSize();
}

void RoleSelect::setGray(bool isSelect)
{
	isRoleSelect = isSelect;
	auto sprite = (Sprite*)this->getChildByTag(12);
	if ( isSelect )
	{
		sprite->setColor(Color3B(80, 80, 80));
	}
	else
	{
		sprite->setColor(Color3B::WHITE);
	}
}

bool RoleSelect::isSelect()
{
	return isRoleSelect;
}