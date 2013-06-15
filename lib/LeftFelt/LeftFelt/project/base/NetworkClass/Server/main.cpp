#include <iostream>
#include <string>
#include <base/NetworkClass.h>
#include <base/SocketClass.h>

void main(){
	Network net;
	
	Socket listen;
	Socket conn;

	std::string str;
	try{
		listen = net.OpenTCP();
		listen.Bind(12345);
		listen.Listen();
		conn = listen.Accept();
		listen.disConnect();
		conn.Recv(str,1024);
		puts(str.c_str());
		puts(str.c_str());
		conn.Send("m");
	}catch(char* mes){
		puts(mes);
	}

}