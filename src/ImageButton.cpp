#include "ImageButton.h"
ImageButton::ImageButton(){
}

//���W�ݒ�
void ImageButton::setPos(int x,int y){
	area.Pos(x,y);
}
//�摜�ݒ�
void ImageButton::setImage(Image &img){
	image = img;
	image.setPos(area.getX(),area.getY());
	area.Size(image.Width(),image.Height());
}
//�N���b�N����
bool ImageButton::isClick(M_BUTTON button){
	if( isOnMouse() &&  mouse->Click(button)) return true;
	else return false;
}
//�}�E�X�I�[�o�[����
bool ImageButton::isOnMouse(){
	if( area.HitCheck(mouse->Pos().x,mouse->Pos().y) ) return true;
	else return false;
}
//=====================================================================
void ImageButton::Initialize(){
	mouse = new Mouse(*this->wnd);
	image.SetWindow(*this->wnd);
	image.Initialize();
}
void ImageButton::Update(){
	image.Update();
}
void ImageButton::Draw(){
	image.Draw();
}
void ImageButton::Delete(){
	delete mouse;
	image.Delete();
}