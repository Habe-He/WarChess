//
//  Thread.cpp
//  ddz
//
//  Created by partygame on 13-5-3.
//
//

#include "Thread.h"
#include "NetMessage.h"

Thread::Thread()
{
    m_needQuit = false;
    m_bInit = false;

	m_thread = nullptr;
	m_threadSend = nullptr;
}

Thread::~Thread() {
    m_needQuit = true;
}

void Thread::sendByThread(char *data, int nDataLen)
{
	if (m_needQuit)
		return;
    
    std::string sendstr;
    sendstr.append(data, nDataLen);
	m_sendMutex.lock();
    m_sendPackets.push_back(sendstr);
	m_sendMutex.unlock();
    
}

int Thread::sendProcess(void* data)
{
    if (m_needQuit) {
        return 1;
    }
    
	int spSize = (int)m_sendPackets.size();
	if (spSize == 0) {
        return 0;
    }
    
	for (int i = 0; i < spSize; i++) {
		std::string sendStr;
		m_sendMutex.lock();
		if (m_sendPackets.size() > 0) {
			sendStr = m_sendPackets.front();
			m_sendPackets.pop_front();
		}
		m_sendMutex.unlock();

		if (sendStr != "") {
			int size = (int)sendStr.size();
			if (m_socket != nullptr) {
				int slen = m_socket->Send(sendStr.c_str(), size);
				if (slen != size) {
					printf("[===] sended len != sendStream length \n");
				}
			}
		}
	}

    return 0;
}

int Thread::threadProcess(void* data)
{
    if (m_needQuit) {
        return 1;
    }
    
    // receive packet
	if (m_socket == nullptr) {
		return 1;
	}

    int ret = m_socket->Select(300);
    if (ret > 0) {
        //memset(dp->content, 0x00, MAX_DATA_PACK_LEN);
		memset(m_recvBuff, 0, sizeof(m_recvBuff));
        int len = m_socket->Recv(m_recvBuff, MAX_DATA_PACK_LEN, 0);
        if (len > 0) {
			m_recvDataPool.append(m_recvBuff, len);
            
            //int dolen = 0;
			size_t size_h = sizeof(ProtHead);
			while (m_recvDataPool.size() >= size_h) {
                auto prot = (ProtHead*)m_recvDataPool.c_str();
				if (prot == nullptr)
					break;

				if (m_recvDataPool.size() >= prot->len) {
					std::string packageStr = m_recvDataPool.substr(0, prot->len);
					m_recvDataPool.erase(0, prot->len);
                    m_completeMutex.lock();
					m_recvPackets.push_back(packageStr);
                    m_completeMutex.unlock();
                } else {
                    break;
                }
            }
        }
        
        if (ret == 1 && len == 0) {
            return 2;
        }
    }
    
    return 0;
}

void Thread::leaveProcess()
{
    if (m_bInit)
    {
//        pthread_mutex_destroy(&m_sendMutex);
        //pthread_mutex_destroy(&m_completeMutex);
        m_bInit = false;
    }
    m_leaved = true;
}

bool Thread::init(ThreadProcess process, ThreadProcess sendProc, void* data)
{
    if (!m_bInit)
    {
        m_bInit = true;
//        pthread_mutex_init(&m_sendMutex, NULL);
        //pthread_mutex_init(&m_completeMutex, NULL);
        //pthread_create(&m_thread, NULL, process, data);
        m_sendPackets.clear();
		m_recvPackets.clear();
        
		if (m_thread != nullptr) {
			CC_SAFE_DELETE(m_thread);
		}
		m_thread = new std::thread(process, data);

        if (sendProc != NULL) {
			if (m_threadSend != nullptr) {
				CC_SAFE_DELETE(m_threadSend);
			}

			m_threadSend = new std::thread(sendProc, data);
            //pthread_create(&m_threadSend, NULL, sendProc, data);
        }
        m_needQuit = false;
    }
    
    return true;
}

void Thread::close()
{
    m_needQuit = true;
    int num = 0;
    while (m_bInit) {
        printf("Thread::close, waiting...\n");
        usleep2(100000);
        if (num++ > 10) {
            break;
        }
    }

	m_recvPackets.clear();
	m_sendPackets.clear();
	
	m_sendStream.clear();
	m_recvStream.clear();

	memset(m_recvBuff, 0, sizeof(m_recvBuff));
}

void usleep2(long msTimes)
{
	//Sleep(msTimes / 1000);
	std::this_thread::sleep_for(std::chrono::microseconds(msTimes));
}
