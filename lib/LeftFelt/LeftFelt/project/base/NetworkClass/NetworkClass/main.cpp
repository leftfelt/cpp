#include <iostream>
#include <string>
#include <base/NetworkClass.h>
#include <base/SocketClass.h>


int main(){
	Network net;
	Socket client;
	std::string str;

	try{
		client = (net.OpenTCP());
		client.Connect("localhost",12345);
		client.Send("m");
		client.Recv(str,1024);
		puts(str.c_str());
		puts(str.c_str());
	}catch(char* mes){
		puts(mes);
	}
	return 0;
}
