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
//�ڑ�
void Socket::Connect(std::string server, int port){
	Search(server);
	memset(&sockadd,0,sizeof(SOCKADDR_IN));
	sockadd.sin_family = AF_INET;
	sockadd.sin_port = htons(port);
	sockadd.sin_addr = *((LPIN_ADDR)*lpHost->h_addr_list);
	if(connect(sock,(PSOCKADDR)&sockadd, sizeof(sockadd)) != 0){
		throw "connection failred";//�ڑ��Ɏ��s
	}
}

//�ؒf
void Socket::disConnect(){
	Shutdown();
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}

//����M���~
void Socket::Shutdown(){
	//Recv���Ă��Ȃ��f�[�^���c���Ă���ꍇ���s����B
	if(shutdown(sock,SD_BOTH != 0)){
		throw "shutdown error\n"; //�@�ؒf�Ɏ��s
	}
}
//���M
void Socket::Send(std::string data){
	send(sock, data.c_str(), data.size(),0);
}
//��M
/**
* @param string &data : �S��M�f�[�^
* @param int size : ��x�Ɏ�M����f�[�^�T�C�Y
* @param bool recv_all_flag :  �S�f�[�^����M���邩
**/
void Socket::Recv(std::string &data, int size, bool recv_all_flag){
	int response_size = -1;
	char* recv_data = (char*)malloc(sizeof(char)*size);

	data.clear();

	do{
		//���ׂẴf�[�^����M����܂Ń��[�v
		recv_data = (char*)memset(recv_data,0,sizeof(char)*size);
		response_size = recv(sock, recv_data, size,0);//size������M
		if(response_size == SOCKET_ERROR){
			throw "recieve error\n";
		}
		data += recv_data;
	}while(response_size != 0 && recv_all_flag);

	free(recv_data);
}
//�z�X�g������
void Socket::Search(std::string server){
	lpHost = gethostbyname(server.c_str());
	if(lpHost == NULL){
		unsigned int addr = inet_addr(server.c_str());
		lpHost = gethostbyaddr((char *)&addr, 4, AF_INET);
		if(lpHost == NULL){;
			throw "server not found\n";//�T�[�o�[��������Ȃ�
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
//�ڑ��ҋ@
void Socket::Listen(){
	if(listen(sock,0) == SOCKET_ERROR){
		throw "listen error\n";
	}
}
//�ڑ�������
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
//���Z�q
Socket Socket::operator=(SOCKET &socket){
	this->sock = socket;
	return *this;
}

/*
Socket Socket::operator=(Socket &socket){
	this->sock = socket.sock;
	return *this;
}*/
