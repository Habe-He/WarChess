#include "KrazySoldier.h"

#include "GameLayer/GameLayer.h"

const int KrazySoldierFlag = 10;

KrazySoldier::KrazySoldier(void)
{

}

KrazySoldier::~KrazySoldier(void)
{
	
}

bool KrazySoldier::init()
{
	string nameString = "KrazySoldier";

	setAttackTime(0.7f);
	setHittedTime(0.4f);
	setDeathTime(1.0f);

	setReaction(1);

	auto rolePng = __String::createWithFormat("%s0.png", nameString.c_str());
	auto rolePlist = __String::createWithFormat("%s0.plist", nameString.c_str());
	auto roleJson = __String::createWithFormat("%s.ExportJson", nameString.c_str());

	ArmatureDataManager::getInstance()->addArmatureFileInfo(rolePng->getCString(), rolePlist->getCString(), roleJson->getCString());
	auto armature = Armature::create(nameString.c_str());
	armature->setAnchorPoint(Vec2(0.45f, 0.8f));
	armature->setTag(KrazySoldierFlag);
	armature->setScale(0.8f);
	this->addChild(armature);

	return true;
}

void KrazySoldier::setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction)
{
	Hero::setRoleDirAndSkill(roleSkill, direction);
	auto armature = (Armature*)this->getChildByTag(KrazySoldierFlag);
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

void KrazySoldier::isAnimationDone(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == COMPLETE)
	{
		//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::newAttackCall), "KrazySoldier Attack", NULL);
		NotificationCenter::getInstance()->postNotification("KrazySoldier Attack");
	}
}

void KrazySoldier::hurt(float _damage)
{
	Hero::hurt(_damage);
	updateHDA(this->getRoleAttack(), this->getRoleHP(), this->getRoleAgility());
}

void KrazySoldier::destry()
{
	Hero::destry();
}

Rect KrazySoldier::collideRect()
{
	return Rect(0, 0, 0, 0);
}

void KrazySoldier::releaseSkill()
{
	
}
