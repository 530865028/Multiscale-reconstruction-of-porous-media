
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;
class FilesProcess
{
public:
	FilesProcess(void);
	~FilesProcess(void);
public:
	int templateSize = 3;
	Mat image2D; //���ڴ�ͼ���ļ���֮����bool����
	bool bDilateAndErode = 0; //�Ƿ����ͺ͸�ʴ
	string recImagePath;//�ؽ�ͼ���·��
	string recImageHeadName;
	int fileNum = 1;
	int fileStart = 0;

	int recYlength;
	int recXlength;
	int recZlength;

	string recImageExt;//�ؽ�ͼ�����չ��
	
	vector<Mat> recImage3D;//�ؽ�ͼ��-3D
	vector<vector<bool>> orgRefImage;//����Ĳο�ͼ��-2D-bool
	vector<vector<bool>> downSamplingorgRefImage;//�ο�ͼ��-2D-bool��Ӧ�ֵ�ĳ߶�
	vector<vector<vector<bool>>>towerRefImage;
	vector<pair<double, double>>towerPoreSize;

	vector<vector<vector<bool>>> recImage3DBool;//�ؽ�ͼ��-3D-bool


public:
	//���ܺ���
	void getRecImg(string path, string name, string ext);
	void getImgFromRec(vector<vector<vector<bool>>>& Image3D);	
	void loadRefImage(string tmpImagePath);//����ο�ͼ��
	void writeRecImage();//д���ؽ�ͼ��
	void calPoreSize(double& aveL_len, double& xigmaL_len, vector<vector<bool>>& Img);
	void calStructSize(double& mix_size_x, double& mix_size_y, vector<vector<bool>>& Img);
	void downSamplingOrg(int& downSamplingCount, vector<vector<bool>>& Img);
	void calBestMatchSize(double &aveLen, double &xigmaLen,int& Countnum);
	void writeImage2D(vector<vector<bool>>& Img2D);
};
