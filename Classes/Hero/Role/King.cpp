#include "King.h"

#include "GameLayer/GameLayer.h"

const int ArmatureFlag = 10;
const string NKAString = "King Attack";

bool King::init()
{
	_roleType = RoleKing;

	setAttackTime(0.7f);
	setHittedTime(0.2f);
	setDeathTime(1.0f);

	setReaction(1);

	string nameString = "King";
	auto rolePng = __String::createWithFormat("%s0.png", nameString.c_str() );
	auto rolePlist = __String::createWithFormat("%s0.plist", nameString.c_str() );
	auto roleJson = __String::createWithFormat("%s.ExportJson", nameString.c_str() );
	
	ArmatureDataManager::getInstance()->addArmatureFileInfo(rolePng->getCString(), rolePlist->getCString(), roleJson->getCString());
	auto armature = Armature::create(nameString.c_str());
	armature->setAnchorPoint(Vec2(0.5f, 0.6f));
	armature->setTag(ArmatureFlag);
	armature->setScale(0.8f);
	this->addChild(armature);	// ×¢²áÏûÏ¢¼àÌý
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::a_Attack_b_Call), NKAString.c_str(), NULL);	
	return true;
}

void King::setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction)
{
	Hero::setRoleDirAndSkill(roleSkill, direction);
	auto armature = (Armature*)this->getChildByTag(ArmatureFlag);
	if (roleSkill == S_Standby || roleSkill == S_Win || roleSkill == S_Failure || roleSkill == S_Move || roleSkill == S_Sprint )
	{
		armature->getAnimation()->play(SkillName[roleSkill][direction]);
	}
	else if ( roleSkill == S_Death || roleSkill == S_Attack || roleSkill == S_Hit )
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

// 	if (S_Attack == roleSkill)
// 	{
// 		armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(King::isAnimationDone,
// 			this, placeholders::_1, placeholders::_2, placeholders::_3));
// 	}
}

void King::isAnimationDone(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
// 	if (movementType == COMPLETE)
// 	{
// 		NotificationCenter::getInstance()->postNotification(NKAString.c_str());
// 	}
}

void King::hurt(float _damage)
{
	Hero::hurt(_damage);
	updateHDA(this->getRoleAttack(), this->getRoleHP(), this->getRoleAgility());
}

void King::destry()
{
	Hero::destry();
}

Rect King::collideRect()
{
	return Rect( 0, 0, 0, 0 );
}

void King::releaseSkill()
{
	
}