#ifndef SOCKETCLASS_INCLUDE
#define SOCKETCLASS_INCLUDE

#include <string>
#ifdef _WIN32
	#include <WinSock2.h> // for windows
	#pragma comment(lib,"ws2_32.lib")//���C�u�����̒ǉ�
#else
	//for linux
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
	#define INVALID_SOCKET 0
	#define SD_BOTH 0x02
	#define SOCKET_ERROR -1
	typedef unsigned int SOCKET;  //SOCKET
	typedef struct sockaddr SOCKADDR;
	typedef struct in_addr* LPIN_ADDR;
	typedef SOCKADDR* PSOCKADDR;
	typedef struct sockaddr_in SOCKADDR_IN; //SOCKADDR_IN
	typedef struct hostent* LPHOSTENT; //LPHOSTENT
#endif

/*
* SocketClass
* @ port
* @ ipaddr
* @ type : TCP/UDP
*/

class Socket{
private:
	bool temp_flag;//�߂�l�t���O
	u_short port; //�|�[�g
	SOCKET sock; //�\�P�b�g
	SOCKADDR_IN sockadd;
	LPHOSTENT lpHost;
public:
	Socket();
	Socket(Socket& socket);
	~Socket();
	void Connect(std::string server,int port); //�ڑ�
	void disConnect(); //�ؒf
	void Search(std::string server);//�z�X�g������
	void Shutdown();//����M���~

	void Bind(int port);
	void Listen();//�ڑ��ҋ@
	Socket Accept();//�ڑ�������

	void Send(std::string data);//���M
	void Recv(std::string &data, int size = 1024, bool recv_all_flag = true);//��M

	//���Z�q
	Socket operator=(SOCKET& socket);
	//Socket operator=(Socket& socket);
};

#endif
