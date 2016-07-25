#include "Soldier.h"

const int SoldierFlag = 10;

Soldier::Soldier(void)
{

}

Soldier::~Soldier(void)
{
	
}

bool Soldier::init()
{
	setAttackTime(0.7f);
	setHittedTime(0.3f);
	setDeathTime(1.0f);

	setReaction(1);

	string nameString = "Soldier";
	auto rolePng = __String::createWithFormat("%s0.png", nameString.c_str());
	auto rolePlist = __String::createWithFormat("%s0.plist", nameString.c_str());
	auto roleJson = __String::createWithFormat("%s.ExportJson", nameString.c_str());

	ArmatureDataManager::getInstance()->addArmatureFileInfo(rolePng->getCString(), rolePlist->getCString(), roleJson->getCString());
	auto armature = Armature::create(nameString.c_str());
	armature->setAnchorPoint(Vec2(0.5f, 0.6f));
	armature->setScale(0.8f);
	armature->setTag(SoldierFlag);
	this->addChild(armature);
	
	return true;
}

void Soldier::setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction)
{
	Hero::setRoleDirAndSkill(roleSkill, direction);
	auto armature = (Armature*)this->getChildByTag(SoldierFlag);
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

void Soldier::hurt(float _damage)
{
	Hero::hurt(_damage);
	updateHDA(this->getRoleAttack(), this->getRoleHP(), this->getRoleAgility());
}

void Soldier::destry()
{
	Hero::destry();
}

Rect Soldier::collideRect()
{
	return Rect(0, 0, 0, 0);
}

void Soldier::releaseSkill()
{
	
}