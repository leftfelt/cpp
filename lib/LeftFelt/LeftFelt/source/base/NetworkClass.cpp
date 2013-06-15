#include <base/NetworkClass.h>

Network::Network(){
}

Network::~Network(){
	Close();
}

//TCP接続でオープン
Socket Network::OpenTCP(){
	SOCKET sock;
	Socket s;
#ifdef _WIN32
	if(WSAStartup(MAKEWORD(1,1),&wsaData) != 0){ // for windows
		throw "WSAStartup error\n";
	}
#endif
	sock = socket(AF_INET,SOCK_STREAM,0);
	s = sock;
	return s;
}
//UDP接続でオープン
Socket Network::OpenUDP(){
	SOCKET sock;
	Socket s;
#ifdef _WIN32
	if(WSAStartup(MAKEWORD(1,1),&wsaData) != 0){ // for windows
		throw "WSAStartup error\n";
	}
#endif
	sock = socket(AF_INET,SOCK_DGRAM,0);
	s = sock;
	return s;
}
void Network::Close(){
#ifdef _WIN32
	WSACleanup(); //for windows
#endif
}
