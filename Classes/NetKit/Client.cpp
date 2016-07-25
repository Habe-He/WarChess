//
//  Client.cpp
//  ParkourGame
//
//  Created by haibo yin on 4/18/14.
//
//

#include "Client.h"
#include <string.h>
#include "NetMessage.h"

static void* ClientThreadProcess(void* data)
{
    int ret = 0;
    while (true) {
        ret = CLIENT()->threadProcess(data);
        if (ret == 1) {
            CLIENT()->leaveProcess();
            break;
        } else if (ret == 2) {
            CLIENT()->leaveProcess();
            CLIENT()->disconnect();
            break;
        }
        usleep2(10000);
    }
    
    return NULL;
}

static void* ClientSendThreadProcess(void* data)
{
    while (true) {
        if (CLIENT()->sendProcess(data) != 0) {
            break;
        }
        usleep2(10000);
    }
    
    return NULL;
}

Client::Client()
{
    m_socket = NULL;
    m_delegate = NULL;
    m_leaved = false;
}

static Client* s_Client = NULL;

Client* Client::getInstance()
{
    if (!s_Client) {
        s_Client = new Client();
    }
    return s_Client;
}

void Client::update(float dt)
{
    if (m_leaved) {
        return;
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
			m_delegate->onClientMessage((void*)(pack.c_str()));
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
        
        m_delegate->onClientMessage( (void*)(pack.c_str()) );
        //break;
    }
    //pthread_mutex_unlock(&m_completeMutex);
    */
    
    
    //pthread_mutex_lock(&m_completeMutex);
//    while (!m_recvStream.empty()),
//    {
//        auto prot = (ProtHead*)(m_recvStream.c_str());
//        int plen = prot->len;
//        int slen = m_recvStream.length();
//        if (plen <= slen) {
//            std::string packStr = m_recvStream.substr(0, plen);
//            m_delegate->onClientMessage( (void*)(packStr.c_str()) );
//            m_recvStream.erase(0, plen);
//        } else {
//            break;
//        }
//    }
    //pthread_mutex_unlock(&m_completeMutex);
}

bool Client::start(const char *ip, unsigned short port, ClientDelegate* delegate)
{
    if (m_socket) {
        stop();
    }
    
    m_delegate = delegate;
    m_leaved = false;
    m_socket = new Socket();
    bool succes = m_socket->Create(AF_INET, SOCK_STREAM);
    bool succeed = m_socket->Connect(ip, port);
    
    init(ClientThreadProcess, ClientSendThreadProcess, NULL);
    
    return succeed && succes;
}

void Client::stop()
{
    Thread::close();
    
    if (m_socket)
    {
        m_socket->Close();
        delete m_socket;
        m_socket = NULL;
    }
    
    if (s_Client != NULL) {
        delete s_Client;
        s_Client = NULL;
    }
}

void Client::disconnect()
{
    m_delegate->onClientDisconnect();
}

void Client::send(void* data, int nDataLen)
{
    sendByThread((char*)data, nDataLen);
}

void Client::sendd(void* data, int nDataLen)
{
    if (m_socket != NULL) {
        m_socket->Send((const char*)data, nDataLen);
    }
}
