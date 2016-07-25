#ifndef _Font_China_
#define _Font_China_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class FontChina
{
public:
	static FontChina* getInstance();

	// ����XML
	void initStringXml(const string& soustr);

	const __String* getString(string key);      // ����keyֵ��ȡ����

private:
	__Dictionary* _mpDicComXml;
	static FontChina* mpFontChina;
};
#endif