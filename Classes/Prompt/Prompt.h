#ifndef Prompt_hpp
#define Prompt_hpp

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

const int PromptFlag = 10;

class Prompt : public Sprite
{
public:
	static Prompt* getInstance();
	static Prompt* mpInstance;

	void setShowPrompt(const string& pString, int fontSize = 18);

	void setMoveTop();

	void remove();
};

#endif