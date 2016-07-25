#ifndef AStarPath_hpp
#define AStarPath_hpp

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
#include "Hero/RoleDef.h"

class AStartPath : public Ref
{
public:
    AStartPath();
    ~AStartPath();
    
    static AStartPath* getInstance(const Vec2& pos);
    
    bool initWithPosition(const Vec2& pos);
    
    int getFScore() const;
    bool isEqual(const AStartPath* other ) const;
    string getDescription() const;

	int calcHScore(const Vec2& fromCoord, const Vec2& toCoord);									// �����ƶ���Hֵ

	int calcGScore(const AStartPath* fromStep, const AStartPath* toStep);						// ����Gֵ

	int getStepIndex(const Vector<AStartPath* >& steps, const AStartPath* step);				// ��ȡ��ǰ��������

	/*
	���� 1�� F ֵ��С�ĵ�����
	���� 2�� ��ǰ��ɫ������
	���� 3�� Ŀ�������
	���� 4�� ��ǰ��ɫ�Ƿ����ӽ�ɫ��ײ
	*/

	PointArray* walkAbleCoord(const Vec2& tileCoord, RoleType roleType, Vec2 targetPoint, bool ingnoreColl) const;

	bool isWallTileCoord(const Vec2& point) const;												// ���ƶ����Ƿ��ڵ�ͼ�ϻ��ߵ�ͼ�ϲ����ƶ���λ��

	bool isRoleTileCoord(const Vec2& point, const Vec2 rolePoint, bool& ingnoreColl) const;												// ���ƶ�λ���Ƿ��н�ɫ
    
    CC_SYNTHESIZE(Vec2, _position, Position);
    CC_SYNTHESIZE(int, _gScore, GScore);														// ��ʼ�㵽��ǰ�ķ�������
    CC_SYNTHESIZE(int, _hScore, HScore);														// ��ǰ�㵽Ŀ���Ԥ��ֵ
    CC_SYNTHESIZE(AStartPath*, _parent, Parent);
} ;

#endif /* AStarPath_hpp */
