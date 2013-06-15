#include "gui/MouseClass.h"

Mouse::Mouse(Window &wnd){
	m_wnd = &wnd;
}

//画面上の座標を得る
POINT Mouse::Pos(){
	HWND hWnd;
	GetCursorPos(&m_pt);//マウスカーソルの位置を取得（スクリーン全体での)

	hWnd = FindWindow(m_wnd->getID().c_str(),m_wnd->getTitle().c_str());
	
	ScreenToClient(hWnd,&m_pt);	//クライアント領域座標へ変換
	return m_pt;
}

//マウスカーソルをクライアント領域の座標へ移動
void Mouse::Pos(int x, int y){
	HWND hWnd;
	m_pt.x = x;
	m_pt.y = y;

	hWnd = FindWindow(m_wnd->getID().c_str(),m_wnd->getTitle().c_str());

	ClientToScreen(hWnd,&m_pt);//スクリーン領域座標へ変換
	SetCursorPos(m_pt.x,m_pt.y);//カーソルを移動
}

//マウスのボタンの状態を取得
bool Mouse::Click(M_BUTTON button){
	if(button == M_LEFT 
		&& Pos().x >= 0 && Pos().x < m_wnd->getWidth() 
		&& Pos().x >= 0 && Pos().y < m_wnd->getHeight()) return ((GetKeyState(VK_LBUTTON) & 0x80) != 0);
	if(button == M_RIGHT
		&& Pos().x >= 0 && Pos().x < m_wnd->getWidth() 
		&& Pos().x >= 0 && Pos().y < m_wnd->getHeight()) return ((GetKeyState(VK_RBUTTON) & 0x80) != 0);
	return false;
}

bool Mouse::onLeftClick(){
	return ((GetKeyState(VK_LBUTTON) & 0x80) != 0);
}
bool onRightClick(){
	return ((GetKeyState(VK_RBUTTON) & 0x80) != 0);
}