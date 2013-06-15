#include "gui/SquareClass.h"

void Square::Apex(POINT pt0,POINT pt1,POINT pt2,POINT pt3){//4つの頂点を設定
	line[0].Start(pt0.x,pt0.y);
	line[0].End(pt1.x,pt1.y);
	line[1].Start(pt1.x,pt1.y);
	line[1].End(pt2.x,pt2.y);
	line[2].Start(pt2.x,pt2.y);
	line[2].End(pt3.x,pt3.y);
	line[3].Start(pt3.x,pt3.y);
	line[3].End(pt0.x,pt0.y);
}

void Square::Apex(int x, int y, int width, int height){
	line[0].Start(x,y);
	line[0].End(x+width,y);
	line[1].Start(x+width,y);
	line[1].End(x+width,y+height);
	line[2].Start(x+width,y+height);
	line[2].End(x,y+height);
	line[3].Start(x,y+height);
	line[3].End(x,y);
}

void Square::Apex(char num, int x, int y){//指定した頂点を設定
	line[num].Start(x,y);
	if(num == 0) num = 3;
	else num--;
	line[num].End(x,y);
}

POINT Square::getApex(char num){//0～3で指定した頂点座標を取得
	return line[num].getStart();
}

void Square::Move(int x, int y){
	int cx,cy;
	cx = x - getMiddle().x;
	cy = y - getMiddle().y;
	for(int i = 0 ; i < 4 ; i++){
		line[i].Move(line[i].getMiddle().x+cx,line[i].getMiddle().y+cy);
	}
}

POINT Square::getMiddle(){//中点の座標を取得
	POINT pt ={0,0};
	for(int i = 0 ; i < 4 ; i++){
		pt.x += line[i].getStart().x + line[i].getEnd().x;
		pt.y += line[i].getStart().y + line[i].getEnd().y;
	}
	pt.x /= 8;
	pt.y /= 8;
	return pt;
}

int Square::getArea(){//面積を取得
	int ab,cd;
	Line dis;
	dis.Start(line[0].getMiddle().x,line[0].getMiddle().y);
	dis.End(line[2].getMiddle().x,line[2].getMiddle().y);
	ab = dis.getDistance();
	dis.Start(line[1].getMiddle().x,line[1].getMiddle().y);
	dis.End(line[3].getMiddle().x,line[3].getMiddle().y);
	cd = dis.getDistance();
	return ab*cd;
}

int Square::getArea(biImage &image){//面積を取得
	int ab,cd;
	Line dis;
	dis.Start(line[0].getMiddle().x,line[0].getMiddle().y);
	dis.End(line[2].getMiddle().x,line[2].getMiddle().y);
	ab = dis.getDistance();
	dis.Draw(image);
	dis.Start(line[1].getMiddle().x,line[1].getMiddle().y);
	dis.End(line[3].getMiddle().x,line[3].getMiddle().y);
	cd = dis.getDistance();
	dis.Draw(image);
	return ab*cd;
}

int Square::getPerimeter(){//周囲長を取得
	return line[0].getDistance() + line[1].getDistance() + line[2].getDistance() + line[3].getDistance();
}

void Square::Color(Pixel color){//色を指定
	int i;
	for(i = 0 ; i < 4 ; i++){
		line[i].Color(color);
	}
}
void Square::Draw(biImage &image){//描画
	int i;
	for(i = 0 ; i < 4 ; i++){
		line[i].Draw(image);
	}
}