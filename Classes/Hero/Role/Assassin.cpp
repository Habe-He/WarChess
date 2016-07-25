#include "Assassin.h"

const int AssassinFlag = 10;

Assassin::Assassin(void)
{

}

Assassin::~Assassin(void)
{
	ArmatureDataManager::destroyInstance();
}

bool Assassin::init()
{
	setAttackTime(0.7f);
	setHittedTime(0.5f);
	setDeathTime(1.0f);

	setReaction(1);

	_roleType = RoleAssassin;
	string nameString = "Assassin";
	auto rolePng = __String::createWithFormat("%s0.png", nameString.c_str());
	auto rolePlist = __String::createWithFormat("%s0.plist", nameString.c_str());
	auto roleJson = __String::createWithFormat("%s.ExportJson", nameString.c_str());

	ArmatureDataManager::getInstance()->addArmatureFileInfo(rolePng->getCString(), rolePlist->getCString(), roleJson->getCString());
	auto armature = Armature::create(nameString.c_str());
	armature->setAnchorPoint(Vec2(0.5f, 0.6f));
	armature->setTag(AssassinFlag);
	armature->setScale(0.8f);
	this->addChild(armature);

	return true;
}

void Assassin::setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction)
{
	Hero::setRoleDirAndSkill(roleSkill, direction);
	auto armature = (Armature*)this->getChildByTag(AssassinFlag);
	if (roleSkill == S_Standby || roleSkill == S_Win || roleSkill == S_Failure || roleSkill == S_Move || roleSkill == S_Sprint)
	{
		armature->getAnimation()->play(SkillName[roleSkill][direction]);
	}
	else if (roleSkill == S_Death || roleSkill == S_Attack || roleSkill == S_Hit)
	{
		armature->getAnimation()->play(SkillName[roleSkill][direction], -1, 0);
		if (roleSkill == S_Death)
		{
			auto callBack = CallFunc::create([this]()
			{
				this->setVisible(false);
			});
			auto se = Sequence::create(FadeOut::create(getDeathTime()), callBack, NULL);
			this->runAction(se);
		}
	}
}

void Assassin::hurt(float _damage)
{
	Hero::hurt(_damage);
	updateHDA(this->getRoleAttack(), this->getRoleHP(), this->getRoleAgility());
}

void Assassin::destry()
{
	Hero::destry();
}

Rect Assassin::collideRect()
{
	return Rect(0, 0, 0, 0);
}

void Assassin::releaseSkill()
{
	
}