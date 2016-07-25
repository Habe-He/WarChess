#include "LoadingLayer.h"
#include "PublicTool/ToolFunc.h"

const int LoadingBarTag = 10;

Scene* LoadingLayer::createScene()
{
	auto scene = Scene::create();
	scene->addChild(LoadingLayer::create());

	return scene;
}

void LoadingLayer::onEnter()
{
	Layer::onEnter();
}

void LoadingLayer::onExit()
{
	m_TextureCacheArray->removeAllObjects();

	Layer::onExit();
}

bool LoadingLayer::init()
{
	m_TextureCacheArray = __Array::create();
	m_TextureCacheArray->retain();

	auto loadSprite = Sprite::create("MainFight/ProgressBG.png");
	loadSprite->setPosition(Vec2(SCREEN_WIDTH_HALF, 100));
	this->addChild(loadSprite);

// 	auto loadingSprite = LoadingBar::create("MainFight/Progress.png", 20);
// 	loadingSprite->setScale9Enabled(true);
// 	loadingSprite->setPosition(Vec2(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF));
// 	loadingSprite->setDirection(LoadingBar::Direction::LEFT);
// 	loadingSprite->setCapInsets(Rect(0, 0, 0, 0));
// 	loadingSprite->setContentSize(Size(300, 10));
// 	this->addChild(loadingSprite, 2, LoadingBarTag);

	auto spriteProgress = Sprite::create("MainFight/Progress.png");
	auto progressBar = ProgressTimer::create(spriteProgress);
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Vec2::ANCHOR_BOTTOM_LEFT);
	progressBar->setBarChangeRate(Vec2::ANCHOR_BOTTOM_RIGHT);
	progressBar->setPosition(Vec2(SCREEN_WIDTH_HALF, 100));
	progressBar->setPercentage(20);
	this->addChild(progressBar, 10, LoadingBarTag);

	return true;
}

void LoadingLayer::addTextureCacheFromFileByName(const char* resName)
{
	
}

void LoadingLayer::setChangeToNode(Scene*(*m_Scene))
{
	
}
