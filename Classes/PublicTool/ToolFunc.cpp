#include "ToolFunc.h"

int ToolFunc::calcRandomNum(int _star, int _end)
{
	int result = 0;
	static int count = 0;
	struct timeval now;
	if (_star >= _end)
		return 0;
	gettimeofday(&now, NULL);
	srand(now.tv_usec + count);
	count += 1;
	result = _star + rand() % (_end - _star + 1);
	return result;
}

vector<string > ToolFunc::splitString(string str, string pattern)
{
	CCASSERT(str.size() > 0, "String NULL"); // 断言描述，当传过来的字符串为空的时候执行断言
	string::size_type pos;
	vector<string > resultString;
	str += pattern;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		pos = str.find(pattern, i);
		if (pos < (unsigned)str.size())
		{
			string passString = str.substr(i, pos - i);
			resultString.push_back(passString);
			i = pos + pattern.size() - 1.0f;
		}
	}
	return resultString;
}

void ToolFunc::vectorSort(vector<Hero* >& original, vector<Hero* >& target, int kind)
{
	Hero* hTemp = nullptr;
	for (unsigned int i = 1; i < original.size();  ++ i)
	{
		for (unsigned z = original.size() - 1; z > i;  -- z)
		{
			if ( original[ z ] > original[ z - 1 ] )
			{
				hTemp = original[z];
				original[z] = original[z - 1];
				original[z - 1] = hTemp;
			}
		}
	}
}

RoleDirection ToolFunc::calcRoleDirection(Vec2 orin, Vec2 tar)
{
	auto offX = tar.x - orin.x;
	auto offY = tar.y - orin.y;
	RoleDirection  roleDirection;
	memset(&roleDirection, 0, sizeof(roleDirection));
	if ((offX == 0) && (offY < 0))
	{
		roleDirection = D_Up;
	}
	else if ((offX > 0) && (offY < 0))
	{
		roleDirection = D_RightUp;
	}
	else if ((offX > 0) && (offY == 0))
	{
		roleDirection = D_Right;
	}
	else if ((offX > 0) && (offY > 0))
	{
		roleDirection = D_RightDown;
	}
	else if ((offX == 0) && (offY > 0))
	{
		roleDirection = D_Down;
	}
	else if ((offX < 0) && (offY > 0))
	{
		roleDirection = D_LeftDown;
	}
	else if ((offX < 0) && (offY == 0))
	{
		roleDirection = D_Left;
	}
	else if ((offX < 0) && (offY < 0))
	{
		roleDirection = D_LeftUp;
	}
	return roleDirection;
}

Vec2 ToolFunc::coverToScreenCoord(TMXTiledMap* tileMap, Vec2 point)
{
	auto mapSize = tileMap->getMapSize();
	auto tileSize = tileMap->getTileSize();
	auto tileWidth = tileMap->getBoundingBox().size.width / tileMap->getMapSize().width;
	auto tileHeight = tileMap->getBoundingBox().size.height / tileMap->getMapSize().height;

	auto variable1 = (point.x + mapSize.width / 2 - mapSize.height) * tileWidth * tileHeight;
	auto variable2 = (-point.y + mapSize.width / 2 + mapSize.height) * tileWidth * tileHeight;

	int posx = (variable1 + variable2) / 2 / tileHeight;
	int posy = (variable2 - variable1) / 2 / tileWidth;

	return Point(posx, posy);
}

Vec2 ToolFunc::coverToTileCoord(TMXTiledMap* tileMap, Vec2 point)
{
	// 触摸的位置信息必须减去瓷砖地图的位置信息,因为地图的位置可能在滚动变化
	auto pos = point - tileMap->getPosition();
	float halfMapWidth = tileMap->getMapSize().width * 0.5f;
	float mapHeight = tileMap->getMapSize().height;
	float tileWidth = tileMap->getTileSize().width;
	float tileHeight = tileMap->getTileSize().height;
	auto tilePosDiv = Vec2(pos.x / tileWidth, pos.y / tileHeight);
	float inverseTileY = mapHeight - tilePosDiv.y;
	// 将得到的计算结果转换成 int,以确保得到的是整数
	float posX = (float)(inverseTileY + tilePosDiv.x - halfMapWidth);
	float posY = (float)(inverseTileY - tilePosDiv.x + halfMapWidth);
	if (posX < 0 || posY < 0)
	{
		return Vec2(-1, -1);
	}
	else
	{
		posX = (int)(inverseTileY + tilePosDiv.x - halfMapWidth);
		posY = (int)(inverseTileY - tilePosDiv.x + halfMapWidth);
	}
	// 确保坐标在地图的边界之内
	if (posX < 0 || posX > tileMap->getMapSize().width - 1)
	{
		posX = -1;
	}
	if (posY < 0 || posY > tileMap->getMapSize().height - 1)
	{
		posY = -1;
	}
	return Vec2(posX, posY);
}

Vec2 ToolFunc::calcRoleBackPoint(RoleDirection _roleDirection, AttackOrCounter _type)
{
	Vec2 OffSet = Vec2(65, 32.5);
	Vec2 delatPoint;
	if ( _type == Attack )
	{
		delatPoint = Vec2(30, 20);
	}
	else if ( _type == CounterBack )
	{
		delatPoint = Vec2(70, 30);  // ( 30, 20 )
	}
	Vec2 point = Vec2::ZERO;
	switch (_roleDirection)
	{
	case D_RightDown:
		point = Vec2(0, OffSet.y);
		break;
	case D_Down:
		point = Vec2(OffSet.x - delatPoint.x, OffSet.y - delatPoint.y);
		break;
	case D_LeftDown:
		point = Vec2(OffSet.x - delatPoint.x, 0);
		break;
	case D_Left:
		point = Vec2(OffSet.x - delatPoint.x, -OffSet.y + delatPoint.y);
		break;
	case D_LeftUp:
		point = Vec2(0, -OffSet.y + delatPoint.y);
		break;
	case D_Up:
		point = Vec2(-OffSet.x + delatPoint.x, -OffSet.y + delatPoint.y);
		break;
	case D_RightUp:
		point = Vec2(-OffSet.x + delatPoint.x, 0 + delatPoint.y - 20);
		break;
	case D_Right:
		point = Vec2(-OffSet.x + delatPoint.x, OffSet.y - delatPoint.y);
		break;
	}
	return point;
}

void ToolFunc::outLineSprite(Sprite* sprite)
{
	auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, "shader/example_Outline.fsh");
	auto _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	_glprogramstate->retain();

	Vec3 color(1.0f, 0.2f, 0.3f);
	GLfloat radius = 0.01f;
	GLfloat threshold = 1.75;

	_glprogramstate->setUniformVec3("u_outlineColor", color);
	_glprogramstate->setUniformFloat("u_radius", radius);
	_glprogramstate->setUniformFloat("u_threshold", threshold);

	sprite->setGLProgramState(_glprogramstate);
}

void ToolFunc::graySprite(Sprite* sprite)
{
	if (sprite)
	{
		GLProgram* p = new GLProgram();
		p->initWithFilenames("Shader/gray.vsh", "Shader/gray.fsh");
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		p->link();
		p->updateUniforms();
		sprite->setGLProgram(p);
	}
}

void ToolFunc::ungraySprite(Sprite* sprite)
{
	if (sprite)
	{
		sprite->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}