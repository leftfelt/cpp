#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <gui/Application.h>
#include <gui/ImageClass.h>
#include <base/PointClass.h>
#include <base/MathClass.h>
#include <image_util/ImageUtilClass.h>
#include <queue>


class Unit{
protected:
	Point position;
	biImage image;
	int speed;
	int angle;
	bool is_enabled;
	int life;
	int size;
public:
	Unit(){
		this->position = position;
		this->speed = 1;
		this->angle = 0;
		this->life = 1;
		this->size = 1;
		this->is_enabled = false;
	}

	Point getPos(){ return this->position; }
	void setPos(Point position){
		this->position = Point(
			this->position.X() + position.X(),
			this->position.Y() + position.Y()
		);
	}

	void setSpeed(int speed){ this->speed = speed; }
	int getSpeed(){	return this->speed; }

	void setAngle(int angle){ this->angle = angle + 90; }
	int getAngle(){ return this->angle; }

	biImage getImage(){ 
		biImage temp = this->image;
		temp.Rotate(this->getAngle());
		return this->image;
	}
	void setImage(biImage image){ this->image = image; }

	int getLife(){ return this->life; }
	void setLife(int life){ this->life = life; }

	int getSize(){ return this->size; }
	void setSize(int size){ this->size = size; }

	void enable(){ this->is_enabled = true;	}
	void disable(){ this->is_enabled = false; }

	bool isEnabled(){ return this->is_enabled; }
	void move(){
		this->position = Point(
			Math::aCos((double)angle)*this->speed + this->position.X(),
			Math::aSin((double)angle)*this->speed + this->position.Y()
		);
	}
};

//弾
class Bullet : public Unit{
public:
	void update(){
	}
};

//エネミー
class Enemy : public Unit{
private:
	std::vector<Bullet> bullet_list;
public:
	void update(){
		this->move();
		std::for_each(this->bullet_list.begin(), this->bullet_list.end(),[&](Bullet bullet){
			if(!bullet.isEnabled()) return;
			bullet.move();
		});
	}
	std::vector<Bullet> getBulletList(){
	}
};

//プレイヤー
class Player : public Unit{
private:
	std::vector<Bullet> bullet_list;
public:
	void update(){
		this->move();
		std::for_each(this->bullet_list.begin(), this->bullet_list.end(),[&](Bullet bullet){
			if(!bullet.isEnabled()) return;
			bullet.move();
		});
	}
	std::vector<Bullet> getBulletList(){
	}
};

class Stage{
protected:
	unsigned int frame;
	
	bool isUnitCollision(Unit unit1, Unit unit2){
		//２点間の距離がunit1のサイズ＋unit2のサイズ以下ならHit
		double distance = Math::distance(unit1.getPos(), unit2.getPos());
		return (distance < unit1.getSize() + unit2.getSize());
	}
public:
	Stage(){
		this->frame = 0;
	}
	virtual void update(biImage &display) = 0;
	virtual bool isEnd() = 0;
};

class Stage1 : public Stage{
private:
	Player player;
	biImage image;
public:
	Stage1():Stage(){
		ImageUtil::Load(image, "../../../data/lenna.bmp");
		image.Size(100,100);
		this->player.setSpeed(1);
		this->player.setAngle(-90);
		this->player.setSize(3);
		this->player.setPos(Point(100,100));
		this->player.setImage(image.Cut(45,45,10,10));
	}
	void update(biImage &display){
		display = 0;
		player.update();
		this->frame++;

		Point pos;
		pos = this->player.getPos();
		display.Paste(pos.X(),pos.Y(),this->player.getImage());
	}
	bool isEnd(){
		return false;
	}
};

class Shooting : public Application{
private:
	Window *window;
	Image *display;
	std::queue<Stage *> stage_queue;
	Stage *stage;
public:
	Shooting(){
		window = this->createWindow("main");
		window->setSize(800, 600);
		display = new Image(800, 600);
		window->AddObject(display);

		stage_queue.push(new Stage1);

		stage = stage_queue.front();
		stage_queue.pop();
	}
	void main(){
		//ステージが終了したか
		stage->update(*this->display);
		if(stage->isEnd()){
			if(this->stage_queue.empty()) this->exit();
			//次のステージへ
			stage = stage_queue.front();
			stage_queue.pop();
		}
	}

};

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow){
	Shooting shooting;
	shooting.winMain(hCurInst);
	return 0;
}


//ユニットを移動できる
//ユニットが弾を発射できる

//ステージにユニットを配置できる
//ユニット同士の当たり判定

//lifeが0以下になったらdisable