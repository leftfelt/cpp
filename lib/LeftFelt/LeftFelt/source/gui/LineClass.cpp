#include "gui/LineClass.h"

Line::Line(){	//コンストラクタ
	pt[0].x = 0;
	pt[0].y = 0;
	pt[1].x = 0;
	pt[1].y = 0;
	mcolor = 0;
}

void Line::Color(Pixel color){ //色を指定
	mcolor = color;
}

void Line::Start(int x , int y){ //始点を設定
	pt[0].x = x;
	pt[0].y = y;
}
void Line::End(int x, int y){	 //終点を設定
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
POINT Line::getStart(){	//始点を取得
	return pt[0];
}
POINT Line::getEnd(){		//終点を取得
	return pt[1];
}

int Line::getDistance(){//二点間の距離を取得
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
			//点を描画
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
			//点を描画
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

//交差判定
bool Line::isClossing(Line line){
	return false;
}