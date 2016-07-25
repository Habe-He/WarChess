
#ifndef __SOCKET_H__
#define __SOCKET_H__

#ifdef WIN32
	#include <winsock2.h>
	typedef int				socklen_t;
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include "errno.h"
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	typedef int				SOCKET;

	//#pragma region define win32 const variable in linux
	#define INVALID_SOCKET	-1
	#define SOCKET_ERROR	-1
	//#pragma endregion
#endif

//#include "pthread.h"

class Socket {
public:
	Socket(SOCKET sock = INVALID_SOCKET);
	~Socket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* ip, unsigned short port);
	//#region server
	// Bind socket
	bool Bind(unsigned short port);
    
    bool BindIP(char* ip, unsigned short port);

	// Listen socket
	bool Listen(int backlog = 5); 

	// Accept socket
	bool Accept(Socket& s, char* fromip = NULL);
	//#endregion
    
    int Select(int timeout_ms);
    
    //int Select(int sec, int usec);
	
	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);
	
	// Close socket
	int Close();

	// Get errno
	int GetError();
	
	//#pragma region just for win32
	// Init winsock DLL 
	static int Init();	
	// Clean winsock DLL
	static int Clean();
	//#pragma endregion

	// Domain parse
	static bool DnsParse(const char* domain, char* ip);

	// get host name
	int GetHostName(char* hostname, int len);

	// get host addr
	bool GetHostAddr(char* hostname, unsigned char* ip);

	// recvfrom
	int Recvfrom(char* buf, int len, int flags, char* ip, unsigned short* port);

	// sendto
	int Sendto(char* buff, int len, int flags, char* ip, unsigned short port); 

	// set opt
	bool SetOption(int optname, const char* val, int optlen);

	// send boardcast
	int SendBroadcast(char* buff, int len, int flags, unsigned short port);

	// recv boardcase
	int RecvBroadcast(char* buf, int len, int flags, char* ip, unsigned short* port);
    
    
    void getHostIp(char* host, char* ip, int size, unsigned short* port);
    
    char* ntoa(struct in_addr addr);
    
    void setTimeout(int sec, int usec);

	Socket& operator = (SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;

};

#endif
