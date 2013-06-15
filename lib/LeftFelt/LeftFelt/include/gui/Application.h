#ifndef APPLICATIONCLASS_INCLUDE
#define APPLICATIONCLASS_INCLUDE

#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <gui/WindowClass.h>
#include <algorithm>

class Application{
private:
	std::vector<Window *> window_list;
	HINSTANCE hInst; //アプリケーションのインスタンス
protected:
	Window* createWindow(std::string id = "main");
	HINSTANCE getInstanceHandle();
public:
	Application();
	~Application();
	void exit();
	virtual void main() = 0;
	int winMain(HINSTANCE hInst);
};

#endif