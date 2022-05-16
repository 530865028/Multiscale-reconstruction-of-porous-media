#pragma once

#include <opencv2\opencv.hpp>
#include <bitset>

using namespace cv;
using namespace std;

class DetailDic
{
public:
	DetailDic();
	~DetailDic();

	public:
		int templateSize = 5;
		long int countRepeatNum = 0;
		
		vector<pair<bitset<27>, vector<bitset<125>>>> dicDetail;
		vector<vector<int>>DlPos;
		vector<pair<bitset<27>, vector<bitset<125>>>> YPCdicDetail;
		vector<vector<int>>YPCDlPos;

		vector<int>DlArrayLeft;
		vector<int>DlArrayRight;

		bool Dldiccreate();

		vector<vector<vector<bool>>> trainImage3DBool;
		bool get3DImageBool(vector<vector<vector<bool>>>& tmp3DImage);

		int ThreshNumTop = 126;//The biggest CC:  
		int ThreshNumBattom = 0;//The smallest CC:
		vector< vector <Point3i> > All_ConRegions;
		vector<  pair< vector<Point3i>,pair<Point3i,Point3i> >  > SmallPore_ConRegions;
		vector<  pair< vector<Point3i>, pair<Point3i, Point3i> >  > SmallPoreDic;
		vector<vector<int>>SPDistribution;
		vector<vector<vector<bool>>> dstImage3D;
		bool SmallPoreGet(vector<vector<vector<bool>>>& srcImage3D, int ObjGray,int ThreshNumTop, int ThreshNumBattom);
		bool MarkConnetedRegion_3D(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<bool>>>& dstImage3D, int ObjGray);
		bool ConnectedRegionOutput(vector<vector<vector<bool>>>& srcImage3D, int ObjGray);
		bool DicSPoreCreate();
		void SPoreDicWrite();
		void SPDSize(int tmpL);
};