
// DictionaryCreate.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <opencv2\opencv.hpp>
#include "FileOpen.h"
#include "DetailDic.h"

using namespace std;
using namespace cv;

int main()
{

	DWORD  timeStart = GetTickCount();

	/*---------------------------------------------------------------------------------
     Build Edge pattern dictionary
    ---------------------------------------------------------------------------------*/
	cout << "Build edge dictionary: " << endl;
	string trainImagePath = "HRImage\\Edge\\"; // LoadHRImage 
	string trainImageName = "Image";//
	string trainImageExt = ".bmp"; //

	FileOpen file;
	file.getImagePath(trainImagePath, trainImageName, trainImageExt);
	file.imgLoad();

	DetailDic Ddic;
	Ddic.get3DImageBool(file.trainImage3DBool);
	Ddic.Dldiccreate();
	cout << "Build edge dictionary successed!!! " << endl;


	/*---------------------------------------------------------------------------------
	  Build Micro-pore pattern dictionary
	---------------------------------------------------------------------------------*/
	cout << "Build micro-pores dictionary: " << endl;

	trainImagePath = "HRImage\\Micro-pores\\"; //LoadHRImage 
	trainImageName = "Image";//
	trainImageExt = ".bmp"; //
	file.getImagePath(trainImagePath, trainImageName, trainImageExt);
	file.imgLoad();
	int max_ConSize = 126, min_ConSize = 0;
	cout << "The biggest CC：";
	cin >> max_ConSize;
	cout << "The smaallest CC：";
	cin >> min_ConSize;
	int tmpLpSize;
	for (tmpLpSize = 0; tmpLpSize * tmpLpSize * tmpLpSize <= max_ConSize; tmpLpSize++);
	Ddic.SPDSize(tmpLpSize + 1);
	Ddic.SmallPoreGet(Ddic.trainImage3DBool, 1, max_ConSize, min_ConSize);
	Ddic.DicSPoreCreate();

	cout << "Build micro-pores dictionary successed!!! " << endl;

	//Running time
	DWORD timeEnd = GetTickCount64();
	DWORD workTime = 0;
	workTime = (timeEnd - timeStart) / 1.0;
	cout << "Runningtime：" << workTime << endl;
	return 0;
}

