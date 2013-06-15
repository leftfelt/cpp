#ifndef MOUSECLASS_INCLUDE
#define MOUSECLASS_INCLUDE

#include <windows.h>
#include <gui/WindowClass.h>

typedef enum{
	M_LEFT,
	M_RIGHT,
}M_BUTTON;

//マウスの情報
class Mouse{
private:
	POINT m_pt;
	Window *m_wnd;
public:
	Mouse(Window &wnd);
	POINT Pos();//ウィンドウ上のマウスの位置を取得
	void Pos(int x,int y);//マウスの位置を設定
	bool Click(M_BUTTON button);//マウスのボタンの状態を取得
	bool onLeftClick();
	bool onRightClick();
};

#endif