#ifndef HTTPCLASS_INCLUDE
#define HTTPCLASS_INCLUDE

#include <base/StringClass.h>
#include <base/NetworkClass.h>
#include <base/SocketClass.h>

/*
* HttpClass
*/

class Http{
private:
	Network network;
	Socket socket;
	String header;
	String host;
	String path;
	String port;
	String base_path;
	String GET(String host, String path);
	String POST(String host, String path, String data);
	String HEAD(String host, String path);
public:
	Http();
	~Http();
	String Request(String URL, String method = "GET", String data = "");
	void setHost(String host);
	void setPath(String path);
	void setPort(String port);
	void setURL(String URL);
	String getHost();
	String getPath();
	String getPort();
	String getURL();
	String getHeader();
};

#endif