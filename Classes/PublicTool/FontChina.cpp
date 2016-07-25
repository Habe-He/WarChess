#include "FontChina.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

FontChina* FontChina::mpFontChina = NULL;

FontChina* FontChina::getInstance()
{
	if (mpFontChina == NULL)
	{
		mpFontChina = new FontChina();
	}
	return mpFontChina;
}

void FontChina::initStringXml(const string& soustr)
{
// 	auto stringPath = __String::createWithFormat(soustr.c_str());
// 
// 	_mpDicComXml = __Dictionary::create();
// 	_mpDicComXml->retain();
// 
// 	const char* pBuffer = NULL;
// 	ssize_t bufferSize = 0;
// 
// 	tinyxml2::XMLDocument myDoc;
// 
// 	pBuffer = FileUtils::getInstance()->getStringFromFile(stringPath->getCString()/*, "rb", &bufferSize*/).c_str();
// 	myDoc.Parse((const char*)pBuffer);
// 
// 	tinyxml2::XMLElement* root = myDoc.FirstChildElement();
// 	tinyxml2::XMLElement* block = root->FirstChildElement();
// 	while (block)
// 	{
// 		string key = block->GetText();
// 		block = block->NextSiblingElement();
// 		__String* value = __String::create(block->GetText());
// 		_mpDicComXml->setObject(value, key);
// 		block = block->NextSiblingElement();
// 	}
}

const __String* FontChina::getString(string key)
{
	return _mpDicComXml->valueForKey(key);
}
