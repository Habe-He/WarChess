//
//  Broadcaster.h
//  ParkourGame
//
//  Created by haibo yin on 4/16/14.
//
//

#ifndef __ParkourGame__Broadcaster__
#define __ParkourGame__Broadcaster__

#include "Socket.h"
#include "Thread.h"

#define BROADCASTER() Broadcaster::getInstance()

class BroadcastDelegate
{
public:
    virtual void onFindServer(const char* serverName, const char* ip, unsigned short port) = 0;
};

class Broadcaster : public Thread
{
public:
    static Broadcaster* getInstance();
    Broadcaster();
    void startServer(unsigned short port, const char* name);
    void startClient(unsigned short port, BroadcastDelegate* delegate);
    void stop();
    bool socketProcess();
    void changeName(const char* name);
    
    
protected:
    void initial(unsigned short port);
    
protected:
    BroadcastDelegate* m_delegate;
    bool m_isServer;
    unsigned short m_port;
    char m_name[256];
};

#endif /* defined(__ParkourGame__Broadcaster__) */
