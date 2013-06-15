#include "gui/MouseClass.h"

Mouse::Mouse(Window &wnd){
	m_wnd = &wnd;
}

//��ʏ�̍��W�𓾂�
POINT Mouse::Pos(){
	HWND hWnd;
	GetCursorPos(&m_pt);//�}�E�X�J�[�\���̈ʒu���擾�i�X�N���[���S�̂ł�)

	hWnd = FindWindow(m_wnd->getID().c_str(),m_wnd->getTitle().c_str());
	
	ScreenToClient(hWnd,&m_pt);	//�N���C�A���g�̈���W�֕ϊ�
	return m_pt;
}

//�}�E�X�J�[�\�����N���C�A���g�̈�̍��W�ֈړ�
void Mouse::Pos(int x, int y){
	HWND hWnd;
	m_pt.x = x;
	m_pt.y = y;

	hWnd = FindWindow(m_wnd->getID().c_str(),m_wnd->getTitle().c_str());

	ClientToScreen(hWnd,&m_pt);//�X�N���[���̈���W�֕ϊ�
	SetCursorPos(m_pt.x,m_pt.y);//�J�[�\�����ړ�
}

//�}�E�X�̃{�^���̏�Ԃ��擾
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