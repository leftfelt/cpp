#include "gui/WindowClass.h"

WindowObject::WindowObject(){
	this->wnd = NULL;
}

//実行するウィンドウを設定する。
bool WindowObject::SetWindow(Window &wnd){
	if(this->wnd == NULL){
		this->wnd = &wnd;
		return true;
	}else{
		this->wnd->DeleteObject(this);
		this->wnd = &wnd;
		return true;
	}
	return false;
}

//親ウィンドウIDを取得
std::string WindowObject::ParentWindow(){
	return this->wnd->getID();
}

//==============================================================================
//アプリケーションのインスタンスハンドル
HINSTANCE Window::hInst = 0;

//コンストラクタ
Window::Window(std::string id){
	this->id = id;
	this->style = WS_OVERLAPPEDWINDOW;
	this->setWidth(640);
	this->setHeight(480);

	this->Initialize();
}

Window::~Window(){
	this->Delete();
}

//大きさを設定
void Window::setSize(int width, int height){
	RECT rc;
	this->setWidth(width);
	this->setHeight(height);
	
	SetRect(&rc, 0, 0, width, height);
	AdjustWindowRect( &rc, this->style , FALSE );

	SetWindowPos(this->hWnd,HWND_TOP,0,0,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE);//ウィンドウサイズを変更
}

//位置を設定
void Window::setPos(int x, int y){
	SetWindowPos(this->hWnd,NULL,x,y,0,0,SWP_NOSIZE); //ウィンドウの位置を変更
}

//スタイル
void Window::setStyle(int style){
	this->style = style;
	SetWindowLong(this->hWnd,GWL_STYLE, this->style);
	SetWindowPos(this->hWnd,HWND_BOTTOM,0,0,0,0,( SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER));
}

//IDを設定
void Window::setID(std::string id){
	this->id = id;
}
//ウィンドウの識別IDを取得
std::string Window::getID(){
	return this->id;
}

//タイトル名を設定
void Window::setTitle(std::string title){
	this->title = title;
	SetWindowText(this->hWnd,title.c_str());//タイトルを変更
}
//ウィンドウのタイトルを取得
std::string Window::getTitle(){
	return this->title;
}

//幅を取得
int  Window::getWidth(){
	return this->width;
}
//幅を設定
void  Window::setWidth(int width){
	this->width = width;
}

//高さ取得
int  Window::getHeight(){
	return this->height;
}
//高さ設定
void  Window::setHeight(int height){
	this->height = height;
}

//オブジェクトを追加
void Window::AddObject(WindowObject *obj){
	if(obj->SetWindow(*this)){
		this->obj.push_back(obj);
	}
	obj->Initialize();
}

//オブジェクトを削除
void Window::DeleteObject(WindowObject *obj){
	this->obj.size();
	if(this->obj.empty() == true){
		std::vector<WindowObject*>::iterator itr;
		for( itr=this->obj.begin() ; itr != this->obj.end() ; itr++ ){
			if(*itr == obj)break;
		}
		this->obj.erase(itr);
	}
}

void Window::hide(){
	ShowWindow(this->hWnd,SW_HIDE);
	UpdateWindow(this->hWnd);
}

void Window::show(){
	ShowWindow(this->hWnd,SW_SHOW);
	UpdateWindow(this->hWnd);
}



bool Window::isVisible(){
	return IsWindowVisible(this->hWnd) == TRUE; 
}
bool Window::isDeleted(){
	return IsWindowEnabled(this->hWnd) == FALSE;
}
bool Window::isActive(){
	return GetActiveWindow() == this->hWnd;
}

