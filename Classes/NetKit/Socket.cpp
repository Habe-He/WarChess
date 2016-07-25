#include "Socket.h"
#include <stdio.h>
#include <string.h>
#include <mutex>

#ifdef WIN32
	#pragma comment(lib, "wsock32")
#endif

//static pthread_mutex_t m_mutex;
static bool m_mutex_init = false;

Socket::Socket(SOCKET sock)
{
	m_sock = sock;
    
    if (!m_mutex_init) {
        //pthread_mutex_init(&m_mutex, NULL);
        m_mutex_init = true;
    }
}

Socket::~Socket()
{
}

int Socket::Init()
{
#ifdef WIN32
	/*
	http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	typedef struct WSAData { 
		WORD wVersion;								//winsock version
		WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
		char szDescription[WSADESCRIPTION_LEN+1]; 
		char szSystemStatus[WSASYSSTATUS_LEN+1]; 
		unsigned short iMaxSockets; 
		unsigned short iMaxUdpDg; 
		char FAR * lpVendorInfo; 
	}WSADATA, *LPWSADATA; 
	*/
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if ( ret ) {
//		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif
	
	return 0;
}
//this is just for windows
int Socket::Clean()
{
#ifdef WIN32
		return (WSACleanup());
#endif
		return 0;
}

Socket& Socket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

Socket::operator SOCKET ()
{
	return m_sock;
}

//create a socket object win/lin is the same
// af:
bool Socket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if ( m_sock == INVALID_SOCKET ) {
		return false;
	}
	return true;
}

bool Socket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
        ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
        if (ret == SOCKET_ERROR) {
            return false;
        }
	}
	return true;
}

bool Socket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svraddr.sin_port = htons(port);
    memset(&svraddr.sin_zero, '\0', 8); 

	int opt =  1;
	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 ) 
		return false;

	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool Socket::BindIP(char* ip, unsigned short port)
{
    struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
    memset(&svraddr.sin_zero, '\0', 8); 
    
	int opt =  1;
	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 ) 
		return false;
    
	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

//for server
bool Socket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool Socket::Accept(Socket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if ( sock == SOCKET_ERROR ) {
		return false;
	}

	s = sock;
	if ( fromip != NULL )
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int Socket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;

	while ( count < len )
    {
		bytes = send(m_sock, buf + count, len - count, flags);
		if ( bytes == -1 || bytes == 0 )
			return -1;
		count += bytes;
	}

	return count;
}

int Socket::Select(int timeout_ms)
{
    // set timeout
    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(m_sock, &rfds);
    //tv.tv_sec = sec;
    //tv.tv_usec = usec;
	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec = (timeout_ms % 1000) * 1000;
    int retval = select(m_sock + 1, &rfds, NULL, NULL, &tv);        
    
    return retval;
}

//int Socket::Select(int delay)
//{
//    return Select(delay, 0);
//}

int Socket::Recv(char* buf, int len, int flags)
{
	return (recv(m_sock, buf, len, flags));
}

int Socket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int Socket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}

bool Socket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ( (p = gethostbyname(domain)) == NULL )
		return false;
		
	sprintf(ip, "%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0], 
		(unsigned char)p->h_addr_list[0][1], 
		(unsigned char)p->h_addr_list[0][2], 
		(unsigned char)p->h_addr_list[0][3]);
	
	return true;
}

int Socket::GetHostName(char* hostname, int len)
{
	return gethostname(hostname, len);
}

bool Socket::GetHostAddr(char* hostname, unsigned char* ip)
{
	struct hostent* p;
	if ( (p = gethostbyname(hostname)) == NULL )
		return false;

	ip[0] = (unsigned char)p->h_addr_list[0][0];
	ip[1] = (unsigned char)p->h_addr_list[0][1];
	ip[2] = (unsigned char)p->h_addr_list[0][2];
	ip[3] = (unsigned char)p->h_addr_list[0][3];

	return true;
}

