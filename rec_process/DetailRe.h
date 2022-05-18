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
	//1,3,5�㣺3*9
	//�ײ�xy��5*5��ǰ��yz��4*5�����xz��4*4
	//һ����25+20+16=61
	//125=61+64
	vector<pair<bitset<27>, vector<bitset<125>>>> dicDetail;
	vector<vector<int>>DlPos;
	vector<int>DlArrayLeft;
	vector<int>DlArrayRight;

	//ϸ����Ϣ�ֵ�
	void getDictionary(vector<pair<bitset<27>, vector<bitset<125>>>> &tmpdicDetail, vector<vector<int>>&tmpDlPos, vector<int>&tmpDlArrayLeft, vector<int>&tmpDlArrayRight);
	
	//���
	void Padding(vector<vector<vector<bool>>>& tmpLRImage3DBool, vector<vector<bool>>& tmpHR2DBool);
	void Pad1(int tmpz, int tmpy, int tmpx);//ֻ�е���
	void Pad2(int tmpz, int tmpy, int tmpx);//�����ǰ��
	void Pad3(int tmpz, int tmpy, int tmpx);//��������
	void Pad4(int tmpz, int tmpy, int tmpx);//����
	void Padding3D(vector<vector<vector<bool>>>& tmpLRImage3DBool);

	//�ؽ��ķ�ʽ��ȡ3D
	vector<vector<vector<bool>>> LRImage3DBool;
	void get3DImageBool(vector<vector<vector<bool>>>& tmp3DImage);

	//������
	vector<vector<vector<bool>>> HRImage3DBool;
	vector<vector<vector<bool>>> HRImage3DWithoutRefBool;

	//��ȡ�ķ�ʽ��ȡ3D
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


	vector<  pair< vector<Point3i>, pair<Point3i, Point3i> >  > SmallPoreDic;//С���ֵ�
	vector<vector<int>>SPDistribution;//С����ͨ��ֲ�
	bool DicSPoreCreate();//С����Ϣ�ֵ�
	bool AddSmallPore(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<uchar>>>& dstImage3D, int ObjGray,double AddRate);//ͳ����ͨ����������ӦС��
	vector<vector<vector<uchar>>>Notice3D;//�ж��������
	bool MarkConnetedRegion_3D(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<uchar>>>& dstImage3D, int ObjGray);
	vector<vector<int>>LR3D_SPDist;


};