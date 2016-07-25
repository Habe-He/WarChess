#include "Prompt.h"

#include "PublicTool/ToolFunc.h"

Prompt* Prompt::mpInstance = NULL;

Prompt* Prompt::getInstance()
{
	if (!mpInstance)
	{
		mpInstance = new Prompt();
	}

	return mpInstance;
}

void Prompt::setShowPrompt(const string& pString, int fontSize)
{
	auto label = Label::create();
	label->setString(pString.c_str());
	label->setSystemFontSize(fontSize);
	label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	auto size = Size(label->getContentSize().width + 20, label->getContentSize().height + 10);

	/*auto sprite = Scale9Sprite::create("CloseNormal.png");
	sprite->setContentSize(size);
	sprite->addChild(label);
	label->setPosition(Point(sprite->getPosition() + Point(8, 5)));
	sprite->setPosition(Point(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF));
	this->addChild(sprite, 10, PromptFlag);*/
}

void Prompt::setMoveTop()
{
	auto sprite = (Sprite*)this->getChildByTag(PromptFlag);
	auto removeSprite = CallFunc::create(CC_CALLBACK_0(Prompt::remove, this));
	auto sequence = Sequence::create(MoveBy::create(2.0f, Point(0, SCREEN_HEIGHT_HALF)), removeSprite, NULL);
	sprite->runAction(sequence);
}

void Prompt::remove()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
}