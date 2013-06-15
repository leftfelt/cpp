#ifndef INCLUDE_IMAGEBUTTONCLASS
#define INCLUDE_IMAGEBUTTONCLASS

#include <WindowClass.h>
#include <MouseClass.h>
#include <KeyClass.h>
#include <ImageClass.h>
#include <Area.h>

class ImageButton : public WindowObject{
private:
	Area area;//座標・大きさ
	Image image;//画像
	Mouse *mouse;
public:
	void setPos(int x,int y);//座標設定
	void setImage(Image &img);//画像設定
	bool isClick(M_BUTTON button);//クリック判定
	bool isOnMouse();//マウスオーバー判定

	ImageButton();
	void Initialize();
	void Update();
	void Draw();
	void Delete();
};

#endif