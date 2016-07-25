#include "AnimationManager.h"

static AnimationManager* s_ShareAnimaion = nullptr;

AnimationManager* AnimationManager::getInstance()
{
	s_ShareAnimaion = new AnimationManager();
	if ( s_ShareAnimaion )
	{
		s_ShareAnimaion->init();
		s_ShareAnimaion->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(s_ShareAnimaion);
		s_ShareAnimaion = nullptr;
	}
	return s_ShareAnimaion;
}

bool AnimationManager::init()
{

	return true;
}

void AnimationManager::playAssMiss()
{
	assMissDelat = 0.5f;
	string nameString = "Role/Effect/BAOZA/BAOZA";
	auto rolePng = __String::createWithFormat("%s0.png", nameString.c_str());
	auto rolePlist = __String::createWithFormat("%s0.plist", nameString.c_str());
	auto roleJson = __String::createWithFormat("%s.ExportJson", nameString.c_str());

	ArmatureDataManager::getInstance()->addArmatureFileInfo(rolePng->getCString(), rolePlist->getCString(), roleJson->getCString());
	auto armature = Armature::create("BAOZA");
	armature->getAnimation()->playWithIndex(0);
	armature->setTag(AnimationFlag);
	armature->setScale(AnimationScale);
	this->addChild(armature);

	std::function<void(Armature*, MovementEventType, const std::string&)> armatureFun = [=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if (type == MovementEventType::COMPLETE)
		{
			this->runAction(Sequence::create(DelayTime::create(.1f), 
							CallFunc::create(CC_CALLBACK_0(AnimationManager::removeAnimation, this)), NULL));
		}
	};
	armature->getAnimation()->setMovementEventCallFunc(armatureFun);
}

void AnimationManager::removeAnimation()
{
	auto armature = (Armature*)this->getChildByTag(AnimationFlag);
	armature->removeFromParentAndCleanup(true);
}