#ifndef INCLUDE_WINDOWCLASS
#define INCLUDE_WINDOWCLASS

#include <gui/WindowClass.h>
#include <string>
#include <vector>
#include <gui/ObjectClass.h>

//Windowクラス
class Window;

//WindowObjectクラス
class WindowObject : public Object{
protected:
	Window *wnd;
public:
	WindowObject();
	bool SetWindow(Window &wnd);
	std::string ParentWindow();
};

//Windowクラス
class Window {
private:
	std::string id;	//ウィンドウID
	std::string title;	//タイトル名

	int style;	//スタイル

	int width; //幅
	int height;//高さ

	ATOM InitApp(HINSTANCE);
	BOOL InitInstance(HINSTANCE, int);
	std::vector<WindowObject*> obj;	//登録されているオブジェクト
	static std::vector<Window *> window_list; 
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	static HINSTANCE hInst; //アプリケーションのインスタンス
	HWND hWnd; //ウィンドウハンドル
	HDC hdc; //コンテキストハンドル

	Window(std::string id="main");	//コンストラクタ
	~Window();

	void setStyle(int style);		//スタイル
		
	void setSize(int width, int height);	//ウィンドウのサイズを設定
	void setPos(int x, int y); //位置を設定

	void setTitle(std::string title);	//ウィンドウのタイトルを設定
	std::string getTitle();		//ウィンドウのタイトルを取得
	void setID(std::string id);		//ウィンドウの識別IDを設定
	std::string getID();	//ウィンドウの識別IDを取得
	int getWidth();	//幅を取得
	void setWidth(int width);	//幅を設定
	int getHeight();	//高さを取得
	void setHeight(int height);	//高さを設定
	
	void AddObject(WindowObject *obj);		//オブジェクトを登録する
	void DeleteObject(WindowObject *obj);		//オブジェクトを削除する

	void hide();
	void show();

	bool isVisible();
	bool isDeleted();
	bool isActive();

	bool Initialize();
	void Update();
	void Draw();
	void Delete();
};

#endif