#include "gui/LineClass.h"

Line::Line(){	//�R���X�g���N�^
	pt[0].x = 0;
	pt[0].y = 0;
	pt[1].x = 0;
	pt[1].y = 0;
	mcolor = 0;
}

void Line::Color(Pixel color){ //�F���w��
	mcolor = color;
}

void Line::Start(int x , int y){ //�n�_��ݒ�
	pt[0].x = x;
	pt[0].y = y;
}
void Line::End(int x, int y){	 //�I�_��ݒ�
	pt[1].x = x;
	pt[1].y = y;
}

void Line::Move(int x, int y){
	int cx,cy;
	cx = x - getMiddle().x;
	cy = y - getMiddle().y;
	pt[0].x += cx;
	pt[0].y += cy;
	pt[1].x += cx;
	pt[1].y += cy;
}

POINT Line::getMiddle(){
	POINT p;
	p.x = (pt[0].x + pt[1].x)/2;
	p.y = (pt[0].y + pt[1].y)/2;
	return p;
}
POINT Line::getStart(){	//�n�_���擾
	return pt[0];
}
POINT Line::getEnd(){		//�I�_���擾
	return pt[1];
}

int Line::getDistance(){//��_�Ԃ̋������擾
	return (int)sqrt(pow(pt[0].x - pt[1].x,2.0) + pow(pt[0].y - pt[1].y,2.0));
}

void Line::Draw(biImage &image){
	int W = pt[1].x - pt[0].x;
	int H = pt[1].y - pt[0].y;
	int dx = 0;
	int dy = 0;
	int Wy = 0;
	int Hx = 0;

	if (abs(W) >= abs(H)){
        for (dx = 0; dx <= abs(W); dx ++){
			//�_��`��
			if(W >= 0){
	            if(H >= 0)image.Put(pt[0].x+dx, pt[0].y+dy,mcolor);
				else image.Put(pt[0].x+dx, pt[0].y-dy,mcolor);
			}else{
	            if(H >= 0)image.Put(pt[0].x-dx, pt[0].y+dy,mcolor);
				else image.Put(pt[0].x-dx, pt[0].y-dy,mcolor);
			}

            Hx += H;
			
			if (abs(Wy) < abs(Hx)){
			    Wy += W;
			    dy += 1;
			}
        }
    }else{
        for (dy = 0; dy <= abs(H); dy ++){
			//�_��`��
			if(H >= 0){
	            if(W >= 0)image.Put(pt[0].x+dx, pt[0].y+dy,mcolor);
				else image.Put(pt[0].x-dx, pt[0].y+dy,mcolor);
			}else{
	            if(W >= 0)image.Put(pt[0].x+dx, pt[0].y-dy,mcolor);
				else image.Put(pt[0].x-dx, pt[0].y-dy,mcolor);
			}
            Wy += W;
            if (abs(Hx) < abs(Wy)){
                Hx += H;
                dx += 1;
            }
        }
    }	
}

//��������
bool Line::isClossing(Line line){
	return false;
}