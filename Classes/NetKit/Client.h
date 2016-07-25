//
//  Client.h
//  ParkourGame
//
//  Created by haibo yin on 4/18/14.
//
//

#ifndef __ParkourGame__Client__
#define __ParkourGame__Client__

#include "Thread.h"

#define CLIENT_RECV_BUFF_LEN 1024

#define CLIENT() Client::getInstance()

class ClientDelegate
{
public:
    virtual void onClientMessage(void* data) = 0;
    virtual void onClientDisconnect() = 0;
};

class Client : public Thread
{
public:
    static Client* getInstance();
    Client();
    bool start(const char* ip, unsigned short port, ClientDelegate* delegate);
    void stop();
    void update(float dt);
    void send(void* data, int nDataLen);
    void sendd(void* data, int nDataLen);
    void disconnect();
    
private:
    ClientDelegate* m_delegate;
    char m_recvBuffer[CLIENT_RECV_BUFF_LEN];
};

#endif /* defined(__ParkourGame__Client__) */
