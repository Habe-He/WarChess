//
//  Server.h
//  ParkourGame
//
//  Created by haibo yin on 4/18/14.
//
//

#ifndef __ParkourGame__Server__
#define __ParkourGame__Server__

#include "Thread.h"

#define RECV_BUFFER_LEN 1024

#define SERVER() Server::getInstance()

class ServerDelegate
{
public:
    virtual void onServerMessage(void* data) = 0;
    virtual void onServerDisconnect() = 0;
};

class Server : public Thread
{
public:
    static Server* getInstance();
    Server();
    void start(unsigned short port, ServerDelegate* delegate);
    void stop();
    void stopListen();
    void update(float dt);
    void send(void* data, int nDataLen);
    void sendd(void* data, int nDataLen);
    void disconnect();
    
private:
    ServerDelegate* m_delegate;
    Socket* m_listenSocket;
    float m_elapsed;
    char m_recvBuffer[RECV_BUFFER_LEN];
};

#endif /* defined(__ParkourGame__Server__) */
