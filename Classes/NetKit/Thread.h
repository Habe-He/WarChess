//  Thread.h
//  GameKit
//
//  Created by dai xinping on 13-5-3.
//
//

#ifndef __ddz__Thread__
#define __ddz__Thread__

#include <iostream>
#include <errno.h>
#include <stack>
#include <string>
#include <cctype>
#include <queue>
#include <list>
//#include <pthread.h>
#include "Socket.h"

#include <mutex>
#include <thread>

#define SOCKET_SUCESS 0
#define SOCKET_TIMEOUT -2

#define MAX_DATA_PACK_LEN 1024
#define THREAD_USE_POOL 1

typedef void* (*ThreadProcess) (void*);

void usleep2(long msTimes);

struct DataPack
{
    //char content[MAX_DATA_PACK_LEN];
    char content[512];
    int nDataLen;
};

class Thread
{
protected:
    Socket* m_socket;

	std::string m_recvDataPool;
    
    char m_recvBuff[MAX_DATA_PACK_LEN];
    std::string m_sendStream;
    std::string m_recvStream;
    
    std::list<std::string> m_recvPackets;
    std::list<std::string> m_sendPackets;
    
    std::thread* m_thread;
    std::thread* m_threadSend;
    std::mutex m_sendMutex;
    std::mutex m_completeMutex;
    
    bool m_needQuit;
    bool m_bInit;
    bool m_leaved;
    
public:
    Thread();
    ~Thread();
    void close();
    bool init(ThreadProcess process, ThreadProcess sendProc, void* data);
    int threadProcess(void* data);
    int sendProcess(void* data);
    void leaveProcess();
    void sendByThread(char* data, int nDataLen);
};

#endif /* defined(__ddz__Thread__) */
