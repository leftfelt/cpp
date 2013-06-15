#ifndef SOCKETCLASS_INCLUDE
#define SOCKETCLASS_INCLUDE

#include <string>
#ifdef _WIN32
	#include <WinSock2.h> // for windows
	#pragma comment(lib,"ws2_32.lib")//ライブラリの追加
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
	bool temp_flag;//戻り値フラグ
	u_short port; //ポート
	SOCKET sock; //ソケット
	SOCKADDR_IN sockadd;
	LPHOSTENT lpHost;
public:
	Socket();
	Socket(Socket& socket);
	~Socket();
	void Connect(std::string server,int port); //接続
	void disConnect(); //切断
	void Search(std::string server);//ホストを検索
	void Shutdown();//送受信を停止

	void Bind(int port);
	void Listen();//接続待機
	Socket Accept();//接続を許可

	void Send(std::string data);//送信
	void Recv(std::string &data, int size = 1024, bool recv_all_flag = true);//受信

	//演算子
	Socket operator=(SOCKET& socket);
	//Socket operator=(Socket& socket);
};

#endif
