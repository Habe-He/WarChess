//
//  Utils.cpp
//  Utils
//
//
//

#include "Utils.h"

float Utils::randValues[RANDOM_VALUE_NUM];

bool Utils::split_int(std::string instr, std::string pattern, std::vector<int>* outs) {
    if (instr.empty()) {
        return false;
    }
	instr += pattern;
    std::string::size_type pos;
    int size = instr.size();
	for(int i = 0; i < size; i++)
	{
		pos = instr.find(pattern,i);
		if(pos < size) {
			std::string s = instr.substr(i, pos-i);
            outs->push_back( atoi(s.c_str()) );
			i = pos + pattern.size()-1;
		}
	}
    return true;
}


std::vector<std::string> Utils::split_string(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
	std::string::size_type size = str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern, i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

int Utils::ut_pow(int base, int exp)
{
    // TODO, 只处理exp >= 0
    if (exp < 0) {
        return -1;
    } else if (exp == 0) {
        return 1;
    }
    
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result = result * base;
    }
    
    return result;
}

Point Utils::calcPointByLen(float x1, float y1, float x2, float y2, float len)
{
    float r = (y2-y1) / (x2 - x1);
    float dx = sqrtf( (len*len) / (r*r+1) );
    float x = x1 + dx;
    float y = y1 + r*dx;
    return Point(x, y);
}

void Utils::graySprite(Node* sp)
{
    if (sp == nullptr)
        return;

	GLProgram * p = new GLProgram();
	p->initWithFilenames("shader/gray.vsh", "shader/gray.fsh");
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();
	sp->setShaderProgram(p);
}

void Utils::ungraySprite(Node* sp)
{
    if (sp == nullptr)
        return;
    
    GLProgram* p = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
    sp->setShaderProgram(p);
}

bool Utils::checkNetwork()
{
    return CCRANDOM_0_1() > 0.01;
}

int Utils::randomb(int min, int max)
{
    int dt = max - min + 1;
    return min + (int)(dt*CCRANDOM_0_1())%dt;
}

int Utils::randomv(std::vector<int>& values)
{
    if (values.size() == 0) {
        return 0;
    }
    
    int size = values.size();
    int index = CCRANDOM_0_1()*size;
    return values[index];
}


#pragma -mark UtilsCountUp

UtilsCountUp::UtilsCountUp():
m_start(0)
,m_end(0)
,m_value(0)
,m_duration(0)
,m_progress(0)
,m_counting(false)
,m_countCall(nullptr)
{}

void UtilsCountUp::start(int start, int end, float duration, std::function<void (int)> call) {
    if (start == end) {
        m_counting = false;
        return;
    }
    
    m_start = start;
    m_end = end;
    m_duration = duration;
    m_countCall = call;
    m_value = 0;
    m_progress = 0;
    m_counting = true;
    m_countDown = m_start > m_end;
}

void UtilsCountUp::update(float dt) {
    if (!m_counting) {
        return;
    }
    
    m_progress += dt;
    
    if (m_countDown) {
        m_value = m_start - easeOutExpo(m_progress, 0, m_start-m_end, m_duration);
        m_value = (m_value < m_end) ? m_end : m_value;
    } else {
        m_value = easeOutExpo(m_progress, m_start, m_end-m_start, m_duration);
        m_value = (m_value > m_end) ? m_end : m_value;
    }
    m_countCall(m_value);
    
    if (m_progress >= m_duration) {
        m_counting = false;
    }
}

int UtilsCountUp::easeOutExpo(float t, int b, int c, int d) {
    return c * (-pow(2, -10 * t / d) + 1) * 1024 / 1023 + b;
}







