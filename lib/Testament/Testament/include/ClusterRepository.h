#ifndef INCLUDE_CLUSTERREPOSITORY
#define INCLUDE_CLUSTERREPOSITORY

#ifdef _DEBUG
	#pragma comment(lib,"sqlite3d.lib")
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"sqlite3.lib")
	#pragma comment(lib,"base.lib")
#endif

#include <ClassifierClass.h>
#include <Converter.h>
#include <SQLite.h>
#include <base/MathClass.h>
#include <base/NumberClass.h>
#include <sstream>

//�N���X�^
class Cluster{
private:
	std::string label; //���ޖ�
	double feature; //������
public:
	Cluster(std::string label, double feature);
	Cluster(Row row);
	std::string getLabel();
	double getFeature();
};

//�N���X�^���|�W�g��
class ClusterRepository{
private:
	std::string data_type; //�f�[�^�̎��
	SQLite sqlite;
public:
	ClusterRepository(std::string db_name, std::string data_type);
	~ClusterRepository();
	std::string getTableName(); //�e�[�u�������擾
	std::string getSelectClusterQuery(double feature, double threshold, int limit); //feature�̍���threshold�ȉ��̃N���X�^���擾����N�G�����擾
	std::string getRaisePriorityQuery(std::vector<Row> clusters); //�w�肵���N���X�^��priority���グ��N�G�����擾
	std::string getAddClusterQuery(double feature, int label = -1); //�N���X�^��ǉ�����N�G�����擾
	//�e�[�u�������݂��Ȃ��ꍇ�̓e�[�u�������
	void setup(); 
	//�����ʂ̍���threshold�ȉ��̃N���X�^���擾
	std::vector<Cluster> search(Pattern pattern,double threshold, int limit = 1);
	//�����ʂ̍���threshold�ȉ��̃N���X�^�͗D��x���グ��Athreshold�ȏ�Ȃ�N���X�^��ǉ�����
	void commit(std::vector<Pattern> pattern_list,  double threshold, int num = 3);
	//�f�[�^����ɂ���
	void truncate();
	//feature�̍ł������������擾����
	double getSmallestDiffByFeature();
	//feature�̍ł��傫�������擾����
	double getLargestDiffByFeature();
	//�p�^�[���̓����ʂ��擾
	double getFeature(Pattern pattern);
};

#endif