int Socket::Recvfrom(char* buf, int len, int flags, char* ip, unsigned short* port)
{
	struct sockaddr_in addr = {0}; 
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int ret = 0;
    int retval = 1;
    
    if (!flags) {
        // set timeout
        fd_set rfds;
        struct timeval tv;
        FD_ZERO(&rfds);
        FD_SET(m_sock, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(m_sock + 1, &rfds, NULL, NULL, &tv);        
    }

    if (retval == -1) {
        ret = -1;
    } else if (retval) {
        ret = recvfrom(m_sock, buf, len, 0, (sockaddr*)&addr, &addr_len);
        
        if (ip) strcpy(ip, ntoa(addr.sin_addr));
        if (port) *port = ntohs(addr.sin_port);
    } else {
        // timeout
        ret = -1;
    }

	return ret;
}

int Socket::Sendto(char* buff, int len, int flags, char* ip, unsigned short port)
{
	struct sockaddr_in addr;
	int ret = 0;

	addr.sin_family = AF_INET;
    if (ip) {
        addr.sin_addr.s_addr = inet_addr(ip);        
    } else {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
	addr.sin_port = htons(port);
    memset(&addr.sin_zero, '\0', 8);

	ret = sendto(m_sock, buff, len, flags, (sockaddr*)&addr, sizeof(struct sockaddr_in));
    
	return ret;
}

bool Socket::SetOption(int optname, const char* val, int optlen)
{
	if (setsockopt(m_sock, SOL_SOCKET, optname, val, optlen) < 0 )
	{
		return false;
	}

	return true;
}

int Socket::SendBroadcast(char* buff, int len, int flags, unsigned short port)
{
	struct sockaddr_in addr;
	int ret = 0;
    
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(port);
    memset(&addr.sin_zero, '\0', 8);

	ret = sendto(m_sock, buff, len, flags, (sockaddr*)&addr, sizeof(struct sockaddr_in));

	return ret;
}

int Socket::RecvBroadcast(char* buf, int len, int flags, char* ip, unsigned short* port)
{
	struct sockaddr_in addr = {0}; 
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int ret = 0;
    int retval = 1;
    
    if (!flags) {
        // set timeout
        fd_set rfds;
        struct timeval tv;
        FD_ZERO(&rfds);
        FD_SET(m_sock, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(m_sock + 1, &rfds, NULL, NULL, &tv);        
    }

    if (retval == -1) {
        ret = -1;
    } else if (retval) {
        ret = recvfrom(m_sock, buf, len, 0, (sockaddr*)&addr, &addr_len);
        if (ip) strcpy(ip, ntoa(addr.sin_addr));
        if (port) *port = ntohs(addr.sin_port);
    } else {
        // timeout
        ret = -1;
    }
    
	return ret;
}

void Socket::getHostIp(char* host, char* ip, int size, unsigned short* port)
{
    struct sockaddr addr = {0};
    struct sockaddr_in* addr_v4 = NULL;
    socklen_t addr_len = sizeof(addr);
    
    // get local ip and port

    if (!host) {
        if (0 == getsockname(m_sock, &addr, &addr_len))
        {
            if (addr.sa_family == AF_INET)
            {
                addr_v4 = (sockaddr_in*)&addr;
            }
        }
    } else {
        if (0 == getpeername(m_sock, &addr, &addr_len))
        {
            if (addr.sa_family == AF_INET)
            {
                addr_v4 = (sockaddr_in*)&addr;
            }
        }
    }
    
    if (addr_v4) {
        strcpy(ip, ntoa(addr_v4->sin_addr));
        *port = ntohs(addr_v4->sin_port);        
    }
}

char* Socket::ntoa(struct in_addr addr)
{
    char* ret = NULL;
    //pthread_mutex_lock(&m_mutex);
    ret = inet_ntoa(addr);
    //pthread_mutex_unlock(&m_mutex);
    
    return ret;
}

void Socket::setTimeout(int sec, int usec)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    
    /* Watch stdin (fd 0) to see when it has input. */
    
    FD_ZERO(&rfds);
    FD_SET(m_sock, &rfds);
    
    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    retval = select(m_sock + 1, &rfds, NULL, NULL, &tv);
}