#include "GongJian.h"

const int GongJianFlag = 10;

GongJian::GongJian(void)
{
	
}

GongJian::~GongJian(void)
{
	
}

bool GongJian::init()
{
	setAttackTime(1.5f);
	setHittedTime(1.0f);
	setDeathTime(1.0f);

	setReaction(1);

	setAttackDelat(0.5f);

	string nameString = "GongJian";
	auto rolePng = __String::createWithFormat("%s0.png", nameString.c_str());
	auto rolePlist = __String::createWithFormat("%s0.plist", nameString.c_str());
	auto roleJson = __String::createWithFormat("%s.ExportJson", nameString.c_str());

	ArmatureDataManager::getInstance()->addArmatureFileInfo(rolePng->getCString(), rolePlist->getCString(), roleJson->getCString());
	auto armature = Armature::create(nameString.c_str());
	armature->setAnchorPoint(Vec2(0.5f, 0.6f));
	armature->setTag(GongJianFlag);
	armature->setScale(0.8f);
	this->addChild(armature);

// 	auto point = armature->getPosition();
// 	auto rect = Rect(point.x - armature->getBoundingBox().size.width / 2, point.y - armature->getBoundingBox().size.height / 2,
// 		armature->getBoundingBox().size.width, armature->getBoundingBox().size.height);
//
// 	auto drawNode = DrawNode::create();
// 	Vec2 points[4] =
// 	{
// 		Vec2(point.x - armature->getBoundingBox().size.width / 4, point.y + armature->getBoundingBox().size.height / 2),
// 		Vec2(point.x + armature->getBoundingBox().size.width / 4, point.y + armature->getBoundingBox().size.height / 2),
// 		Vec2(point.x + armature->getBoundingBox().size.width / 4, point.y - armature->getBoundingBox().size.height / 2),
// 		Vec2(point.x - armature->getBoundingBox().size.width / 4, point.y - armature->getBoundingBox().size.height / 2),
// 	};
// 	drawNode->drawPolygon(points, 4, Color4F::BLACK, 0, Color4F::RED);
// 	drawNode->setPosition(armature->getPosition());
// 	this->addChild(drawNode, -1);
	
	return true;
}

void GongJian::setRoleDirAndSkill(RoleSkill roleSkill, RoleDirection direction)
{
	Hero::setRoleDirAndSkill(roleSkill, direction);
	auto armature = (Armature*)this->getChildByTag(GongJianFlag);
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

void GongJian::hurt(float _damage)
{
	Hero::hurt(_damage);
	updateHDA(this->getRoleAttack(), this->getRoleHP(), this->getRoleAgility());
}

void GongJian::destry()
{
	Hero::destry();
}

Rect GongJian::collideRect()
{
	auto armature = (Armature*)this->getChildByTag(GongJianFlag);
	auto point = armature->getPosition();
	auto rect = Rect(point.x - armature->getBoundingBox().size.width / 4, point.y - armature->getBoundingBox().size.height / 2,
		armature->getBoundingBox().size.width, armature->getBoundingBox().size.height);

	return rect;
}

void GongJian::releaseSkill()
{
	
}