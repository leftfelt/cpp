#ifndef INCLUDE_OBJECTCLASS
#define INCLUDE_OBJECTCLASS

#include <windows.h>

//�I�u�W�F�N�g�N���X
class Object{
private:
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Delete() = 0;
};

//WindowObject�N���X
class WindowObject{
public:
	virtual void Initialize(HWND hWnd, HDC hdc) = 0;
	virtual void Update(HWND hWnd, HDC hdc) = 0;
	virtual void Draw(HWND hWnd, HDC hdc) = 0;
	virtual void Delete(HWND hWnd, HDC hdc) = 0;
};

#endif