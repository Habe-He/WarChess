#include "AIManager.h"
#include "DataManager/DataManager.h"

AIManager* AIManager::mpInstance = NULL;

AIManager* AIManager::getInstance(TMXTiledMap* tileMap)
{
    if ( !mpInstance )
    {
        mpInstance = new AIManager();
        mpInstance->init(tileMap);
    }
    else
    {
        CC_SAFE_DELETE(mpInstance);
        mpInstance = NULL;
    }
    
    return mpInstance;
}

bool AIManager::init(TMXTiledMap* tileMap)
{
    _roleVector.clear();
    
    _tileMap = tileMap;
    
    for ( auto iter : DataManager::getInstance()->getRoleInfo() )
    {
        auto role = (Hero* )iter;
//         if ( role->getRolePossess() == P_Black)
//         {
//             _roleVector.push_back(role);
//         }
    }
    getMostSuitPath();
    return true;
}

void AIManager::getMostSuitRole()
{
    
}

void AIManager::getMostSuitPath()
{
    for ( unsigned int i = 0; i < _roleVector.size(); i ++ )
    {
        if ( i == 0 )
        {
            auto role = (Hero* )_roleVector[ i ];
            auto pos = role->getRoleCoord() + Vec2( 1, 1 );
            role->setRoleCoord(pos);
            auto ss = coverToScreenCoord(pos);
            role->setPosition(ss);
        }
    }
}


Vec2 AIManager::coverToScreenCoord(Vec2 _point)
{
    auto mapSize = _tileMap->getMapSize();
    auto tileSize = _tileMap->getTileSize();
    auto tileWidth = _tileMap->getBoundingBox().size.width / _tileMap->getMapSize().width;
    auto tileHeight = _tileMap->getBoundingBox().size.height / _tileMap->getMapSize().height;
    
    auto variable1 = (_point.x + mapSize.width / 2 - mapSize.height) * tileWidth * tileHeight ;
    auto variable2 = (-_point.y + mapSize.width / 2 + mapSize.height) * tileWidth * tileHeight ;
    
    int posx = (variable1 + variable2) / 2 / tileHeight;
    int posy = (variable2 - variable1) / 2 / tileWidth;
    
    return Point(posx, posy);
}
