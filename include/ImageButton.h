#ifndef INCLUDE_IMAGEBUTTONCLASS
#define INCLUDE_IMAGEBUTTONCLASS

#include <WindowClass.h>
#include <MouseClass.h>
#include <KeyClass.h>
#include <ImageClass.h>
#include <Area.h>

class ImageButton : public WindowObject{
private:
	Area area;//���W�E�傫��
	Image image;//�摜
	Mouse *mouse;
public:
	void setPos(int x,int y);//���W�ݒ�
	void setImage(Image &img);//�摜�ݒ�
	bool isClick(M_BUTTON button);//�N���b�N����
	bool isOnMouse();//�}�E�X�I�[�o�[����

	ImageButton();
	void Initialize();
	void Update();
	void Draw();
	void Delete();
};

#endif