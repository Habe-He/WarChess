//
//  Utils.h
//  Utils
//
//
//

#ifndef __PARKOUR_UTILS_H_
#define __PARKOUR_UTILS_H_

#include "cocos2d.h"

USING_NS_CC;

#define RANDOM_VALUE_NUM 20

class Utils {

public:
    static float randValues[RANDOM_VALUE_NUM];
    
    static bool split_int(std::string instr, std::string pattern, std::vector<int>* outs);
	static std::vector<std::string> split_string(std::string str, std::string pattern);
    static int ut_pow(int base, int exp);
	static cocos2d::Point calcPointByLen(float x1, float y1, float x2, float y2, float len);

    static void graySprite(Node* sp);
    static void ungraySprite(Node* sp);
    
    static bool checkNetwork();
    static int randomb(int min, int max);
    static int randomv(std::vector<int>& values);
};

class UtilsCountUp
{
public:
    UtilsCountUp();
    void start(int start, int end, float duration, std::function<void(int)> call);
    void update(float dt);
    
private:
    int easeOutExpo(float t, int b, int c, int d);
    
    int m_start;
    int m_end;
    int m_value;
    float m_duration;
    float m_progress;
    bool m_counting;
    bool m_countDown;
    std::function<void (int)> m_countCall;
};

#endif /* defined(__PARKOUR_UTILS_H_) */
