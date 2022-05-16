#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class FileOpen
{
public:
	FileOpen();
	~FileOpen();

public:
	string trainImagePath;
	string trainImageName;
	string trainImageExt;

	vector<Mat> trainImage3D;
	vector<vector<vector<bool>>> trainImage3DBool;

	bool getImagePath(string tmpImagePath, string tmpImageName, string tmpImageExt);
	bool imgLoad();
};

