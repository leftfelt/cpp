#include <gui/Application.h>

Application::Application(){

}

Application::~Application(){
}

Window* Application::createWindow(std::string id){
	Window *window = new Window(id);
	this->window_list.push_back(window);
	return window;
}

HINSTANCE Application::getInstanceHandle(){
	return this->hInst;
}

void Application::exit(){
	PostQuitMessage(0);
}

int Application::winMain(HINSTANCE hInst){

	this->hInst = hInst;
	Window::hInst = hInst;
	MSG msg; //メッセージ

	while(true){
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)){
			if(!GetMessage(&msg,NULL,0,0))break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//閉じたウィンドウを削除
		this->window_list.erase(
			std::remove_if(this->window_list.begin(), this->window_list.end(),[](Window *window)->bool{
				return window->isDeleted();
			}),
			this->window_list.end()
		);
		if((signed)this->window_list.size() == 0)PostQuitMessage(0);

		this->main();

		std::for_each(this->window_list.begin(), this->window_list.end(),[](Window *window){
			window->Update();
			window->Draw();
		});
	}

	return (int)msg.wParam;
}
