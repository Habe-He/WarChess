//
//  IOStreamBuffer.hpp
//  soccer
//
//  Created by dai xinping on 13-4-22.
//
//

#ifndef soccer_IOStreamBuffer_h
#define soccer_IOStreamBuffer_h

#include "cocos2d.h"

using namespace cocos2d;

#define ntohll(x) ( ((long long)x & 0xff00000000000000LL)>>56 | \
((long long)x & 0x00ff000000000000LL)>>40 | \
((long long)x & 0x0000ff0000000000LL)>>24 | \
((long long)x & 0x000000ff00000000LL)>>8 | \
((long long)x & 0x00000000ff000000LL)<<8 | \
((long long)x & 0x0000000000ff0000LL)<<24 | \
((long long)x & 0x000000000000ff00LL)<<40 | \
((long long)x & 0x00000000000000ffLL)<<56 )

class IOStreamBuffer : public CCObject {
private:
    int m_outBufferLen;
    char* m_outBuffer;
    char* m_outBufferTemp;
    
    char* m_inBuffer;
    char* m_inBufferTemp;

    
public:
    IOStreamBuffer()
    {
        m_outBufferLen = 0;
        m_outBuffer = NULL;
        
        m_inBuffer = NULL;
        m_inBufferTemp = NULL;
    }
    ~IOStreamBuffer()
    {
        clearBuffer();
    }
    
    void clearBuffer() {
        if (m_outBuffer) {
            free(m_outBuffer);
            m_outBuffer = NULL;
        }
        if (m_inBuffer) {
            free(m_inBuffer);
            m_inBuffer = NULL;
        }
    }
    
    void setBuffer(void* data, int dataLen) {
        CCAssert(m_inBuffer == NULL, "");
        
        m_inBuffer = (char*)malloc(dataLen);
        memcpy(m_inBuffer, data, dataLen);
        m_inBufferTemp = m_inBuffer;
    }
    
    int getBufferLen() {
        return m_outBufferLen;
    }
    
    const char* getBuffer() {
        return (const char*)m_outBuffer;
    }
    
    char getByte()
    {
        char ret = *m_inBufferTemp;
        m_inBufferTemp++;

        return ret;
    }
    
    short getShort()
    {
        short ret = ntohs(*((short*)m_inBufferTemp));
        m_inBufferTemp+=2;

        return ret;
    }
    
    int getInt()
    {
        int ret = ntohl(*((int*)m_inBufferTemp));
        m_inBufferTemp+=4;

        return ret;
    }
    
    float getFloat()
    {
        int ret = ntohl(*((int*)m_inBufferTemp));
        m_inBufferTemp+=4;

        return *((float*)&ret);
    }
    
    long long getLong()
    {
        long long ret, temp;
        memcpy(&temp, m_inBufferTemp, 8);
        ret = ntohll(temp);
        m_inBufferTemp+=8;

        return ret;
    }
    
    double getDouble()
    {
        double ret;
        long long temp;
        memcpy(&temp, m_inBufferTemp, 8);
        temp = ntohll(temp);
        ret = *((double*)&temp);
        m_inBufferTemp+=8;

        return ret;
    }
    
    std::string getString()
    {
        std::string ret = "";
        short len = ntohs(*((short*)m_inBufferTemp));
        if (len == 0) {
            m_inBufferTemp+=2;
            return ret;
        }

        char* str = (char*)malloc(len+1);
        memset(str, 0x00, len+1);
        memcpy(str, m_inBufferTemp+2, len);
        ret = str;
        free(str);
        m_inBufferTemp += len+2;
        
        return ret;
    }

    void putInt(int value)
    {
        value = htonl(value);
        m_outBuffer = (char*)realloc(m_outBuffer, m_outBufferLen + sizeof(value));
        memcpy(m_outBuffer + m_outBufferLen, &value, sizeof(value));
        m_outBufferLen += sizeof(value);
    }
    
    void putShort(short value)
    {
        value = htons(value);
        m_outBuffer = (char*)realloc(m_outBuffer, m_outBufferLen + sizeof(value));
        memcpy(m_outBuffer + m_outBufferLen, &value, sizeof(value));
        m_outBufferLen += sizeof(value);
    }
    
    void putByte(char value)
    {
        m_outBuffer = (char*)realloc(m_outBuffer, m_outBufferLen + sizeof(value));
        memcpy(m_outBuffer + m_outBufferLen, &value, sizeof(value));
        m_outBufferLen += sizeof(value);
    }
    
    void putLong(long long value)
    {
        value = ntohll(value);
        m_outBuffer = (char*)realloc(m_outBuffer, m_outBufferLen + sizeof(value));
        memcpy(m_outBuffer + m_outBufferLen, &value, sizeof(value));
        m_outBufferLen += sizeof(value);
    }
    
    void putString(const char *value)
    {
        if (value) {
            m_outBuffer = (char*)realloc(m_outBuffer, m_outBufferLen + strlen(value) + sizeof(short));
            *((short*)(m_outBuffer + m_outBufferLen)) = htons((short)strlen(value));
            memcpy(m_outBuffer + m_outBufferLen + sizeof(short), value, strlen(value));
            m_outBufferLen += strlen(value) + sizeof(short);
        } else {
            *((short*)(m_outBuffer + m_outBufferLen)) = 0x0000;
            m_outBufferLen += sizeof(short);
        }
    }
    
    void putBuffer(const char *value, int valueLen) 
    {
        m_outBuffer = (char*)realloc(m_outBuffer, m_outBufferLen + valueLen);
        memcpy(m_outBuffer + m_outBufferLen, value, valueLen);
        m_outBufferLen += valueLen;
    }
};

#endif
