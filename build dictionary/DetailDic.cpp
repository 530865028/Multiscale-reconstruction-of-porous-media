#include "DetailDic.h"

DetailDic::DetailDic()
{
	DlPos.resize(28);
	YPCDlPos.resize(28);
	DlArrayLeft.resize(28);
	DlArrayRight.resize(28);

}

DetailDic::~DetailDic()
{
}

bool DetailDic::Dldiccreate()
{
	fstream dicDlFile;
	dicDlFile.open("results\\EdgeDicLib.txt", ios_base::in);

	if (!dicDlFile)
	{
		cout << "create the 'EdgeDicLib.txt' file!!!" << endl;
	}
	else
	{
		
		int tmpSize;
		dicDlFile >> tmpSize;
		
		for (int i = 0; i < tmpSize + 1; i++)
		{
			int tmphistSize;
			dicDlFile >> tmphistSize;
			if (tmphistSize != 0)
			{
				for (int cnt = 0; cnt < tmphistSize; cnt++)
				{
					int tmpValue;
					dicDlFile >> tmpValue;
					DlPos[i].push_back(tmpValue);
				}
			}
		}
		for (int i = 0; i < tmpSize + 1; i++)
		{
			int tmpValueLeft;
			int tmpValueRight;
			dicDlFile >> tmpValueLeft;
			dicDlFile >> tmpValueRight;
		}

		// load 3D block 
		while (!dicDlFile.eof())
		{
			char ch;
			bitset<27> bufKey;
			bitset<125> tmpValue;
			vector<bitset<125>> bufValue;
			ch = dicDlFile.peek();
			if (ch == EOF)
				break;

			dicDlFile >> bufKey;

			do
			{
				dicDlFile >> tmpValue;
				bufValue.push_back(tmpValue);

				ch = dicDlFile.get(); //
				ch = dicDlFile.peek(); //
			} while (ch != '\n');

			dicDetail.push_back(pair<bitset<27>, vector<bitset<125>>>(bufKey, bufValue));
			ch = dicDlFile.get();//
		}
	}
	dicDlFile.close();

	//
	for (int z = 0; z < trainImage3DBool.size() - templateSize + 1; z += 1)
	{
		if (z % 10 == 0)
		{
			cout << "Layer:" << z << endl;
		}
		for (int y = 0; y < trainImage3DBool[z].size() - templateSize + 1; y += 1)
			for (int x = 0; x < trainImage3DBool[z][y].size() - templateSize + 1; x += 1)
			{
				bitset<27> tmpKey(0);
				bitset<125> tmpValue(0);

				bool bKeyExist = 0;
				int index = 0;
				int tmpsum = 0;
				//Key
				for (int i = 0; i < templateSize; i+=2)
				{
					for (int j = 0; j < templateSize; j+=2)
					{
						for (int k = 0; k < templateSize; k+=2)
						{
							if (trainImage3DBool[z + i][y + j][x + k] == 1)
							{
								tmpKey.set(index, 1);
								tmpsum++;
							}
							index++;
						}
					}
				}


				//Value
				index = 0;
				//
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (trainImage3DBool[z + 0][y + j][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					if (trainImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (trainImage3DBool[z + i][y + j][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (trainImage3DBool[z + i][y + 0][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (trainImage3DBool[z + i][y + j][x + k] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
				}

				if (DlPos[tmpsum].size() == 0)
				{
					vector<bitset<125>> tmpValues;
					tmpValues.push_back(tmpValue);
					dicDetail.push_back(pair<bitset<27>, vector<bitset<125>>>(tmpKey, tmpValues));

					DlPos[tmpsum].push_back(dicDetail.size() - 1);
				}
				else
				{
					for (int i = 0; i < DlPos[tmpsum].size(); i++)
					{
						if (tmpKey == dicDetail[DlPos[tmpsum][i]].first)
						{
							bKeyExist = true;
							bool bValueExist = 0;
							for (int num = 0; num < dicDetail[DlPos[tmpsum][i]].second.size(); num++)
							{
								if (tmpValue == dicDetail[DlPos[tmpsum][i]].second[num])
								{
									if (tmpValue.count() != 0 && tmpValue != 0)
									{
										countRepeatNum++;
									}
									bValueExist = true;
									break;
								}
							}
							if (!bValueExist)
							{
								dicDetail[DlPos[tmpsum][i]].second.push_back(tmpValue);
							}
							break;
						}
					}
					if (!bKeyExist)
					{
						vector<bitset<125>> tmpValues;
						tmpValues.push_back(tmpValue);

						dicDetail.push_back(pair<bitset<27>, vector<bitset<125>>>(tmpKey, tmpValues));

						DlPos[tmpsum].push_back(dicDetail.size() - 1);
					}
				}
			}
	}
	//cout << dicDetail.size() << endl;

	//
	//YPC
	int tmp_histnum = 0;
	for (int i = 0; i < 28; i++)
	{
		DlArrayLeft[i] = tmp_histnum;
		if (DlPos[i].size() != 0)
		{
			for (int cnt = 0; cnt < DlPos[i].size(); cnt++)
			{
				YPCdicDetail.push_back(pair<bitset<27>, vector<bitset<125>>>(dicDetail[DlPos[i][cnt]].first, dicDetail[DlPos[i][cnt]].second));
				YPCDlPos[i].push_back(tmp_histnum);
				tmp_histnum++;
			}
		}
		DlArrayRight[i] = tmp_histnum;
		//cout << DlArrayLeft[i] << " " << DlArrayRight[i] << " " << endl;
	}
	//YPC

	//output
	dicDlFile.open("results\\EdgeDicLib.txt", ios_base::out);

	//YPC
	int weightsum = 27;
	dicDlFile << weightsum << endl;
	for (int i = 0; i < weightsum + 1; i++)
	{
		dicDlFile << YPCDlPos[i].size() << " ";
		if (YPCDlPos[i].size() != 0)
		{
			for (int cnt = 0; cnt < YPCDlPos[i].size(); cnt++)
			{
				dicDlFile << YPCDlPos[i][cnt] << " ";
			}
		}
		dicDlFile << endl;
	}
	for (int i = 0; i < weightsum + 1; i++)
	{
		dicDlFile << DlArrayLeft[i] << " " << DlArrayRight[i] << " ";
	}
	dicDlFile << endl;
	//YPC

	//output
	for (int i = 0; i < YPCdicDetail.size(); i++)
	{
		dicDlFile << YPCdicDetail[i].first << " ";
		for (int j = 0; j < YPCdicDetail[i].second.size(); j++)
		{
			dicDlFile << YPCdicDetail[i].second[j] << " ";
		}
		dicDlFile << endl;
	}
	dicDlFile.close();

	return true;
}

bool DetailDic::get3DImageBool(vector<vector<vector<bool>>>& tmp3DImage)
{
	trainImage3DBool = tmp3DImage;
	return true;
}

bool DetailDic::SmallPoreGet(vector<vector<vector<bool>>>& srcImage3D, int ObjGray,int ThreshNumTop, int ThreshNumBattom)
{
	int layer = srcImage3D.size();
	int height = srcImage3D[0].size();
	int width = srcImage3D[0][0].size();

	//
	vector<vector<vector<bool>>>Notaccessed;
	Notaccessed.resize(layer);
	for (int i = 0; i < layer; i++)
	{
		Notaccessed[i].resize(height);
		for (int j = 0; j < height; j++)
		{
			Notaccessed[i][j].resize(width);
			for (int k = 0; k < width; k++)
			{
				Notaccessed[i][j][k] = 1;
			}
		}
	}

	Point3i Current_Point;
	vector < Point3i >  Neighbor;
	vector <Point3i >  Region;
	vector< vector <Point3i> > ALL_Regions;

	for (int k = 0; k != layer; ++k)
	{
		for (int i = 0; i != height; ++i)
		{
			for (int j = 0; j != width; ++j)
			{
				int temp1 = Notaccessed[k][i][j];
				if (temp1 && srcImage3D[k][i][j] == ObjGray) 
				{
					Notaccessed[k][i][j] = 0;
					Region.clear();

					Neighbor.clear();
					Neighbor.push_back(Point3i(k, i, j));

					while (!Neighbor.empty())
					{
						Current_Point = Neighbor.back();
						Region.push_back(Current_Point);
						Neighbor.pop_back();

						for (int r = -1; r != 2; ++r)
						{
							for (int p = -1; p != 2; ++p)
							{
								for (int q = -1; q != 2; ++q)
								{
									
									int l = Current_Point.x;
									int m = Current_Point.y;
									int n = Current_Point.z;

									if ((l + r) < layer && (l + r) >= 0 && (m + p) < height && (m + p) >= 0 && (n + q) >= 0 && (n + q) < width)
									{
										int temp2 = Notaccessed[l + r][m + p][n + q];
										if (temp2 && srcImage3D[l + r][m + p][n + q] == ObjGray)
										{
											Notaccessed[l + r][m + p][n + q] = 0;  
											Neighbor.push_back(Point3i(l + r, m + p, n + q));
										}
									}
								}
							}
						}
					}
					ALL_Regions.push_back(Region); 
				}
			}
		}
	}


	for (int i = 0; i < ALL_Regions.size(); i++)
	{
		
		Point3i minPoint(999999, 999999, 999999);
		Point3i maxPoint(0, 0, 0);

		if (ALL_Regions[i].size() < ThreshNumTop && ALL_Regions[i].size() > ThreshNumBattom)
		{
			vector<Point3i>tmpRegion= ALL_Regions[i];
			
			for (int count_num = 0; count_num < tmpRegion.size(); count_num++)
			{
				
				if (tmpRegion[count_num].x > maxPoint.x)
				{
					maxPoint.x = tmpRegion[count_num].x;
				}
				if (tmpRegion[count_num].y > maxPoint.y)
				{
					maxPoint.y = tmpRegion[count_num].y;
				}
				if (tmpRegion[count_num].z > maxPoint.z)
				{
					maxPoint.z = tmpRegion[count_num].z;
				}
				
				if (tmpRegion[count_num].x < minPoint.x)
				{
					minPoint.x = tmpRegion[count_num].x;
				}
				if (tmpRegion[count_num].y < minPoint.y)
				{
					minPoint.y = tmpRegion[count_num].y;
				}
				if (tmpRegion[count_num].z < minPoint.z)
				{
					minPoint.z = tmpRegion[count_num].z;
				}
			}
			//cout << minPoint << maxPoint << endl;
			SmallPore_ConRegions.push_back(pair<vector<Point3i>, pair<Point3i, Point3i>>(tmpRegion, make_pair(minPoint, maxPoint)));
		}
	}
	return true;
}


bool DetailDic::DicSPoreCreate()
{

	fstream dicSmallPoreFile;
	dicSmallPoreFile.open("results\\SmallPoreLib.txt", ios_base::in);
	if (!dicSmallPoreFile)
	{
		cout << "create the 'SmallPoreLib.txt' file!!!" << endl;
	}
	else
	{
		
		int tmpSize;
		dicSmallPoreFile >> tmpSize;
		for (int i = 0; i < tmpSize; i++)
		{
			
			int tmpSPNum;
			dicSmallPoreFile >> tmpSPNum;
			vector<Point3i>tmpRegion;
			for (int cnt = 0; cnt < tmpSPNum; cnt++)
			{
				
				Point3i tmpPoint;
				dicSmallPoreFile >> tmpPoint.x;
				dicSmallPoreFile >> tmpPoint.y;
				dicSmallPoreFile >> tmpPoint.z;
				tmpRegion.push_back(tmpPoint);
			}
			Point3i tmpStartPoint, tmpEndPoint;
			{
				
				dicSmallPoreFile >> tmpStartPoint.x;
				dicSmallPoreFile >> tmpStartPoint.y;
				dicSmallPoreFile >> tmpStartPoint.z;
				dicSmallPoreFile >> tmpEndPoint.x;
				dicSmallPoreFile >> tmpEndPoint.y;
				dicSmallPoreFile >> tmpEndPoint.z;
			}
			SmallPoreDic.push_back(pair<vector<Point3i>, pair<Point3i, Point3i>>(tmpRegion, make_pair(tmpStartPoint, tmpEndPoint)));
		}

		
		dicSmallPoreFile >> tmpSize;
		//SPDistribution.resize(tmpSize);
		for (int i = 0; i < tmpSize; i++)
		{
			int tmphistSize;
			dicSmallPoreFile >> tmphistSize;
			if (tmphistSize != 0)
			{
				for (int cnt = 0; cnt < tmphistSize; cnt++)
				{
					int tmpValue;
					dicSmallPoreFile >> tmpValue;
					SPDistribution[i].push_back(tmpValue);
				}
			}
		}
	}
	dicSmallPoreFile.close();

	
	for (int count_num = 0; count_num < SmallPore_ConRegions.size(); count_num++)
	{
		
		int tmpLp;
		for (tmpLp = 0; tmpLp * tmpLp * tmpLp <= SmallPore_ConRegions[count_num].first.size(); tmpLp++);
		//SPDistribution[tmpLp - 1].push_back(count_num);
		SmallPoreDic.push_back(SmallPore_ConRegions[count_num]);
		SPDistribution[tmpLp - 1].push_back(SmallPoreDic.size() - 1);
	}
	
	dicSmallPoreFile.open("results\\SmallPoreLib.txt", ios_base::out);
	dicSmallPoreFile << SmallPoreDic.size() << endl;
	for (int i = 0; i < SmallPoreDic.size(); i++)
	{
		
		dicSmallPoreFile << SmallPoreDic[i].first.size() << " ";
		for (int cnt = 0; cnt < SmallPoreDic[i].first.size(); cnt++)
		{
			dicSmallPoreFile << SmallPoreDic[i].first[cnt].x << " " << SmallPoreDic[i].first[cnt].y << " " << SmallPoreDic[i].first[cnt].z << " ";
		}
		
		{
			dicSmallPoreFile << SmallPoreDic[i].second.first.x << " " << SmallPoreDic[i].second.first.y << " " << SmallPoreDic[i].second.first.z << " ";
			dicSmallPoreFile << SmallPoreDic[i].second.second.x << " " << SmallPoreDic[i].second.second.y << " " << SmallPoreDic[i].second.second.z << " ";
		}
		dicSmallPoreFile << endl;
	}
	dicSmallPoreFile << SPDistribution.size() << endl;
	for (int i = 0; i < SPDistribution.size(); i++)
	{
		dicSmallPoreFile << SPDistribution[i].size() << " ";
		if (SPDistribution[i].size() != 0)
		{
			for (int j = 0; j < SPDistribution[i].size(); j++)
			{
				dicSmallPoreFile << SPDistribution[i][j] << " ";
			}
		}
		dicSmallPoreFile << endl;
	}
	dicSmallPoreFile.close();

	return true;
}


void DetailDic::SPDSize(int tmpL)
{
	SPDistribution.resize(tmpL);
}



