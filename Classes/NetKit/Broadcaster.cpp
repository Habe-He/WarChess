//
//  Broadcaster.cpp
//  ParkourGame
//
//  Created by haibo yin on 4/16/14.
//
//

#include "Broadcaster.h"
#include <string>

static void* BroadcastThreadProcess(void* data)
{
    while (true) {
//        if (BROADCASTER()->threadProcess(data) != 0) {
//            BROADCASTER()->leaveProcess();
//            break;
//        }
        if (!BROADCASTER()->socketProcess()) {
            BROADCASTER()->leaveProcess();
            break;
        }
        usleep2(1000000);
    }
    
    return NULL;
}

Broadcaster::Broadcaster()
{
    m_socket = NULL;
    m_delegate = NULL;
}

static Broadcaster* s_Broadcaster = NULL;

Broadcaster* Broadcaster::getInstance()
{
    if (!s_Broadcaster) {
        s_Broadcaster = new Broadcaster();
    }
    return s_Broadcaster;
}

void Broadcaster::initial(unsigned short port)
{
    m_port = port;
    
    m_socket = new Socket();
    m_socket->Create(AF_INET, SOCK_DGRAM);
    m_socket->Bind(m_port);
    int opt = 1;
    m_socket->SetOption(SO_BROADCAST, (char*)&opt, sizeof(opt));
    
    //init(BroadcasterThreadProcess, NULL);
}

void Broadcaster::startServer(unsigned short port, const char *name)
{
    m_isServer = true;
    strcpy(m_name, name);
    initial(port);
    
    init(BroadcastThreadProcess, NULL, NULL);
}

void Broadcaster::changeName(const char* name) {
    strcpy(m_name, name);
}

void Broadcaster::startClient(unsigned short port, BroadcastDelegate* delegate)
{
    m_isServer = false;
    m_delegate = delegate;
    initial(port);
    
    init(BroadcastThreadProcess, NULL, NULL);
}

void Broadcaster::stop()
{
    Thread::close();
    
    //printf("stop m_needQuit = %d", (int)m_needQuit);
    if (m_socket)
    {
        m_socket->Close();
        
        delete m_socket;
        m_socket = NULL;
        m_delegate = NULL;
        //printf(" set m_socket nullptr .......");
    }
    
    delete s_Broadcaster;
    s_Broadcaster = NULL;
}

bool Broadcaster::socketProcess()
{
    if (m_needQuit || !m_socket) {
        return false;
    }
    
    if (m_isServer)
    {
        m_socket->SendBroadcast(m_name, sizeof(m_name), 0, m_port);
    }
    else
    {
        char recvBuffer[256] = {0};
        char ip[64] = {0};
        unsigned short port = 0;
        
        if (m_socket != nullptr) {
            if (m_socket->RecvBroadcast(recvBuffer, sizeof(recvBuffer), 0, ip, &port) != -1) {
                m_delegate->onFindServer((const char*)recvBuffer, ip, port);
            }
        }else {
            //printf("socketProcess m_needQuit = %d", m_needQuit);
            //printf("m_socket = nullptr .....1");
        }
    }
    return true;
}

