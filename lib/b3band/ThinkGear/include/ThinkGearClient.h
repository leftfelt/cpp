#ifndef _THINKGEARCLIENT_INCLUDE
#define _THINKGEARCLIENT_INCLUDE

#pragma comment(lib,"thinkgear.lib")
#include <thinkgear.h>
#include <algorithm>
#include <string>
#include <BrainWaves.h>

class ThinkGearClient {
private:
	int connection_id;
	std::map<std::string,int> waves_map;
	bool is_start_capture;//キャプチャ中フラグ
	int readPackets(int packet_num);//指定したパケット数だけ読み取る
	int enableAutoRead(bool enabled);//パケットの自動読み取りを有効にする
	double getValue(int data_type);//パケットを１つ読み込み、指定された定義済データ型の値で返す
	bool create();//新しくIDを割り当てる。
	double getCaptureData(int data_type);//指定された定義済データ型の値を返す
public:
	ThinkGearClient();
	~ThinkGearClient();
	bool isStartCapture();//キャプチャ中であるか
	int connect(std::string port_name, int bit_rate=TG_BAUD_9600, int data_type=TG_STREAM_PACKETS);//指定したシリアルポートに接続を行い、IDと関連付ける
	void disconnect();//IDに関連付けられているシリアルポートとの接続を切断する。
	int streamLog(std::string filename);//読み取ったバイトををログファイルへ自動的に記録する
	int dataLog(std::string filename);//解析データをログファイルへ自動的に記録する
	void startCapture();//パケットの読み取りを開始する
	void stopCapture();//パケットの読み取りを停止する 
	BrainWaves getWaves();
};


#endif