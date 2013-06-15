#ifndef MOUSECLASS_INCLUDE
#define MOUSECLASS_INCLUDE

#include <windows.h>
#include <gui/WindowClass.h>

typedef enum{
	M_LEFT,
	M_RIGHT,
}M_BUTTON;

//�}�E�X�̏��
class Mouse{
private:
	POINT m_pt;
	Window *m_wnd;
public:
	Mouse(Window &wnd);
	POINT Pos();//�E�B���h�E��̃}�E�X�̈ʒu���擾
	void Pos(int x,int y);//�}�E�X�̈ʒu��ݒ�
	bool Click(M_BUTTON button);//�}�E�X�̃{�^���̏�Ԃ��擾
	bool onLeftClick();
	bool onRightClick();
};

#endif