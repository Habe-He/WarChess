//
//  NetKit.h
//  ParkourGame
//
//  Created by haibo yin on 4/18/14.
//
//

#ifndef __ParkourGame__NetKit__
#define __ParkourGame__NetKit__

#include "cocos2d.h"
#include "Broadcaster.h"
#include "Client.h"
#include "Server.h"

USING_NS_CC;
#define NK() NetKit::getInstance()

class NetKitDelegate 
{
public:
    virtual void onFindServer(const char* serverName, const char* ip, unsigned short port) {}
    virtual void onServerMessage(void* data) {}
    virtual void onClientMessage(void* data) {}
    virtual void onServerDisconnect() {}
    virtual void onClientDisconnect() {}
};

class NetKit : public Node, public BroadcastDelegate, public ClientDelegate, public ServerDelegate
{
public:
	NetKit();
    ~NetKit();
	static NetKit* getInstance();
	
    void update(float dt);
    void setDelegate(NetKitDelegate* delegate);
    
	// for broadcast
    void changeName(const char* name);
	void startBroadServer(unsigned short port, const char* name);
	void startFindServer(unsigned short port);
	void stopBroadcast();
	virtual void onFindServer(const char* serverName, const char* ip, unsigned short port);
    
    // for server
    void startServer(unsigned short port);
    void stopListen();
    virtual void onServerMessage(void* data);
    virtual void onServerDisconnect();
    
    // for client
    bool connectServer(const char* ip, unsigned short port);
    virtual void onClientMessage(void* data);
    virtual void onClientDisconnect();
    
    // for server or client
    void stop();
    void send(void* data, int nDataLen);
    void sendd(void* data, int nDataLen);

	bool getIsConnecting();
private:
	NetKitDelegate* m_delegate;
    bool m_broadcast;
    bool m_client;
    bool m_server;
	//Broadcaster* m_broadcaster;
    //Client* m_client;
    //Server* m_server;
};

#endif /* defined(__ParkourGame__NetKit__) */
