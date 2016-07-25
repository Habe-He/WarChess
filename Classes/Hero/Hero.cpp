#include "Hero.h"

#include "Role/King.h"
#include "Role/Soldier.h"
#include "Role/GongJian.h"
#include "Role/Assassin.h"
#include "Role/KrazySoldier.h"

#include "GameLayer/GameLayer.h"
#include "PublicTool/ComResDef.h"
#include "GameManager/GameManager.h"

Hero* Hero::mpHero = nullptr;

Hero::Hero()
{
	
}

Hero::~Hero(void)
{
	
}

Hero* Hero::createRole(RoleType roleType, RolePossess rolePossess)
{
	switch (roleType)
	{
	case RoleAssassin:
		mpHero = Assassin::create();
		break;
	case RoleKrazySoldier:
		mpHero = KrazySoldier::create();
		break;
	case RoleKing:
		mpHero = King::create();
		break;
	case RoleArrow:
		mpHero = GongJian::create();
		break;
	case RoleSoldier:
		mpHero = Soldier::create();
		break;
	}
// 	switch (rolePossess)
// 	{
// 	case P_MainLand:
// 		mpHero->getChildByTag(10)->setColor(Color3B::GREEN);
// 		break;
// 	case P_Geocenter:
// 		mpHero->getChildByTag(10)->setColor(Color3B::RED);
// 		break;
// 	case P_Sky:
// 		mpHero->getChildByTag(10)->setColor(Color3B::BLUE);
// 		break;
// 	}
	
	mpHero->initHero(roleType, rolePossess);
	return mpHero;
}

bool Hero::initHero(RoleType roleType, RolePossess rolePossess)
{
	_roleType = roleType;
	_rolePossess = rolePossess;
 	memset(&_rolePropty, 0, sizeof(_rolePropty));
	//setRoleHDA(RoleAHA[roleType][0], RoleAHA[roleType][1], RoleAHA[roleType][2]);
	setRoleSpeed(RoleAHA[roleType][3]);
	setCounterTime(1.0f);

	return true;
}

void Hero::setRoleCoord(Vec2 coord)
{
	this->setLocalZOrder( (coord.y * coord.x + coord.y) * 10 );
	_roleCoord = coord;
}

Vec2 Hero::getRoleCoord()
{
	return _roleCoord;
}

string Hero::getRoleName(RoleType roleType)
{
	return nameString[roleType];
}

RolePossess Hero::getRolePossess()
{
	return _rolePossess;
}

void Hero::setSkinColor(int colorIndex)
{
	mpHero->getChildByTag(10)->setColor(GameMgr()->getTeamColor(colorIndex));
	m_colorInd = colorIndex;
}

int Hero::getSkinColor()
{
	return m_colorInd;
}

void Hero::setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction)
{
	_direction = direction;
	_roleSkill = roleSkill;
}

void Hero::setRoleHDA(float attack, float hp, float agility, string name)
{
	this->_rolePropty.attack = attack;
	this->_rolePropty.HP = hp;
	this->_rolePropty.agility = agility;

	auto proptySprite = Sprite::create("PreUI/RolePropty.png");
	proptySprite->setPosition(Vec2( 0, -20));
	proptySprite->setTag(SpriteFlag);
	this->addChild(proptySprite);

	auto attackString = __String::createWithFormat("%.0f", attack);
	auto hpString = __String::createWithFormat("%.0f", hp);
	auto agilityString = __String::createWithFormat("%.0f", agility);
	auto nameString = __String::createWithFormat("%s", name.c_str());

	auto attacklabel = Label::createWithTTF("", "fonts/Pixel.TTF", FontSize);
	auto hplabel = Label::createWithTTF("", "fonts/Pixel.TTF", FontSize);
	auto agilitylabel = Label::createWithTTF("", "fonts/Pixel.TTF", FontSize);
	auto nameLabel = Label::create();
	
	attacklabel->setColor(Color3B(255, 255, 150));
	hplabel->setColor(Color3B(255, 105, 105));
	agilitylabel->setColor(Color3B(105, 205, 150));

	attacklabel->setTag(SpriteFlag);
	hplabel->setTag(SpriteFlag + 1);
	agilitylabel->setTag(SpriteFlag + 2);

	attacklabel->setString(attackString->getCString());
	hplabel->setString(hpString->getCString());
	agilitylabel->setString(agilityString->getCString());
	nameLabel->setString(nameString->getCString());

	attacklabel->setPosition(Vec2(proptySprite->getContentSize().width / 6, proptySprite->getContentSize().height / 2));
	hplabel->setPosition(Vec2(proptySprite->getContentSize().width / 2, proptySprite->getContentSize().height / 2));
	agilitylabel->setPosition(Vec2(proptySprite->getContentSize().width / 6 * 5, proptySprite->getContentSize().height / 2));
	nameLabel->setPosition(Vec2(proptySprite->getContentSize().width / 2, proptySprite->getContentSize().height / 2 - 20));

	proptySprite->addChild(attacklabel);
	proptySprite->addChild(hplabel);
	proptySprite->addChild(agilitylabel);
	proptySprite->addChild(nameLabel);
}

