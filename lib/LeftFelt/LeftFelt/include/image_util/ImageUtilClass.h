#ifndef IMAGEUTILCLASS_INCLUDE
#define IMAGEUTILCLASS_INCLUDE

//OpenCV�̈ˑ����Ȃ���
#include <map>
#include <image_util/biImageClass.h>
#include <base/MathClass.h>
#include <base/AreaClass.h>
#include <base/PointClass.h>
//*

class ImageUtil{
private:
	float static laplacian_filter[9];
	int static min_calc(int *p);
	int static Lookup_update(int i,int * label);
	void static filtering(biImage &image, float *filter, int filter_size[2], int direction[2]);
	float static convolution(biImage &image, float *filter, int x, int y, int filter_size[2], int direction[2]);
	float static derivativesGaussian(int x, int y, float sigma); //1������K�E�V�A��
	float static Gaussian(int x, int y, float sigma);
public:
	void static Shrink(biImage &image);//���̈�����k
	void static Expand(biImage &image);//���̈��c��
	void static Opening(biImage &image, int level);//�I�[�v�j���O�i���̃m�C�Y����������j
	void static Closing(biImage &image, int level);//�N���[�W���O�i���̃m�C�Y����������j
	void static TopHat(biImage &image,int level);//�g�b�v�n�b�g
	void static BlackHat(biImage &image,int level);//�u���b�N�n�b�g
	int static Thinning(biImage &image);//�א���
	void static UnSharpMasking(biImage &image,int level);//�N�s��
	void static LowResolution(biImage &image,int level);//��𑜓x��
	void static colorRegionSplit(biImage &image, int threshold = 100);//�F�̈敪��
	void static getHistogram(biImage &image, int *histogram);
	int static getThreshold(biImage &image);//���ʕ��͖@
	void static toGrayScale(biImage &image);//�O���C�X�P�[���ɕϊ�
	int static Binarize(biImage &image, int threshold = -1);//��l��
	void static Incline(biImage &image, double R,double G,double B);//�X����������
	void static Sobel(biImage &image);//�]�[�x���t�B���^
	void static Laplacian(biImage &image);//���v���V�A���t�B���^
	void static Gaussian(biImage &image, float sigma);//�K�E�V�A���t�B���^
	PointList static Harris(biImage &image, float sigma = 1, float threshold = 0.1);
	void static AntiNoise(biImage &image, unsigned int level = 1);//�m�C�Y����
	void static Contrast(biImage &image);//�R���g���X�g���P
	void static YIQRange(biImage &image, int Ylow, int Yhigh,int Ilow = 0, int Ihigh = 0, int Qlow = 0, int Qhigh = 0);//�F���o
	void static HSVRange(biImage &image, int Hlow, int Hhigh,int Slow = 0, int Shigh = 100, int Vlow = 0, int Vhigh = 100);//�F���o
	void static HRange(biImage &image, int low, int high, bool in_range = true); //�F�����w��͈͂̉�f�𒊏o
	void static SRange(biImage &image, int low, int high, bool in_range = true); //�ʓx���w��͈͂̉�f�𒊏o
	void static VRange(biImage &image, int low, int high, bool in_range = true); //���x���w��͈͂̉�f�𒊏o
	void static GammaCorrection(biImage &image, double gamma);//�K���}�␳
	void static Brightness(biImage &image, int threshold=128);//���邳����
	Area static Scope(biImage &image);//�L���͈͂��擾����(255������͈�)
	Point static TemplateMatching(biImage &image, biImage temp, int threshold = -1);//�e���v���[�g�}�b�`���O
	Point static TemplateMatching(biImage &image, int x, int y, int width, int height, biImage temp, int threshold = -1);
	int static Matching(biImage &image, int x,int y,biImage temp);
	PointList static getNodeList(biImage &image,int connected_num);//�ڑ�����connected_num�̃m�[�h���擾����
	int static getConnectedNum(biImage &image,int x,int y);//����8�}�X�ɂ���Pixel(255)�̐����擾
	int static Labeling(biImage &image);//���x�����O �ԍ���1����J�n
	int static SamplingObject(biImage &image, int label);//���x���ԍ�����I�u�W�F�N�g�𒊏o����B���łɖʐς��Ԃ��B
	int static SamplingLargeObject(biImage &image);//���x�����O�ς݉摜����ʐς��ő�̃I�u�W�F�N�g�𒊏o����
	double static getBulr(biImage &image); //�ڂ₯�x���擾����
	void static costomBinalize(biImage &image, Pixel attention, Pixel sigma); // <-- TODO:���O�K�������炩����I
	/** ImageIO�Ɉړ������� bitmap�ł͂Ȃ��Ă����� ppm�`���Ƃ�
	bool static Save(biImage &image, std::string filename, short bitCount = 24, int dataoffset = 54);
	bool static Load(biImage &image, std::string filename);
	*/
};

#endif