
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
	Mat image2D; //用于打开图像文件，之后存成bool类型
	bool bDilateAndErode = 0; //是否膨胀和腐蚀
	string recImagePath;//重建图像的路径
	string recImageHeadName;
	int fileNum = 1;
	int fileStart = 0;

	int recYlength;
	int recXlength;
	int recZlength;

	string recImageExt;//重建图像的扩展名
	
	vector<Mat> recImage3D;//重建图像-3D
	vector<vector<bool>> orgRefImage;//读入的参考图像-2D-bool
	vector<vector<bool>> downSamplingorgRefImage;//参考图像-2D-bool对应字典的尺度
	vector<vector<vector<bool>>>towerRefImage;
	vector<pair<double, double>>towerPoreSize;

	vector<vector<vector<bool>>> recImage3DBool;//重建图像-3D-bool


public:
	//功能函数
	void getRecImg(string path, string name, string ext);
	void getImgFromRec(vector<vector<vector<bool>>>& Image3D);	
	void loadRefImage(string tmpImagePath);//读入参考图像
	void writeRecImage();//写出重建图像
	void calPoreSize(double& aveL_len, double& xigmaL_len, vector<vector<bool>>& Img);
	void calStructSize(double& mix_size_x, double& mix_size_y, vector<vector<bool>>& Img);
	void downSamplingOrg(int& downSamplingCount, vector<vector<bool>>& Img);
	void calBestMatchSize(double &aveLen, double &xigmaLen,int& Countnum);
	void writeImage2D(vector<vector<bool>>& Img2D);
};