//===============================================================================================
//初期処理
bool Window::Initialize(){
	HDC hdc;
	HBITMAP hBmp_mem;

	if(!InitApp(this->hInst))
		return false;
	if(!InitInstance(this->hInst, SW_SHOWNORMAL))
		return false;
	hdc = GetDC(this->hWnd);

	//描画領域を作成
	this->hdc = CreateCompatibleDC(hdc);
	hBmp_mem = CreateCompatibleBitmap(hdc,this->getWidth(),this->getHeight());
	SelectObject(this->hdc, hBmp_mem);
	//白で塗りつぶす
	PatBlt(this->hdc,0,0,this->getWidth(),this->getHeight(),WHITENESS);
	ReleaseDC(this->hWnd, hdc);

	//ウィンドウプロシージャに自分へのポインタを渡す。
	SetWindowLongPtr(this->hWnd,GWL_USERDATA,(LONG)this);
	
	UpdateWindow(this->hWnd);

	return true;
}
//更新処理
void Window::Update(){
	for(int i=0 ; i < (signed)this->obj.size() ; i++){
		this->obj.at(i)->Update();
	}
}
//描画処理
void Window::Draw(){
	
	for(int i=0 ; i < (signed)this->obj.size() ; i++){
		this->obj.at(i)->Draw();
	}
	//ウィンドウプロシージャに自分へのポインタを渡す。
	SetWindowLongPtr(this->hWnd,GWL_USERDATA,(LONG)this);
	InvalidateRect(this->hWnd, NULL, FALSE);
	UpdateWindow(this->hWnd);
}
//削除処理
void Window::Delete(){
	for(int i=0 ; i < (signed)this->obj.size() ; i++){
		this->obj.at(i)->Delete();
	}
	DeleteDC(this->hdc);
	DestroyWindow(this->hWnd);
	this->hdc = NULL;
	this->hWnd = NULL;
}
//===============================================================================================
//ウィンドウの登録
ATOM Window::InitApp(HINSTANCE hInst)
{
	//ウィンドウの設定はここで行う。
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;		//ウィンドウのスタイル       
	wc.lpfnWndProc = &Window::WndProc;		//ウィンドウプロシージャ
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = (HICON)LoadImage(NULL,		//アイコン
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = (HCURSOR)LoadImage(NULL,	//カーソル
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	wc.lpszMenuName = NULL;					//メニューバー
	wc.lpszClassName = (LPCSTR)this->id.c_str();	//ウィンドウID
	wc.hIconSm = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	return (RegisterClassEx(&wc));
}
//ウィンドウの作成
BOOL Window::InitInstance(HINSTANCE hInst, int nCmdShow)
{
	RECT rc;
	
	SetRect(&rc, 0, 0, this->getWidth(), this->getHeight());
	AdjustWindowRect( &rc, this->style , FALSE );
	
	this->hWnd = CreateWindowEx(
		0,
		this->id.c_str(),//ID
		this->title.c_str(),//タイトル
		this->style,//スタイル //WS_POPUP:枠なし
		CW_USEDEFAULT,			//初期X座標
		CW_USEDEFAULT,			//初期Y座標
		rc.right - rc.left,		//幅
		rc.bottom - rc.top,		//高さ
		NULL,
		NULL,
		hInst,
		NULL);

	if(!this->hWnd)
		return FALSE;

	ShowWindow(this->hWnd, nCmdShow);

	return TRUE;
}


//ウィンドウプロシージャ
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;
	std::vector<Window*>::iterator itr;

	Window *window = (Window*)GetWindowLongPtr(hWnd,GWL_USERDATA);

	switch (msg) {
		case WM_CREATE:
			break;
		case WM_PAINT: //描画
			hdc = BeginPaint(hWnd,&ps);

			if(window == NULL) break;
			BitBlt(hdc, 0,0,window->getWidth(),window->getHeight(), window->hdc, 0,0,SRCCOPY);

			EndPaint(hWnd,&ps);
			break;
		case WM_SIZE:	//サイズが変更された
			if(window == NULL) break;
			window->setWidth(lp & 0xffff);			//width
			window->setHeight((lp >> 16) & 0xffff);	//height
			break;
		case WM_CLOSE: //終了
			delete window;
			break;
		case WM_COMMAND:
			break;
		case WM_DESTROY:
			break;
		default:
			return (DefWindowProc(hWnd, msg, wp, lp));
	}
	return 0;
}