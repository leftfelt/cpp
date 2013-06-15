#include <base/HttpClass.h>


Http::Http(){
	//HTTP,HTTPS�w�b�_�[
	this->header = "http://";
	//this->header = "https://";//TODO ���̂����Ή�
}
Http::~Http(){
}
//GET���\�b�h
String Http::GET(String host, String path){
	String protocol;
	protocol = "GET "+path+" HTTP/1.0\r\n\r\n";
	return protocol;
}
//POST���\�b�h
String Http::POST(String host, String path, String data){
	String protocol;
	String length;
	length = (long)data.length();
	protocol = 
		"POST "+path+" HTTP/1.0\r\n"+
		"Content-Type: application/x-www-form-urlencoded\r\n"+
		"Content-Length:"+length+
		"\r\n\r\n"+data;
	return protocol;
}
String Http::HEAD(String host, String path){
	String protocol = "";
	protocol = "HEAD "+path+" HTTP/1.0\r\n\r\n";
	return protocol;
}

//�T�[�o��HTTP���N�G�X�g�𑗐M�ヌ�X�|���X��Ԃ�
String Http::Request(String URL, String method, String data){
	String request;
	String response;

	this->setURL(URL);
	//�x�[�X�p�X���X�V����
	this->base_path = this->getPath().EndSplit('/')[0];

	//���N�G�X�g�𑗐M========================================
	try{
		this->socket = this->network.OpenTCP();
		this->socket.Connect(this->getHost(),atoi(port.c_str()));//�ڑ�
		
		//���\�b�h�I��
		if(method == "GET"){
			request = GET(this->getHost(),this->getPath());
		}else if(method == "POST"){
			request = POST(this->getHost(),this->getPath(),data);
		}else if(method == "HEAD"){
			request = HEAD(this->getHost(),this->getPath());
		}
		
		//�f�[�^����M
		this->socket.Send(request);
		this->socket.Recv(response);

		this->socket.disConnect();
		this->network.Close();
	}catch(char* mes){
		puts(mes);
	}

	return response;
}

void Http::setHost(String host){
	host.FrontStrip(' ');
	host.EndStrip(' ');
	this->host = host;
	this->base_path = "";
}

void Http::setPath(String path){
	StringList list; 
	
	path.FrontStrip(' ');
	path.EndStrip(' ');

	if(path.Front() != "/") {
		path = "/"+path;
		this->path = this->base_path;
	}else{
		this->path = "";
	}
	
	//���΃p�X�Ȃ��΃p�X��
	while(path != ""){
		path = path.substr(1);
		list = path.FrontSplit('/');
		path = list[1];
		if(list[0] == ".."){
			this->path = this->path.EndSplit('/')[0];
		}else if(list[0] == ""){
			path = "/";
			break;
		}else{
			this->path += "/"+list[0];
		}
	}
 	this->path += path;
}
void Http::setPort(String port){
	port.FrontStrip(' ');
	port.EndStrip(' ');
	this->port = port;
}
void Http::setURL(String URL){
	
	std::string::size_type index = 0;
	
	this->header = "http://";
	index = URL.find(this->getHeader());
	if(index == std::string::npos){
		this->header = "https://";
		index = URL.find(this->getHeader());
	}
	//�w�b�_�[�������������΃p�X
	if(index != std::string::npos){
		URL = URL.substr(index+this->header.size());

		//URL����z�X�g���E�p�X�E�|�[�g�ԍ����擾================
		index = URL.find("/");
		if(index == std::string::npos){
			this->setHost(URL);
			this->setPath("/");
		}else{
			this->setHost(URL.substr(0,index));
			this->setPath(URL.substr(index));
		}
	}else{
		//������Ȃ������瑊�΃p�X
		this->setPath(URL);
	}
	
	index = host.find_first_of(":");
	if(index == std::string::npos){
		//�|�[�g�ԍ��̓f�t�H���g��80���g��
		this->setPort("80");
	}else{
		this->setPort(this->host.substr(index+1));
	}

}

String Http::getHeader(){
	return this->header;
}

String Http::getHost(){
	return this->host;
}

String Http::getPath(){
	return this->path;
}

String Http::getURL(){
	//��΃p�X�ɕϊ�����
	return this->getHeader() + this->getHost() + this->getPath();
}

String Http::getPort(){
	return this->port;
}