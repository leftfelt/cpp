#include "gui/WindowClass.h"

//==============================================================================
//�A�v���P�[�V�����̃C���X�^���X�n���h��
HINSTANCE Window::hInst = 0;

//�R���X�g���N�^
Window::Window(std::string id){
	this->id = id;
	this->style = WS_OVERLAPPEDWINDOW;
	this->display.Create(640,480);
	this->setSize(640,480);

	this->Initialize();
}

Window::~Window(){
	this->Delete();
}

//�傫����ݒ�
void Window::setSize(int width, int height){
	RECT rc;

	this->width = width;
	this->height = height;

	this->display.Size(this->width, this->height);
	
	SetRect(&rc, 0, 0, width, height);
	AdjustWindowRect( &rc, this->style , FALSE );

	SetWindowPos(this->hWnd,HWND_TOP,0,0,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE);//�E�B���h�E�T�C�Y��ύX
}

//�ʒu��ݒ�
void Window::setPos(int x, int y){
	SetWindowPos(this->hWnd,NULL,x,y,0,0,SWP_NOSIZE); //�E�B���h�E�̈ʒu��ύX
}

//�X�^�C��
void Window::setStyle(int style){
	this->style = style;
	SetWindowLong(this->hWnd,GWL_STYLE, this->style);
	SetWindowPos(this->hWnd,HWND_BOTTOM,0,0,0,0,( SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER));
}

//ID��ݒ�
void Window::setID(std::string id){
	this->id = id;
}
//�E�B���h�E�̎���ID���擾
std::string Window::getID(){
	return this->id;
}

//�^�C�g������ݒ�
void Window::setTitle(std::string title){
	this->title = title;
	SetWindowText(this->hWnd,title.c_str());//�^�C�g����ύX
}
//�E�B���h�E�̃^�C�g�����擾
std::string Window::getTitle(){
	return this->title;
}

//�����擾
int  Window::getWidth(){
	return this->width;
}
//����ݒ�
void  Window::setWidth(int width){
	this->setSize(width, this->height);
}

//�����擾
int  Window::getHeight(){
	return this->height;
}
//�����ݒ�
void  Window::setHeight(int height){
	this->setSize(this->width, height);
}

//�I�u�W�F�N�g��ǉ�
void Window::AddObject(WindowObject *obj){
	this->obj.push_back(obj);
	obj->Initialize(this->hWnd, this->hdc);
}

//�I�u�W�F�N�g���폜
void Window::DeleteObject(WindowObject *obj){
	if(this->obj.empty()){
		std::vector<WindowObject*>::iterator itr;
		for( itr=this->obj.begin() ; itr != this->obj.end() ; itr++ ){
			if(*itr == obj)break;
		}
		this->obj.erase(itr);
	}
}

Image* Window::getDisplay(){
	return &this->display;
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
//��������
bool Window::Initialize(){
	HDC hdc;
	HBITMAP hBmp_mem;

	if(!InitApp(this->hInst))
		return false;
	if(!InitInstance(this->hInst, SW_SHOWNORMAL))
		return false;
	hdc = GetDC(this->hWnd);

	//�`��̈���쐬
	this->hdc = CreateCompatibleDC(hdc);
	hBmp_mem = CreateCompatibleBitmap(hdc,this->getWidth(),this->getHeight());
	SelectObject(this->hdc, hBmp_mem);
	//���œh��Ԃ�
	PatBlt(this->hdc,0,0,this->getWidth(),this->getHeight(),WHITENESS);
	ReleaseDC(this->hWnd, hdc);

	//�E�B���h�E�v���V�[�W���Ɏ����ւ̃|�C���^��n���B
	SetWindowLongPtr(this->hWnd,GWL_USERDATA,(LONG)this);
	
	UpdateWindow(this->hWnd);

	this->AddObject(&this->display);

	return true;
}
//�X�V����
void Window::Update(){
	for(int i=0 ; i < (signed)this->obj.size() ; i++){
		this->obj.at(i)->Update(this->hWnd, this->hdc);
	}
}
//�`�揈��
void Window::Draw(){
	
	for(int i=0 ; i < (signed)this->obj.size() ; i++){
		this->obj.at(i)->Draw(this->hWnd, this->hdc);
	}
	//�E�B���h�E�v���V�[�W���Ɏ����ւ̃|�C���^��n���B
	SetWindowLongPtr(this->hWnd,GWL_USERDATA,(LONG)this);
	InvalidateRect(this->hWnd, NULL, FALSE);
	UpdateWindow(this->hWnd);
}
//�폜����
void Window::Delete(){
	for(int i=0 ; i < (signed)this->obj.size() ; i++){
		this->obj.at(i)->Delete(this->hWnd, this->hdc);
	}
	DeleteDC(this->hdc);
	DestroyWindow(this->hWnd);
	this->hdc = NULL;
	this->hWnd = NULL;
}
//===============================================================================================
//�E�B���h�E�̓o�^
ATOM Window::InitApp(HINSTANCE hInst)
{
	//�E�B���h�E�̐ݒ�͂����ōs���B
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;		//�E�B���h�E�̃X�^�C��       
	wc.lpfnWndProc = &Window::WndProc;		//�E�B���h�E�v���V�[�W��
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = (HICON)LoadImage(NULL,		//�A�C�R��
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = (HCURSOR)LoadImage(NULL,	//�J�[�\��
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	wc.lpszMenuName = NULL;					//���j���[�o�[
	wc.lpszClassName = (LPCSTR)this->id.c_str();	//�E�B���h�EID
	wc.hIconSm = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	return (RegisterClassEx(&wc));
}
//�E�B���h�E�̍쐬
BOOL Window::InitInstance(HINSTANCE hInst, int nCmdShow)
{
	RECT rc;
	
	SetRect(&rc, 0, 0, this->getWidth(), this->getHeight());
	AdjustWindowRect( &rc, this->style , FALSE );
	
	this->hWnd = CreateWindowEx(
		0,
		this->id.c_str(),//ID
		this->title.c_str(),//�^�C�g��
		this->style,//�X�^�C�� //WS_POPUP:�g�Ȃ�
		CW_USEDEFAULT,			//����X���W
		CW_USEDEFAULT,			//����Y���W
		rc.right - rc.left,		//��
		rc.bottom - rc.top,		//����
		NULL,
		NULL,
		hInst,
		NULL);

	if(!this->hWnd)
		return FALSE;

	ShowWindow(this->hWnd, nCmdShow);

	return TRUE;
}


//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;
	std::vector<Window*>::iterator itr;

	Window *window = (Window*)GetWindowLongPtr(hWnd,GWL_USERDATA);

	switch (msg) {
		case WM_CREATE:
			break;
		case WM_PAINT: //�`��
			hdc = BeginPaint(hWnd,&ps);

			if(window == NULL) break;
			BitBlt(hdc, 0,0,window->getWidth(),window->getHeight(), window->hdc, 0,0,SRCCOPY);

			EndPaint(hWnd,&ps);
			break;
		case WM_SIZE:	//�T�C�Y���ύX���ꂽ
			if(window == NULL) break;
			window->setWidth(lp & 0xffff);			//width
			window->setHeight((lp >> 16) & 0xffff);	//height
			break;
		case WM_CLOSE: //�I��
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