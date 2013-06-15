#ifndef _THINKGEARCLASS_INCLUDE
#define _THINKGEARCLASS_INCLUDE

#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"base.lib")
#endif

#pragma comment(lib,"thinkgear.lib")
#include <thinkgear.h>
#include <base/StringClass.h>
#include <map>
#include <algorithm>

//ThinkGearから取得できる値を格納しておくクラス
class BrainWaves {
private:
	std::map<String, double> waves;
	std::vector<String> parameters;
public:
	BrainWaves();
	BrainWaves(std::map<String, double> waves);

	std::vector<String> getParameters();
	double getValue(String name);

	std::map<String,double> toMap();

	double getBattery();//バッテリー？
	double getPoorSignal();//信号品質？
	double getAttention();//集中度(0-100)
	double getMeditation();//リラックス度(0-100)
	double getRaw();//生波形(12bit)
	double getDelta();//デルタ波(0.5-2.75Hz)
	double getTheta();//シータ波(3.5-6.75Hz)
	double getAlpha1();//低アルファ波(7.5-9.25Hz)
	double getAlpha2();//高アルファ波(10-11.75Hz)
	double getBeta1();//低ベータ波(13-16.75Hz)
	double getBeta2();//高ベータ波(18-29.75Hz)
	double getGamma1();//低ガンマ波(31-39.75Hz)
	double getGamma2();//中ガンマ波(41-49.75Hz)
	double getBlinkStrength();//まばたき？

	BrainWaves operator+(BrainWaves waves);
	BrainWaves operator-(BrainWaves waves);
	BrainWaves operator*(BrainWaves waves);
	BrainWaves operator/(BrainWaves waves);
	
	BrainWaves operator+=(BrainWaves waves);
	BrainWaves operator-=(BrainWaves waves);
	BrainWaves operator*=(BrainWaves waves);
	BrainWaves operator/=(BrainWaves waves);
};


class ThinkGear {
private:
	std::map<String,int> waves_map;
	int connection_id;
	bool is_start_capture;//キャプチャ中フラグ
	int ReadPackets(int packet_num);//指定したパケット数だけ読み取る
	int EnableAutoRead(bool enabled);//パケットの自動読み取りを有効にする
	double getValue(int data_type);//パケットを１つ読み込み、指定された定義済データ型の値で返す
	bool Create();//新しくIDを割り当てる。
	double getCaptureData(int data_type);//指定された定義済データ型の値を返す
public:
	ThinkGear();
	~ThinkGear();
	bool isStartCapture();//キャプチャ中であるか
	int Connect(String port_name, int bit_rate=TG_BAUD_9600, int data_type=TG_STREAM_PACKETS);//指定したシリアルポートに接続を行い、IDと関連付ける
	void DisConnect();//IDに関連付けられているシリアルポートとの接続を切断する。
	int StreamLog(String filename);//読み取ったバイトををログファイルへ自動的に記録する
	int DataLog(String filename);//解析データをログファイルへ自動的に記録する
	void StartCapture();//パケットの読み取りを開始する
	void StopCapture();//パケットの読み取りを停止する 
	BrainWaves getWaves();
};




#endif