void Hero::setLabelVisable(bool isVisable)
{
	auto proptySprite = this->getChildByTag(SpriteFlag);
	proptySprite->setVisible(isVisable);
}

void Hero::updateHDA(float attack, float hp, float agility)
{
	auto spriteAttack = this->getChildByTag(SpriteFlag);

	auto labelAttack = (Label*)spriteAttack->getChildByTag(SpriteFlag);
	auto labelHp = (Label*)spriteAttack->getChildByTag(SpriteFlag + 1);
	auto labelAgility = (Label*)spriteAttack->getChildByTag(SpriteFlag + 2);

	auto attackString = __String::createWithFormat("%.0f", attack);
	__String* hpString = nullptr;
	if (hp < 0)
	{
		hpString = __String::createWithFormat("%d", 0);
	}
	else
	{
		hpString = __String::createWithFormat("%.0f", hp);
	}
	auto agilityString = __String::createWithFormat("%.0f", agility);

	labelAttack->setString(attackString->getCString());
	labelHp->setString(hpString->getCString());
	labelAgility->setString(agilityString->getCString());
}

float Hero::getRoleHP()
{
	return _rolePropty.HP;
}

float Hero::getRoleAttack()
{
	return _rolePropty.attack;
}

float Hero::getRoleAgility()
{
	return _rolePropty.agility;
}

RoleDirection Hero::getRoleDirection()
{
	return _direction;
}

RoleSkill Hero::getRoleState()
{
	return _roleSkill;
}

void Hero::hurt(float _damage)
{
	this->_rolePropty.HP -= _damage;
	
	auto hurtSprite = Sprite::create("PreUI/Hurt.png");
	hurtSprite->setScale(0.7f);
	hurtSprite->setPosition(Vec2(this->getPosition()));
	this->getParent()->addChild(hurtSprite, 10000);

	auto hurtString = __String::createWithFormat("-%.0f", _damage);
	
	auto hurtLabel = Label::createWithTTF("", PixelFont, 20.0f);
	hurtLabel->setString(hurtString->getCString());
	hurtLabel->setColor(Color3B::BLACK);
	hurtLabel->setPosition(Vec2( 30, 36 ));
	hurtSprite->addChild(hurtLabel);
	

	auto callBack = CallFunc::create([=]()
	{
		hurtSprite->removeFromParent();
	});
	hurtSprite->runAction(Sequence::create(DelayTime::create(0.5f), callBack, NULL));

	updateHDA(_rolePropty.attack, _rolePropty.HP, _rolePropty.agility);
}

void Hero::destry()
{
	this->removeFromParentAndCleanup(true);
}

Rect Hero::collideRect()
{
	auto position = _armature->getPosition();
	auto size = _armature->getContentSize() * _armature->getScale();
	return Rect(position.x - size.width / 2, position.y - size.height / 2, size.width, size.height);
}

RoleType Hero::getRoleType()
{
	return _roleType;
}

void Hero::releaseSkill()
{

}

bool Hero::isIngnoreCollide()
{
	bool bRet = false;
	if (_roleType == RoleAssassin)
	{
		bRet = true;
	}
	return bRet;
}
