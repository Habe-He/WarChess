#ifndef _Tool_Func_
#define _Tool_Func_

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

#include "Hero/Hero.h"
#include "Hero/RoleDef.h"

#define SCREEN_WIDTH	960
#define SCREEN_HEIGTH	640

#define SCREEN_WIDTH_HALF	( 960 >> 1 )
#define SCREEN_HEIGHT_HALF	( 640 >> 1 )

#define TXM_MAX_SCALE 1.5f		// ��ͼ���Ŵ����
#define TXM_MIN_SCALE 0.9f		// ��ͼ��С���ű���

enum AttackOrCounter
{
	Attack,							// ��������
	CounterBack,					// ����
};

class ToolFunc
{
public:
	static int calcRandomNum( int _star, int _end );							// ���������

	static vector<string > splitString(string str, string pattern);             // �ָ��ַ���

	static RoleDirection calcRoleDirection(Vec2 orin, Vec2 tar);				// �����ɫ����

	static Vec2 coverToScreenCoord(TMXTiledMap* tileMap, Vec2 point);			// ��ͼ���� ת ��Ļ����

	static Vec2 coverToTileCoord(TMXTiledMap* tileMap, Vec2 point);				// ��Ļ����ת ��ͼ����

	static Vec2 calcRoleBackPoint(RoleDirection _roleDirection, AttackOrCounter _type);				// ��ɫ����λ��

	static void outLineSprite(Sprite* sprite);									// ͼƬ���
																			
	static void graySprite(Sprite* sprite);										// ͼƬ��Ϊ�Ұ�ɫ
		
	static void ungraySprite(Sprite* sprite);									// ��ͼƬ��Ϊ������ɫ

	static void vectorSort(vector<Hero* >& original, vector<Hero* >& target, int kind);
};
#endif