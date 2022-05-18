#include "FilesProcess.h"

FilesProcess::FilesProcess(void)
{
}

FilesProcess::~FilesProcess(void)
{
}


void FilesProcess::getImgFromRec(vector<vector<vector<bool>>>& Image3D)
{
	recImage3DBool = Image3D;
	recYlength = Image3D[0].size();
    recXlength = Image3D[0][0].size();
    recZlength = Image3D.size();

}

void FilesProcess::loadRefImage(string tmpImagePath)
{
	/*--------------------------------------
	Load image
	---------------------------------------*/
	string refImagePathName = tmpImagePath;
	image2D = cv::imread(refImagePathName, 0);
	if (!image2D.data)
	{
		cout << "Load image fail！" << endl;
		return;
	}

	orgRefImage.resize(image2D.rows);
	for (int i = 0; i < orgRefImage.size(); i++)
	{
		orgRefImage[i].resize(image2D.cols);
	}

	for (int i = 0; i < image2D.rows; i++)
	{
		for (int j = 0; j < image2D.cols; j++)
		{
			if (image2D.at<uchar>(i, j) == 0)
				orgRefImage[i][j] = 0;
			else
				orgRefImage[i][j] = 1;
		}
	}
}

void FilesProcess::writeRecImage()
{
	recXlength = recImage3DBool.size();
	recYlength = recImage3DBool.size();
	/*--------------------------------------
	描述：先将Bool类型的数据转成Mat,然后写出
	---------------------------------------*/
	int tempStart = 0;
	string tempString;
	//stringstream tempfileNum;
	//tempfileNum << setw(3) << setfill('0') << fileNum;
	//string TEMPFILE = tempfileNum.str();

	Mat image = Mat::zeros(recYlength, recXlength, CV_8UC1);
	image2D = Mat::zeros(recYlength, recXlength, CV_8UC1);
	if (bDilateAndErode)
	{
		//
		//xy
		for (int i = 0; i < recZlength; i++)
		{
			for (int j = 0; j != recYlength; j++)
				for (int k = 0; k != recXlength; k++)
				{
					if (recImage3DBool[i][j][k] == 0)
						image2D.at<uchar>(j, k) = 0;
					else
						image2D.at<uchar>(j, k) = 255;
				}
			Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(templateSize, templateSize));
			morphologyEx(image2D, image2D, MORPH_DILATE, kernel, Point(-1, -1), 1, BORDER_REFLECT);//Dilatation
			morphologyEx(image2D, image2D, MORPH_ERODE, kernel, Point(-1, -1), 1, BORDER_REFLECT);//Corrosion
			for (int j = 0; j != recYlength; j++)
				for (int k = 0; k != recXlength; k++)
				{
					if (image2D.at<uchar>(j, k) == 0)
						recImage3DBool[i][j][k] = 0;
					else
						recImage3DBool[i][j][k] = 1;
				}
		}
		//xz
		for (int j = 0; j < recYlength; j++)
		{
			for (int i = 0; i != recZlength; i++)
				for (int k = 0; k != recXlength; k++)
				{
					if (recImage3DBool[i][j][k] == 0)
						image2D.at<uchar>(i, k) = 0;
					else
						image2D.at<uchar>(i, k) = 255;
				}
			Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(templateSize, templateSize));
			morphologyEx(image2D, image2D, MORPH_DILATE, kernel, Point(-1, -1), 1, BORDER_REFLECT);//Dilatation
			morphologyEx(image2D, image2D, MORPH_ERODE, kernel, Point(-1, -1), 1, BORDER_REFLECT);//Corrosion
			for (int i = 0; i != recZlength; i++)
				for (int k = 0; k != recXlength; k++)
				{
					if (image2D.at<uchar>(i, k) == 0)
						recImage3DBool[i][j][k] = 0;
					else
						recImage3DBool[i][j][k] = 1;
				}
		}
		//yz
		for (int k = 0; k < recXlength; k++)
		{
			for (int j = 0; j != recYlength; j++)
				for (int i = 0; i != recZlength; i++)
				{
					if (recImage3DBool[i][j][k] == 0)
						image2D.at<uchar>(j, i) = 0;
					else
						image2D.at<uchar>(j, i) = 255;
				}
			Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(templateSize, templateSize));
			morphologyEx(image2D, image2D, MORPH_DILATE, kernel, Point(-1, -1), 1, BORDER_REFLECT);//Dilatation
			morphologyEx(image2D, image2D, MORPH_ERODE, kernel, Point(-1, -1), 1, BORDER_REFLECT);//Corrosion
			for (int j = 0; j != recYlength; j++)
				for (int i = 0; i != recXlength; i++)
				{
					if (image2D.at<uchar>(j, i) == 0)
						recImage3DBool[i][j][k] = 0;
					else
						recImage3DBool[i][j][k] = 1;
				}
		}
	}	

	for (int i = 0; i != recZlength; i++)
	{
		for (int j = 0; j != recYlength; j++)
			for (int k = 0; k != recXlength; k++)
			{
				if (recImage3DBool[i][j][k] == 0)
					image.at<uchar>(j, k) = 0;
				else
					image.at<uchar>(j, k) = 255;
			}

		stringstream temp;
		temp << setw(4) << setfill('0') << tempStart;
		tempString = temp.str();

		string tempPath = recImagePath + recImageHeadName + tempString + recImageExt;
		imwrite(tempPath, image);
		tempStart++;
	}
}

