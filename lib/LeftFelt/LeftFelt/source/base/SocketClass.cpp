#include <base/SocketClass.h>

Socket::Socket(){
	sock = 0;
}

Socket::Socket(Socket& socket){
	this->sock = socket.sock;
}

Socket::~Socket(){
}

//================================================================================
//接続
void Socket::Connect(std::string server, int port){
	Search(server);
	memset(&sockadd,0,sizeof(SOCKADDR_IN));
	sockadd.sin_family = AF_INET;
	sockadd.sin_port = htons(port);
	sockadd.sin_addr = *((LPIN_ADDR)*lpHost->h_addr_list);
	if(connect(sock,(PSOCKADDR)&sockadd, sizeof(sockadd)) != 0){
		throw "connection failred";//接続に失敗
	}
}

//切断
void Socket::disConnect(){
	Shutdown();
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}

//送受信を停止
void Socket::Shutdown(){
	//Recvしていないデータが残っている場合失敗する。
	if(shutdown(sock,SD_BOTH != 0)){
		throw "shutdown error\n"; //　切断に失敗
	}
}
//送信
void Socket::Send(std::string data){
	send(sock, data.c_str(), data.size(),0);
}
//受信
/**
* @param string &data : 全受信データ
* @param int size : 一度に受信するデータサイズ
* @param bool recv_all_flag :  全データを受信するか
**/
void Socket::Recv(std::string &data, int size, bool recv_all_flag){
	int response_size = -1;
	char* recv_data = (char*)malloc(sizeof(char)*size);

	data.clear();

	do{
		//すべてのデータを受信するまでループ
		recv_data = (char*)memset(recv_data,0,sizeof(char)*size);
		response_size = recv(sock, recv_data, size,0);//sizeだけ受信
		if(response_size == SOCKET_ERROR){
			throw "recieve error\n";
		}
		data += recv_data;
	}while(response_size != 0 && recv_all_flag);

	free(recv_data);
}
//ホストを検索
void Socket::Search(std::string server){
	lpHost = gethostbyname(server.c_str());
	if(lpHost == NULL){
		unsigned int addr = inet_addr(server.c_str());
		lpHost = gethostbyaddr((char *)&addr, 4, AF_INET);
		if(lpHost == NULL){;
			throw "server not found\n";//サーバーが見つからない
		}
	}
}

//================================================================================
void Socket::Bind(int port){
	memset(&sockadd,0,sizeof(SOCKADDR_IN));
	sockadd.sin_family = AF_INET;
	sockadd.sin_port = htons(port);
	sockadd.sin_addr.s_addr = INADDR_ANY;
	if(bind(sock,(struct sockaddr *)&sockadd, sizeof(sockadd)) == SOCKET_ERROR){
		disConnect();
		throw "bind error\n";
	}
}
//接続待機
void Socket::Listen(){
	if(listen(sock,0) == SOCKET_ERROR){
		throw "listen error\n";
	}
}
//接続を許可
Socket Socket::Accept(){
	Socket s;
	s.temp_flag = true;//
#ifdef _WIN32
	int len = sizeof(sockadd);
#else
	socklen_t len = sizeof(sockadd);
#endif
	s.sock = accept(sock,(SOCKADDR *)&sockadd, &len);
	if(s.sock == INVALID_SOCKET){
		disConnect();
		throw "accept error\n";
	}
	return s;
}

//================================================================================
//演算子
Socket Socket::operator=(SOCKET &socket){
	this->sock = socket;
	return *this;
}

/*
Socket Socket::operator=(Socket &socket){
	this->sock = socket.sock;
	return *this;
}*/
