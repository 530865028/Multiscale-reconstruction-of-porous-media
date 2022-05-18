#pragma once

#include <opencv2\opencv.hpp>
#include <vector>
#include <bitset>
#include <stdlib.h>    

using namespace cv;
using namespace std;

class DetailDic
{
public:
	DetailDic();
	~DetailDic();

public:
	int templateSize = 5;
	//1,3,5层：3*9
	//底层xy：5*5，前侧yz：4*5，左侧xz：4*4
	//一共：25+20+16=61
	//125=61+64
	vector<pair<bitset<27>, vector<bitset<125>>>> dicDetail;
	vector<vector<int>>DlPos;
	vector<int>DlArrayLeft;
	vector<int>DlArrayRight;

	//细节信息字典
	void getDictionary(vector<pair<bitset<27>, vector<bitset<125>>>> &tmpdicDetail, vector<vector<int>>&tmpDlPos, vector<int>&tmpDlArrayLeft, vector<int>&tmpDlArrayRight);
	
	//填充
	void Padding(vector<vector<vector<bool>>>& tmpLRImage3DBool, vector<vector<bool>>& tmpHR2DBool);
	void Pad1(int tmpz, int tmpy, int tmpx);//只有底面
	void Pad2(int tmpz, int tmpy, int tmpx);//底面和前侧
	void Pad3(int tmpz, int tmpy, int tmpx);//底面和左侧
	void Pad4(int tmpz, int tmpy, int tmpx);//其他
	void Padding3D(vector<vector<vector<bool>>>& tmpLRImage3DBool);

	//重建的方式获取3D
	vector<vector<vector<bool>>> LRImage3DBool;
	void get3DImageBool(vector<vector<vector<bool>>>& tmp3DImage);

	//填充后结果
	vector<vector<vector<bool>>> HRImage3DBool;
	vector<vector<vector<bool>>> HRImage3DWithoutRefBool;

	//读取的方式获取3D
	string LRImagePath;
	string LRImageName;
	string LRImageExt;
	vector<Mat> LRImage3D;
	vector<vector<vector<bool>>> LRImg3DReadBool;
	bool getLRImagePath(string tmpImagePath, string tmpImageName, string tmpImageExt);
	bool imgLoad();
	int imgLoadStart = 0;

	string HRImagePath;
	string HRImageName;
	string HRImageExt;
	vector<vector<bool>>HR2D;
	bool getHRImagePath(string tmpImagePath, string tmpImageName, string tmpImageExt);
	bool imgLoadHR2D();


	vector<  pair< vector<Point3i>, pair<Point3i, Point3i> >  > SmallPoreDic;//小孔字典
	vector<vector<int>>SPDistribution;//小孔连通域分布
	bool DicSPoreCreate();//小孔信息字典
	bool AddSmallPore(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<uchar>>>& dstImage3D, int ObjGray,double AddRate);//统计连通域体积加入对应小孔
	vector<vector<vector<uchar>>>Notice3D;//判断填充区域
	bool MarkConnetedRegion_3D(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<uchar>>>& dstImage3D, int ObjGray);
	vector<vector<int>>LR3D_SPDist;


};