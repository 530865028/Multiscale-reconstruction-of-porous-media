#include "FileOpen.h"

FileOpen::FileOpen()
{
}

FileOpen::~FileOpen()
{
}

bool FileOpen::getImagePath(string tmpImagePath, string tmpImageName, string tmpImageExt)
{
	trainImagePath = tmpImagePath;
	trainImageName = tmpImageName;
	trainImageExt = tmpImageExt;
	return true;
}

bool FileOpen::imgLoad()
{
	Mat image2D;
	int tempStart = 0;
	string tempString;
	do
	{
		stringstream temp;
		temp << setw(4) << setfill('0') << tempStart;
		tempString = temp.str();
		string tempPath = trainImagePath + trainImageName + tempString + trainImageExt;
		image2D = cv::imread(tempPath, 0);

		if (!image2D.empty())
		{
			trainImage3D.push_back(image2D);
		}
		tempStart++;

	} while (!image2D.empty());

	trainImage3DBool.resize(trainImage3D.size());
	for (int i = 0; i < trainImage3DBool.size(); i++)
	{
		trainImage3DBool[i].resize(trainImage3D[i].rows);
		for (int j = 0; j < trainImage3DBool[i].size(); j++)
		{
			trainImage3DBool[i][j].resize(trainImage3D[i].cols);
			for (int k = 0; k < trainImage3DBool[i][j].size(); k++)
			{
				trainImage3DBool[i][j][k] = 0;
			}
		}
	}
	for (int z = 0; z < trainImage3D.size(); z++)
	{
		for (int y = 0; y < trainImage3D[z].rows; y++)
		{
			for (int x = 0; x < trainImage3D[z].cols; x++)
			{
				if (trainImage3D[z].at<uchar>(y, x) != 0)
					trainImage3DBool[z][y][x] = 1;
			}
		}
	}

	return true;
}