void FilesProcess::getRecImg(string path, string name, string ext)
{
	recImagePath = path;
	recImageHeadName = name;
	recImageExt = ext;
}

void FilesProcess::calPoreSize(double& aveL_len, double& xigmaL_len, vector<vector<bool>>& Img)
{
	int height = Img.size();
	int width = Img[0].size();

	//
	vector<int>XLiner, YLiner;
	XLiner.resize(width + 1, 0);
	YLiner.resize(height + 1, 0);
	double XL_cnt = 0, YL_cnt = 0;
	double XL_len = 0;
	double YL_len = 0;
	double xigmaXL_len = 0;
	double xigmaYL_len = 0;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int tmp_len = 0;//

			while (Img[i][j] != 0)
			{
				tmp_len++;
				j++;
				if (j == width)
					break;
			}

			if (tmp_len > 0)
			{
				XLiner[tmp_len]++;
				XL_cnt++;
			}
		}
	}
	//
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < height; i++)
		{
			int tmp_len = 0;//

			while (Img[i][j] != 0)
			{
				tmp_len++;
				i++;
				if (i == height)
					break;
			}

			if (tmp_len > 0)
			{
				YLiner[tmp_len]++;
				YL_cnt++;
			}
		}
	}
	//
	for (int i = 1; i < XLiner.size(); i++)
	{
		XL_len += i * XLiner[i] / XL_cnt;
	}
	for (int j = 1; j < YLiner.size(); j++)
	{
		YL_len += j * YLiner[j] / YL_cnt;
	}
	aveL_len = (XL_len + YL_len) / 2.0;

	//
	for (int i = 1; i < XLiner.size(); i++)
	{
		xigmaXL_len += (i - XL_len) * (i - XL_len) * XLiner[i] / XL_cnt;
	}
	for (int i = 1; i < YLiner.size(); i++)
	{
		xigmaYL_len += (i - YL_len) * (i - YL_len) * YLiner[i] / YL_cnt;
	}
	xigmaL_len = (sqrt(xigmaXL_len) + sqrt(xigmaYL_len)) / 2.0;

	cout << "255相平均线性路径长度:" << aveL_len << endl;
	cout << "255相平均线性路径标准差长度:" << xigmaL_len << endl;

}

void FilesProcess::calStructSize(double& mix_size_x, double& mix_size_y, vector<vector<bool>>& Img)
{
	int height = Img.size();
	int width = Img[0].size();
	double Ex = 0.0, Ey = 0.0;
	int number_x = 0;               //
	int number_y = 0;               //
	int i = 0, j = 0;
	for (j = 0; j < height; j++)      //
	{
		for (i = 0; i < width - 1; i++)
		{
			if (Img[i][j] != Img[i][j + 1])
				++Ex;
		}
		++Ex;
	}
	mix_size_x = (width * height) * 1.0 / Ex;//

	for (i = 0; i < width; i++)         //
	{
		for (j = 0; j < height - 1; j++)
		{
			if (Img[j][i] != Img[j + 1][i])
				++Ey;
		}
		++Ey;
	}
	mix_size_y = (width * height) * 1.0 / Ey;       //
	cout << "the size of the mix face is " << mix_size_x << " * "<< mix_size_y << endl;
}

void FilesProcess::downSamplingOrg(int& downSamplingCount, vector<vector<bool>>& Img)
{
	int Orgheight = Img.size();
	int Orgwidth = Img[0].size();

	for (int k = 0; k < downSamplingCount; k++)
	{
		vector<vector<bool>>tmpDownSamplingImage;
		int tmpnum = pow(2, k);
		int height = Orgheight / tmpnum;
		int width = Orgwidth / tmpnum;
		tmpDownSamplingImage.resize(height);
		for (int i = 0; i < height; i++)
		{
			tmpDownSamplingImage[i].resize(width);
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				tmpDownSamplingImage[i][j] = Img[i * tmpnum][j * tmpnum];
			}
		}
		towerRefImage.push_back(tmpDownSamplingImage);
		pair<double, double>tmpPoreSize(0,0);
		calPoreSize(tmpPoreSize.first, tmpPoreSize.second, tmpDownSamplingImage);
		towerPoreSize.push_back(tmpPoreSize);

	}


}

void FilesProcess::calBestMatchSize(double &aveLen,double &xigmaLen,int& Countnum)
{
	if (towerPoreSize.size() == 0)
	{
		cout << "未计算孔径平均尺寸！！！" << endl;
		return;
	}
	else
	{
		double minErr = 999999;
		for (int k = 0; k < towerPoreSize.size(); k++)
		{
			double tmpErr = abs(towerPoreSize[k].first - aveLen) + abs(towerPoreSize[k].second - xigmaLen);
			if (tmpErr < minErr)
			{
				minErr = tmpErr;
				Countnum = k;
			}
		}
		cout << "最佳匹配匹配级数为：" << Countnum << endl;
	}
}

void FilesProcess::writeImage2D(vector<vector<bool>>& Img2D)
{
	int height = Img2D.size();
	int width = Img2D[0].size();
	if (height == 0 || width == 0)
	{
		cout << "写出2D图像信息错误！！！" << endl;
		return;
	}
	Mat outImage = Mat::zeros(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (Img2D[i][j] != 0)
				outImage.at<uchar>(i, j) = 255;
		}
	}
	string tempString="FinalRI";
	string tempPath = recImagePath + tempString + recImageExt;
	imwrite(tempPath, outImage);


}
