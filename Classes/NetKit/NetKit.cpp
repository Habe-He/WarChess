//
//  NetKit.cpp
//  ParkourGame
//
//  Created by haibo yin on 4/18/14.
//
//

#include "NetKit.h"
#include "NetMessage.h"

NetKit::NetKit()
{
	m_delegate = nullptr;
    m_broadcast = false;
    m_client = false;
    m_server = false;
}

NetKit::~NetKit()
{
    stopBroadcast();
    stop();
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(NetKit::update), this);
}

static NetKit* s_NetKit = NULL;

NetKit* NetKit::getInstance()
{
	if (!s_NetKit)
    {
		s_NetKit = new NetKit();
        Director::getInstance()->getScheduler()->schedule(schedule_selector(NetKit::update), s_NetKit, 0, false);
	}
	
	return s_NetKit;
}

void NetKit::setDelegate(NetKitDelegate *delegate) {
    m_delegate = delegate;
}

void NetKit::update(float dt)
{
//    if (m_broadcast) {
//        BROADCASTER()->update(dt);
//    }
    
    if (m_client) {
        CLIENT()->update(dt);
    }
    if (m_server) {
        SERVER()->update(dt);
    }
}

#pragma broadcast
void NetKit::startBroadServer(unsigned short port, const char* name)
{
    if (m_broadcast) {
        return;
    }
    m_broadcast = true;
    BROADCASTER()->startServer(port, name);
}

void NetKit::changeName(const char* name) {
    if (!m_broadcast) {
        return;
    }
    BROADCASTER()->changeName(name);
}

void NetKit::startFindServer(unsigned short port)
{
    if (m_broadcast) {
        return;
    }
    m_broadcast = true;
    BROADCASTER()->startClient(port, this);
}

void NetKit::stopListen()
{
    if (!m_server) {
        return;
    }
    SERVER()->stopListen();
}

void NetKit::stopBroadcast()
{
	if (!m_broadcast) {
		return;
	}
    m_broadcast = false;
    BROADCASTER()->stop();
}

void NetKit::onFindServer(const char* serverName, const char* ip, unsigned short port)
{
	if (m_delegate != nullptr)
		m_delegate->onFindServer(serverName, ip, port);
}

#pragma client
bool NetKit::connectServer(const char* ip, unsigned short port)
{
    if (m_client) {
        return false;
    }
    m_client = true;
    return CLIENT()->start(ip, port, this);
}

void NetKit::onClientMessage(void* data)
{
	if (m_delegate != nullptr)
		m_delegate->onClientMessage(data);
}

void NetKit::onClientDisconnect()
{
	if (m_delegate != nullptr)
		m_delegate->onClientDisconnect();
}

#pragma server
void NetKit::startServer(unsigned short port)
{
    if (m_server) {
        return;
    }
    m_server = true;
    SERVER()->start(port, this);
}

void NetKit::onServerMessage(void* data)
{
	if (m_delegate != nullptr)
		m_delegate->onServerMessage(data);
}

void NetKit::onServerDisconnect()
{
	if (m_delegate != nullptr)
		m_delegate->onServerDisconnect();
}

#pragma common
void NetKit::send(void* data, int nDataLen)
{
    ((ProtHead*)data)->len = nDataLen;
    if (m_client) {
        CLIENT()->send(data, nDataLen);
    } else if (m_server) {
        SERVER()->send(data, nDataLen);
    }
}

void NetKit::sendd(void* data, int nDataLen)
{
    ((ProtHead*)data)->len = nDataLen;
    if (m_client) {
        CLIENT()->sendd(data, nDataLen);
    } else if (m_server) {
        SERVER()->sendd(data, nDataLen);
    }
}

void NetKit::stop()
{
    if (m_client) {
        m_client = false;
        CLIENT()->stop();
    }
    
    if (m_server) {
        m_server = false;
        SERVER()->stop();
    }

    if (m_delegate != nullptr) {
        m_delegate = nullptr;
    }
}

bool NetKit::getIsConnecting()
{
	if (m_client == false && m_server == false && m_broadcast == false)
		return false;

	return true;
}