#ifndef NETWORKCLASS_INCLUDE
#define NETWORKCLASS_INCLUDE

#include <string>
#include <base/SocketClass.h>
#ifdef _WIN32
	#include <WinSock2.h> // for windows
	#pragma comment(lib,"ws2_32.lib")//ライブラリの追加
#endif

/*
* NetworkClass
* @ port
* @ ipaddr
* @ type : TCP/UDP
*/

class Network{
private:
#ifdef _WIN32
	WSADATA wsaData; //for windows
#endif
public:
	Network();
	~Network();
	Socket OpenTCP(); //TCP接続
	Socket OpenUDP(); //UDP接続
	void Close();
};

#endif
