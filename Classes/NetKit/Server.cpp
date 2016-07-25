//
//  Server.cpp
//  ParkourGame
//
//  Created by haibo yin on 4/18/14.
//
//

#include "Server.h"
#include <stdio.h>
#include <string.h>
#include "NetMessage.h"

static void* ServerThreadProcess(void* data)
{
    int ret = 0;
    while (true) {
        ret = SERVER()->threadProcess(data);
        if (ret == 1) {
            SERVER()->leaveProcess();
            break;
        } else if (ret == 2) {
            SERVER()->leaveProcess();
            SERVER()->disconnect();
            break;
        }
        usleep2(10000);
    }
    
    return NULL;
}

static void* ServerSendThreadProcess(void* data)
{
    while (true) {
        if (SERVER()->sendProcess(data) != 0) {
            break;
        }
        usleep2(10000);
    }
    
    return NULL;
}

Server::Server()
{
    m_socket = NULL;
    m_delegate = NULL;
    m_elapsed = 0;
    m_leaved = false;

	m_listenSocket = NULL;
}

static Server* s_Server = NULL;

Server* Server::getInstance()
{
    if (!s_Server) {
        s_Server = new Server();
    }
    return s_Server;
}

void Server::start(unsigned short port, ServerDelegate *delegate)
{
    m_delegate = delegate;
    m_leaved = false;
    m_elapsed = 0;

    m_listenSocket = new Socket();
    m_listenSocket->Create(AF_INET, SOCK_STREAM);
    m_listenSocket->Bind(port);
    m_listenSocket->Listen();
}

void Server::update(float dt)
{
    if (m_leaved) {
        return;
    }
    
    m_elapsed += dt;
    if (m_elapsed >= 0.6 && m_listenSocket)
    {
        m_elapsed = 0;
        
        int ret = m_listenSocket->Select(0);
        if (ret > 0)
        {
            Socket sk;
            char ip[64] = {0};
            if (m_listenSocket->Accept(sk, ip)) {
                m_socket = new Socket(sk);
                init(ServerThreadProcess, ServerSendThreadProcess, NULL);
            }
            
            printf("server select ret=%d ip=%s\n", ret, ip);
        }
    }
    
	int size = m_recvPackets.size();
	for (int i = 0; i < size; i++) {
		std::string pack;
		m_completeMutex.lock();
		if (m_recvPackets.size() > 0) {
			pack = m_recvPackets.front();
			m_recvPackets.pop_front();
		}
		m_completeMutex.unlock();

		if (pack != "") {
			m_delegate->onServerMessage((void*)pack.c_str());
		}
	}

    /*
    //pthread_mutex_lock(&m_completeMutex);
    while (m_recvPackets.size() > 0)
    {
		m_completeMutex.lock();
        std::string pack = m_recvPackets.front();
        m_recvPackets.pop_front();
		m_completeMutex.unlock();
        
        m_delegate->onServerMessage( (void*)(pack.c_str()) );
        //break;
    }
    //pthread_mutex_unlock(&m_completeMutex);
    */
    
    //pthread_mutex_lock(&m_completeMutex);
//    while (!m_recvStream.empty())
//    {
//        auto prot = (ProtHead*)(m_recvStream.c_str());
//        int plen = prot->len;
//        int slen = m_recvStream.length();
//        if (plen <= slen) {
//            std::string packStr = m_recvStream.substr(0, plen);
//            m_delegate->onServerMessage( (void*)(packStr.c_str()) );
//            m_recvStream.erase(0, plen);
//        } else {
//            break;
//        }
//    }
    //pthread_mutex_unlock(&m_completeMutex);
}

void Server::send(void* data, int nDataLen)
{
    sendByThread((char*)data, nDataLen);
}

void Server::sendd(void* data, int nDataLen)
{
    if (m_socket != NULL) {
        m_socket->Send((const char*)data, nDataLen);
    }
}

void Server::disconnect()
{
    m_delegate->onServerDisconnect();
}

void Server::stop()
{
    Thread::close();
    
    stopListen();
    
    if (m_socket)
    {
        m_socket->Close();
        delete  m_socket;
        m_socket = NULL;
    }
    
    if (s_Server != NULL) {
        delete  s_Server;
        s_Server = NULL;
    }
}

void Server::stopListen()
{
    if (m_listenSocket)
    {
        m_listenSocket->Close();
        delete m_listenSocket;
        m_listenSocket = NULL;
    }
}



