#ifndef AIManager_hpp
#define AIManager_hpp

#include "cocos2d.h"
USING_NS_CC;

#include "Hero/Hero.h"

class AIManager
{
public:
    static AIManager* getInstance( TMXTiledMap* tileMap );
    
    static AIManager* mpInstance;
    
    bool init( TMXTiledMap* tileMap );
    
    void getMostSuitRole();
    
    void getMostSuitPath();
    
    Vec2 coverToScreenCoord(Vec2 _point);
    
private:
    vector<Hero* >  _roleVector;
    TMXTiledMap*    _tileMap;
    
};

#endif /* AIManager_hpp */
