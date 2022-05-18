
#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "FilesProcess.h"
#include "Dictionary.h"
#include "DetailRe.h"
#include <direct.h>

using namespace std;
using namespace cv;

void main()
{
	DWORD  timeStart = GetTickCount();
	/*---------------------------------------------------------------------------------
	Parameters setting
	---------------------------------------------------------------------------------*/
	//template size
	int templateSize = 5;
	//load files in the folder
	string DetailDicPath = "DictionaryLib"; \\
	std::vector<cv::String>tmpFilename;
	glob(DetailDicPath + "\\EdgeDicLib_*.txt", tmpFilename, false);  \\ edge pattern dictionary
	if (tmpFilename.size() == 0)
	{
		cout << "No file£¡£¡£¡" << endl;
		return;
	}

	// 
	string recImagePath = "Reconstructed\\results\\"; //reconstruction results
	string recImageName = "RecImage";// 
	string recImageExt = ".bmp"; //
	//
	string TestLRImagePath = "Input\\"; //The LR images
	string TestLRImageName = "Image";
	string TestLRImageExt = ".bmp";


	DetailDic detaildic;
	FilesProcess file;
	detaildic.getLRImagePath(TestLRImagePath, TestLRImageName, TestLRImageExt);
	detaildic.imgLoad();
	file.getRecImg(recImagePath, recImageName, recImageExt);
	file.getImgFromRec(detaildic.LRImg3DReadBool);
	//file.writeRecImage();

	int Stageindex = 1;
	for (int iter = tmpFilename.size() - 1; iter >= 0; iter--)
	{

		cout << " Edge Reconstruction Stage:" << Stageindex << endl;
		Stageindex++;
     /*---------------------------------------------------------------------------------
     Load file
     ---------------------------------------------------------------------------------*/
		Dictionary dict;
		dict.DldicLoad(tmpFilename[iter]);
		detaildic.getDictionary(dict.dicDetail, dict.DlPos, dict.DlArrayLeft, dict.DlArrayRight);

	 /*---------------------------------------------------------------------------------
     Edge reconstrution
     ---------------------------------------------------------------------------------*/
		detaildic.Padding3D(file.recImage3DBool);
		file.getImgFromRec(detaildic.HRImage3DBool);
		//file.writeRecImage();
	}

	/*---------------------------------------------------------------------------------
   Micro-pores padding
    ---------------------------------------------------------------------------------*/
	cout << "Micro-pores Reconstruction:" << endl;
	detaildic.DicSPoreCreate();
	double tmpAddrate = 1;// defualt
	detaildic.AddSmallPore(detaildic.HRImage3DBool, detaildic.Notice3D, 1, tmpAddrate);
	cout << "Reconstruction successed!!!" << endl;
	file.writeRecImage();


	//
	char appPath[_MAX_PATH];
	_getcwd(appPath, _MAX_PATH);

	time_t sysTime;
	sysTime = time(&sysTime);

	DWORD timeEnd = GetTickCount();
	ofstream timeOut;
	DWORD workTime = 0;
	workTime = (timeEnd - timeStart) / 1000.0;
	timeOut.open("time.txt", ios_base::app);
	timeOut << ctime(&sysTime) << "direction = " << appPath << " runningtime=  " << workTime << "s" << endl << endl;
	timeOut.close();
	
}
