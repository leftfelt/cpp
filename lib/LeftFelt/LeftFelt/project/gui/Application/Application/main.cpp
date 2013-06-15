#ifdef _DEBUG
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <gui/Application.h>

class Test : public Application{
private:
public:
	Test(){
		this->createWindow()->setTitle("main");
		this->createWindow("sub")->setTitle("sub");
	}
	void main(){
	}
};

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	Test test;
	test.winMain(hCurInst);
}
