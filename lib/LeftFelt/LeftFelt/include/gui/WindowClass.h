#ifndef INCLUDE_WINDOWCLASS
#define INCLUDE_WINDOWCLASS

#include <gui/WindowClass.h>
#include <string>
#include <vector>
#include <gui/ObjectClass.h>

//Window�N���X
class Window;

//WindowObject�N���X
class WindowObject : public Object{
protected:
	Window *wnd;
public:
	WindowObject();
	bool SetWindow(Window &wnd);
	std::string ParentWindow();
};

//Window�N���X
class Window {
private:
	std::string id;	//�E�B���h�EID
	std::string title;	//�^�C�g����

	int style;	//�X�^�C��

	int width; //��
	int height;//����

	ATOM InitApp(HINSTANCE);
	BOOL InitInstance(HINSTANCE, int);
	std::vector<WindowObject*> obj;	//�o�^����Ă���I�u�W�F�N�g
	static std::vector<Window *> window_list; 
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	static HINSTANCE hInst; //�A�v���P�[�V�����̃C���X�^���X
	HWND hWnd; //�E�B���h�E�n���h��
	HDC hdc; //�R���e�L�X�g�n���h��

	Window(std::string id="main");	//�R���X�g���N�^
	~Window();

	void setStyle(int style);		//�X�^�C��
		
	void setSize(int width, int height);	//�E�B���h�E�̃T�C�Y��ݒ�
	void setPos(int x, int y); //�ʒu��ݒ�

	void setTitle(std::string title);	//�E�B���h�E�̃^�C�g����ݒ�
	std::string getTitle();		//�E�B���h�E�̃^�C�g�����擾
	void setID(std::string id);		//�E�B���h�E�̎���ID��ݒ�
	std::string getID();	//�E�B���h�E�̎���ID���擾
	int getWidth();	//�����擾
	void setWidth(int width);	//����ݒ�
	int getHeight();	//�������擾
	void setHeight(int height);	//������ݒ�
	
	void AddObject(WindowObject *obj);		//�I�u�W�F�N�g��o�^����
	void DeleteObject(WindowObject *obj);		//�I�u�W�F�N�g���폜����

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