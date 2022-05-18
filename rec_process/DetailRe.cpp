
#include "DetailRe.h"
#include"time.h"

DetailDic::DetailDic()
{
}

DetailDic::~DetailDic()
{
}

void DetailDic::getDictionary(vector<pair<bitset<27>, vector<bitset<125>>>>& tmpdicDetail, vector<vector<int>>& tmpDlPos, vector<int>& tmpDlArrayLeft, vector<int>& tmpDlArrayRight)
{
	dicDetail = tmpdicDetail;
	DlPos = tmpDlPos;
	DlArrayLeft = tmpDlArrayLeft;
	DlArrayRight = tmpDlArrayRight;
}

void DetailDic::Padding(vector<vector<vector<bool>>> &tmpLRImage3DBool, vector<vector<bool>>& tmpHR2DBool)
{
	//
	int PadZSize = (2 * tmpLRImage3DBool.size() + 1) - ((2 * tmpLRImage3DBool.size() + 1) - 1) % 4;
	int PadYSize = (2 * tmpLRImage3DBool[0].size() - 1) - ((2 * tmpLRImage3DBool[0].size() - 1) - 1) % 4;
	int PadXSize = (2 * tmpLRImage3DBool[0][0].size() - 1) - ((2 * tmpLRImage3DBool[0][0].size() - 1) - 1) % 4;
	HRImage3DBool.resize(PadZSize);
	for (int k = 0; k < PadZSize; k++)
	{
		HRImage3DBool[k].resize(PadYSize);
		for (int i = 0; i < PadYSize; i++)
		{
			HRImage3DBool[k][i].resize(PadXSize);
		}
	}
	HRImage3DWithoutRefBool.resize(PadZSize - 1);
	for (int k = 0; k < PadZSize - 1; k++)
	{
		HRImage3DWithoutRefBool[k].resize(PadYSize);
		for (int i = 0; i < PadYSize; i++)
		{
			HRImage3DWithoutRefBool[k][i].resize(PadXSize);
		}
	}

	//
	for (int i = 0; i < PadYSize; i++)
	{
		for (int j = 0; j < PadXSize; j++)
		{
			HRImage3DBool[0][i][j] = tmpHR2DBool[i][j];
		}
	}
	//
	for (int k = 2; k < PadZSize; k += 2)
	{
		for (int i = 0; i < PadYSize; i += 2)
		{
			for (int j = 0; j < PadXSize; j += 2)
			{
				HRImage3DBool[k][i][j] = tmpLRImage3DBool[k / 2 - 1][i / 2][j / 2];
			}
		}
	}

	//
	int z = 0, x = 0, y = 0;
	//
	x = 0, y = 0;
	for (z = 0; z < PadZSize-4; z += 4)
	{
		bitset<27> tmpKey(0);
		int index = 0;
		int tmpsum = 0;
		//Key
		for (int i = 0; i < templateSize; i += 2)
		{
			for (int j = 0; j < templateSize; j += 2)
			{
				for (int k = 0; k < templateSize; k += 2)
				{
					if (HRImage3DBool[z + i][y + j][x + k] == 1)
					{
						tmpKey.set(index, 1);
						tmpsum++;
					}
					index++;
				}
			}
		}
		//
		if (tmpsum == 0)
		{
			for (int i = 0; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						HRImage3DBool[z + i][y + j][x + k] = 0;
					}
				}
			}
		}
		else if (tmpsum == 27)
		{
			for (int i = 0; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						HRImage3DBool[z + i][y + j][x + k] = 1;
					}
				}
			}
		}
		else
		{
			//Value
			bitset<25> tmpValue(0);
			//bitset<25> AndVBit;//
			bitset<27> AndKBit;//
			pair<int, int>Kpos(0, 0);
			pair<int, int>Vpos(0, 0);
			index = 0;
			//
			for (int j = 0; j < templateSize; j++)
			{
				for (int k = 0; k < templateSize; k++)
				{
					if (HRImage3DBool[z + 0][y + j][x + k] == 1)
						tmpValue.set(index, 1);
					index++;
				}
			}
			
			{
				//
				//for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
				for (int tmpknum = 0; tmpknum < dicDetail.size(); tmpknum++)
				{
					AndKBit = tmpKey & dicDetail[tmpknum].first;
					int tmpmatch = AndKBit.count();
					if (Kpos.second < tmpmatch)
					{
						Kpos.first = tmpknum;
						Kpos.second = tmpmatch;
					}
				}
				//
				for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
				{
					int tmpmatch = 0;
					for (int mm = 0; mm < 25; mm++)
					{
						if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
						{
							tmpmatch++;
						}
					}
					if (Vpos.second < tmpmatch)
					{
						Vpos.first = tmpvnum;
						Vpos.second = tmpmatch;
					}
				}
			}
			
			index = 25;
			
			for (int i = 1; i < templateSize; i++)
			{
				if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
					HRImage3DBool[z + i][y + 0][x + 0] = 1;
				else
					HRImage3DBool[z + i][y + 0][x + 0] = 0;

				index++;
			}
			//
			for (int i = 1; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + i][y + j][x + 0] = 1;
					else
						HRImage3DBool[z + i][y + j][x + 0] = 0;

					index++;
				}
			}
			//
			for (int i = 1; i < templateSize; i++)
			{
				for (int k = 1; k < templateSize; k++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + i][y + 0][x + k] = 1;
					else
						HRImage3DBool[z + i][y + 0][x + k] = 0;

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
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + j][x + 0] = 1;
						else
							HRImage3DBool[z + i][y + j][x + 0] = 0;

						index++;
					}
				}
			}
		}

	}
	//2
	x = 0;
	for (z = 0; z < PadZSize-4; z += 4)
	{
		for (y = 4; y < PadYSize-4; y += 4)
		{
			bitset<27> tmpKey(0);
			int index = 0;
			int tmpsum = 0;
			//Key
			for (int i = 0; i < templateSize; i += 2)
			{
				for (int j = 0; j < templateSize; j += 2)
				{
					for (int k = 0; k < templateSize; k += 2)
					{
						if (HRImage3DBool[z + i][y + j][x + k] == 1)
						{
							tmpKey.set(index, 1);
							tmpsum++;
						}
						index++;
					}
				}
			}
			if (tmpsum == 0)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 0;
						}
					}
				}
			}
			else if (tmpsum == 27)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 1;
						}
					}
				}
			}
			else
			{
				//Value
				bitset<45> tmpValue(0);
				//bitset<25> AndVBit;//
				bitset<27> AndKBit;//
				pair<int, int>Kpos(0, 0);
				pair<int, int>Vpos(0, 0);
				index = 0;
				//
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (HRImage3DBool[z + 0][y + j][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (HRImage3DBool[z + i][y + 0][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//if (DlPos[tmpsum].size() == 0)
				//{
				//	cout << "字典不完备！！！" << endl;
				//	return;
				//}
				//else
				{
					//
				    for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					{
						AndKBit = tmpKey & dicDetail[tmpknum].first;
						int tmpmatch = AndKBit.count();
						if (Kpos.second < tmpmatch)
						{
							Kpos.first = tmpknum;
							Kpos.second = tmpmatch;
						}
					}
					//
					for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
					{
						int tmpmatch = 0;
						for (int mm = 0; mm < 25 + 4; mm++)
						{
							if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						for (int mm = 25 + 4 + 16; mm < 25 + 4 + 16 + 16; mm++)
						{
							if (tmpValue[mm - 16] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						if (Vpos.second < tmpmatch)
						{
							Vpos.first = tmpvnum;
							Vpos.second = tmpmatch;
						}
					}
				}
				//
				
				index = 25;
				//
				index += 4;
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + j][x + 0] = 1;
						else
							HRImage3DBool[z + i][y + j][x + 0] = 0;

						index++;
					}
				}
				//
				index += 16;
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + i][y + j][x + k] = 1;
							else
								HRImage3DBool[z + i][y + j][x + k] = 0;

							index++;
						}
					}
				}
			}
		}


	}
	//3.
	y = 0;
	for (z = 0; z < PadZSize - 4; z += 4)
	{
		if (z % 10 == 0)
		{
			cout << "Layer z=" << z << endl;
		}
		for (x = 4; x < PadXSize-4; x += 4)
		{
			bitset<27> tmpKey(0);
			int index = 0;
			int tmpsum = 0;
			//Key
			for (int i = 0; i < templateSize; i += 2)
			{
				for (int j = 0; j < templateSize; j += 2)
				{
					for (int k = 0; k < templateSize; k += 2)
					{
						if (HRImage3DBool[z + i][y + j][x + k] == 1)
						{
							tmpKey.set(index, 1);
							tmpsum++;
						}
						index++;
					}
				}
			}
			if (tmpsum == 0)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 0;
						}
					}
				}
			}
			else if (tmpsum == 27)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 1;
						}
					}
				}
			}
			else
			{
				//Value
				bitset<45> tmpValue(0);
				//bitset<25> AndVBit;
				bitset<27> AndKBit;
				pair<int, int>Kpos(0, 0);
				pair<int, int>Vpos(0, 0);
				index = 0;
				//
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (HRImage3DBool[z + 0][y + j][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (HRImage3DBool[z + i][y + j][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//if (DlPos[tmpsum].size() == 0)
				//{
				//	cout << "字典不完备！！！" << endl;
				//	return;
				//}
				//else
				{
					//
					for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					{
						AndKBit = tmpKey & dicDetail[tmpknum].first;
						int tmpmatch = AndKBit.count();
						if (Kpos.second < tmpmatch)
						{
							Kpos.first = tmpknum;
							Kpos.second = tmpmatch;
						}
					}
					//
					for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
					{
						int tmpmatch = 0;
						for (int mm = 0; mm < 25 + 4 + 16; mm++)
						{
							if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						if (Vpos.second < tmpmatch)
						{
							Vpos.first = tmpvnum;
							Vpos.second = tmpmatch;
						}
					}
				}
				//
				index = 25;
				//
				index += 4;
				//left16
				index += 16;
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + 0][x + k] = 1;
						else
							HRImage3DBool[z + i][y + 0][x + k] = 0;

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
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + i][y + j][x + k] = 1;
							else
								HRImage3DBool[z + i][y + j][x + k] = 0;

							index++;
						}
					}
				}
			}

		}
	}
	//4.
	for (z = 0; z < PadZSize-4; z += 4)
	{
		if (z % 10 == 0)
		{
			cout << "Layer z=" << z << endl;
		}
		for (y = 4; y < PadYSize-4; y += 4)
		{
			for (x = 4; x < PadXSize-4; x += 4)
			{
				bitset<27> tmpKey(0);
				int index = 0;
				int tmpsum = 0;
				//Key
				for (int i = 0; i < templateSize; i += 2)
				{
					for (int j = 0; j < templateSize; j += 2)
					{
						for (int k = 0; k < templateSize; k += 2)
						{
							if (HRImage3DBool[z + i][y + j][x + k] == 1)
							{
								tmpKey.set(index, 1);
								tmpsum++;
							}
							index++;
						}
					}
				}
				if (tmpsum == 0)
				{
					for (int i = 0; i < templateSize; i++)
					{
						for (int j = 0; j < templateSize; j++)
						{
							for (int k = 0; k < templateSize; k++)
							{
								HRImage3DBool[z + i][y + j][x + k] = 0;
							}
						}
					}
				}
				else if (tmpsum == 27)
				{
					for (int i = 0; i < templateSize; i++)
					{
						for (int j = 0; j < templateSize; j++)
						{
							for (int k = 0; k < templateSize; k++)
							{
								HRImage3DBool[z + i][y + j][x + k] = 1;
							}
						}
					}
				}
				else
				{
					//Value
					bitset<61> tmpValue(0);
					//bitset<25> AndVBit;
					bitset<27> AndKBit;
					pair<int, int>Kpos(0, 0);
					pair<int, int>Vpos(0, 0);
					index = 0;
					//
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							if (HRImage3DBool[z + 0][y + j][x + k] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//
					for (int i = 1; i < templateSize; i++)
					{
						if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
					//
					for (int i = 1; i < templateSize; i++)
					{
						for (int j = 1; j < templateSize; j++)
						{
							if (HRImage3DBool[z + i][y + j][x + 0] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//
					for (int i = 1; i < templateSize; i++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (HRImage3DBool[z + i][y + 0][x + k] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//if (DlPos[tmpsum].size() == 0)
					//{
					//	cout << "字典不完备！！！" << endl;
					//	return;
					//}
					//else
					{
						//
						for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
						{
							AndKBit = tmpKey & dicDetail[tmpknum].first;
							int tmpmatch = AndKBit.count();
							if (Kpos.second < tmpmatch)
							{
								Kpos.first = tmpknum;
								Kpos.second = tmpmatch;
							}
						}
						//
						for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
						{
							int tmpmatch = 0;
							for (int mm = 0; mm < 61; mm++)
							{
								if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
								{
									tmpmatch++;
								}
							}
							if (Vpos.second < tmpmatch)
							{
								Vpos.first = tmpvnum;
								Vpos.second = tmpmatch;
							}
						}
					}
					
					index = 25;
					//
					index += 4;
					//
					index += 16;
					//
					index += 16;					
					//
					for (int i = 1; i < templateSize; i++)
					{
						for (int j = 1; j < templateSize; j++)
						{
							for (int k = 1; k < templateSize; k++)
							{
								if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
									HRImage3DBool[z + i][y + j][x + k] = 1;
								else
									HRImage3DBool[z + i][y + j][x + k] = 0;

								index++;
							}
						}
					}
				}

			}
		}
	}

	//
	for (int i = 0; i < PadZSize - 1; i++)
	{
		for (int j = 0; j < PadYSize; j++)
		{
			for (int k = 0; k < PadXSize; k++)
			{
				HRImage3DWithoutRefBool[i][j][k] = HRImage3DBool[i + 1][j][k];
			}
		}
	}
}

void DetailDic::Pad1(int tmpz, int tmpy, int tmpx)
{
}

void DetailDic::Pad2(int tmpz, int tmpy, int tmpx)
{
}

void DetailDic::Pad3(int tmpz, int tmpy, int tmpx)
{
}

void DetailDic::Pad4(int tmpz, int tmpy, int tmpx)
{
}

void DetailDic::Padding3D(vector<vector<vector<bool>>>& tmpLRImage3DBool)
{
	srand((unsigned)time(0));//Initialize the random seed
	//
	int PadZSize = (2 * tmpLRImage3DBool.size() - 1) - ((2 * tmpLRImage3DBool.size() - 1) - 1) % 4;
	int PadYSize = (2 * tmpLRImage3DBool[0].size() - 1) - ((2 * tmpLRImage3DBool[0].size() - 1) - 1) % 4;
	int PadXSize = (2 * tmpLRImage3DBool[0][0].size() - 1) - ((2 * tmpLRImage3DBool[0][0].size() - 1) - 1) % 4;
	HRImage3DBool.resize(PadZSize);
	for (int k = 0; k < PadZSize; k++)
	{
		HRImage3DBool[k].resize(PadYSize);
		for (int i = 0; i < PadYSize; i++)
		{
			HRImage3DBool[k][i].resize(PadXSize);
		}
	}
	//
	for (int k = 0; k < PadZSize; k += 2)
	{
		for (int i = 0; i < PadYSize; i += 2)
		{
			for (int j = 0; j < PadXSize; j += 2)
			{
				HRImage3DBool[k][i][j] = tmpLRImage3DBool[k / 2][i / 2][j / 2];
			}
		}
	}
	//
	int z = 0, x = 0, y = 0;
	//0.
	
	x = 0, y = 0, z = 0;
	{
		bitset<27> tmpKey(0);
		int index = 0;
		int tmpsum = 0;
		//Key
		for (int i = 0; i < templateSize; i += 2)
		{
			for (int j = 0; j < templateSize; j += 2)
			{
				for (int k = 0; k < templateSize; k += 2)
				{
					if (HRImage3DBool[z + i][y + j][x + k] == 1)
					{
						tmpKey.set(index, 1);
						tmpsum++;
					}
					index++;
				}
			}
		}
		//
		if (tmpsum == 0)
		{
			for (int i = 0; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						HRImage3DBool[z + i][y + j][x + k] = 0;
					}
				}
			}
		}
		else if (tmpsum == 27)
		{
			for (int i = 0; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						HRImage3DBool[z + i][y + j][x + k] = 1;
					}
				}
			}
		}
		else
		{
			//Value
			bitset<27> AndKBit;
			pair<int, int>Kpos(0, 0);
			pair<int, int>Vpos(0, 0);
			index = 0;
			//if (DlPos[tmpsum].size() == 0)
			//{
			//	cout << "字典不完备！！！" << endl;
			//	return;
			//}
			//else
			{
				//
				for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					//for (int tmpknum = 0; tmpknum < dicDetail.size(); tmpknum++)
				{
					AndKBit = tmpKey & dicDetail[tmpknum].first;
					int tmpmatch = AndKBit.count();
					if (Kpos.second < tmpmatch)
					{
						Kpos.first = tmpknum;
						Kpos.second = tmpmatch;
					}
				}
				//
				Vpos.first = rand() % dicDetail[Kpos.first].second.size();
			}
			
			index = 0;
			for (int j = 0; j < templateSize; j++)
			{
				for (int k = 0; k < templateSize; k++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + 0][y + j][x + k] = 1;
					else
						HRImage3DBool[z + 0][y + j][x + k] = 0;

					index++;
				}
			}
			//
			for (int i = 1; i < templateSize; i++)
			{
				if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
					HRImage3DBool[z + i][y + 0][x + 0] = 1;
				else
					HRImage3DBool[z + i][y + 0][x + 0] = 0;

				index++;
			}
			//
			for (int i = 1; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + i][y + j][x + 0] = 1;
					else
						HRImage3DBool[z + i][y + j][x + 0] = 0;

					index++;
				}
			}
			//
			for (int i = 1; i < templateSize; i++)
			{
				for (int k = 1; k < templateSize; k++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + i][y + 0][x + k] = 1;
					else
						HRImage3DBool[z + i][y + 0][x + k] = 0;

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
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + j][x + 0] = 1;
						else
							HRImage3DBool[z + i][y + j][x + 0] = 0;

						index++;
					}
				}
			}
		}
	}
	//
	x = 0;
	{
		for (y = 4; y < PadYSize - 4; y += 4)
		{
			bitset<27> tmpKey(0);
			int index = 0;
			int tmpsum = 0;
			//Key
			for (int i = 0; i < templateSize; i += 2)
			{
				for (int j = 0; j < templateSize; j += 2)
				{
					for (int k = 0; k < templateSize; k += 2)
					{
						if (HRImage3DBool[z + i][y + j][x + k] == 1)
						{
							tmpKey.set(index, 1);
							tmpsum++;
						}
						index++;
					}
				}
			}
			if (tmpsum == 0)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 0;
						}
					}
				}
			}
			else if (tmpsum == 27)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 1;
						}
					}
				}
			}
			else
			{
				//Value
				bitset<20> tmpValue(0);
				bitset<27> AndKBit;//
				pair<int, int>Kpos(0, 0);
				pair<int, int>Vpos(0, 0);
				index = 0;
				
				for (int i = 1; i < templateSize; i++)
				{
					if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (HRImage3DBool[z + i][y + 0][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//if (DlPos[tmpsum].size() == 0)
				//{
				//	cout << "字典不完备！！！" << endl;
				//	return;
				//}
				//else
				{
					
					for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					{
						AndKBit = tmpKey & dicDetail[tmpknum].first;
						int tmpmatch = AndKBit.count();
						if (Kpos.second < tmpmatch)
						{
							Kpos.first = tmpknum;
							Kpos.second = tmpmatch;
						}
					}
					
					for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
					{
						int tmpmatch = 0;
						for (int mm = 25; mm < 25 + 4; mm++)
						{
							if (tmpValue[mm - 25] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						for (int mm = 25 + 4 + 16; mm < 25 + 4 + 16 + 16; mm++)
						{
							if (tmpValue[mm - 16 - 25] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						if (Vpos.second < tmpmatch)
						{
							Vpos.first = tmpvnum;
							Vpos.second = tmpmatch;
						}
					}
				}
				
				index = 0;
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + 0][y + j][x + k] = 1;
						else
							HRImage3DBool[z + 0][y + j][x + k] = 0;

						index++;
					}
				}
				//
				index += 4;
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + j][x + 0] = 1;
						else
							HRImage3DBool[z + i][y + j][x + 0] = 0;

						index++;
					}
				}
				//
				index += 16;
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + i][y + j][x + k] = 1;
							else
								HRImage3DBool[z + i][y + j][x + k] = 0;

							index++;
						}
					}
				}
			}
		}
	}
	//
	y = 0;
	{
		for (x = 4; x < PadXSize - 4; x += 4)
		{
			bitset<27> tmpKey(0);
			int index = 0;
			int tmpsum = 0;
			//Key
			for (int i = 0; i < templateSize; i += 2)
			{
				for (int j = 0; j < templateSize; j += 2)
				{
					for (int k = 0; k < templateSize; k += 2)
					{
						if (HRImage3DBool[z + i][y + j][x + k] == 1)
						{
							tmpKey.set(index, 1);
							tmpsum++;
						}
						index++;
					}
				}
			}
			if (tmpsum == 0)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 0;
						}
					}
				}
			}
			else if (tmpsum == 27)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 1;
						}
					}
				}
			}
			else
			{
				//Value
				bitset<20> tmpValue(0);
				bitset<27> AndKBit;
				pair<int, int>Kpos(0, 0);
				pair<int, int>Vpos(0, 0);
				index = 0;
				
				for (int i = 1; i < templateSize; i++)
				{
					if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (HRImage3DBool[z + i][y + j][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//if (DlPos[tmpsum].size() == 0)
				//{
				//	cout << "字典不完备！！！" << endl;
				//	return;
				//}
				//else
				{
					
					for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					{
						AndKBit = tmpKey & dicDetail[tmpknum].first;
						int tmpmatch = AndKBit.count();
						if (Kpos.second < tmpmatch)
						{
							Kpos.first = tmpknum;
							Kpos.second = tmpmatch;
						}
					}
					
					for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
					{
						int tmpmatch = 0;
						for (int mm = 25; mm < 25 + 4 + 16; mm++)
						{
							if (tmpValue[mm - 25] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						if (Vpos.second < tmpmatch)
						{
							Vpos.first = tmpvnum;
							Vpos.second = tmpmatch;
						}
					}
				}
				
				index = 0;
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + 0][y + j][x + k] = 1;
						else
							HRImage3DBool[z + 0][y + j][x + k] = 0;

						index++;
					}
				}
				//
				index += 4;
				//
				index += 16;
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + 0][x + k] = 1;
						else
							HRImage3DBool[z + i][y + 0][x + k] = 0;

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
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + i][y + j][x + k] = 1;
							else
								HRImage3DBool[z + i][y + j][x + k] = 0;

							index++;
						}
					}
				}
			}
		}
	}
	
	{
		for (y = 4; y < PadYSize - 4; y += 4)
		{
			for (x = 4; x < PadXSize - 4; x += 4)
			{
				bitset<27> tmpKey(0);
				int index = 0;
				int tmpsum = 0;
				//Key
				for (int i = 0; i < templateSize; i += 2)
				{
					for (int j = 0; j < templateSize; j += 2)
					{
						for (int k = 0; k < templateSize; k += 2)
						{
							if (HRImage3DBool[z + i][y + j][x + k] == 1)
							{
								tmpKey.set(index, 1);
								tmpsum++;
							}
							index++;
						}
					}
				}
				if (tmpsum == 0)
				{
					for (int i = 0; i < templateSize; i++)
					{
						for (int j = 0; j < templateSize; j++)
						{
							for (int k = 0; k < templateSize; k++)
							{
								HRImage3DBool[z + i][y + j][x + k] = 0;
							}
						}
					}
				}
				else if (tmpsum == 27)
				{
					for (int i = 0; i < templateSize; i++)
					{
						for (int j = 0; j < templateSize; j++)
						{
							for (int k = 0; k < templateSize; k++)
							{
								HRImage3DBool[z + i][y + j][x + k] = 1;
							}
						}
					}
				}
				else
				{
					//Value
					bitset<36> tmpValue(0);
					bitset<27> AndKBit;
					pair<int, int>Kpos(0, 0);
					pair<int, int>Vpos(0, 0);
					index = 0;
					
					
					for (int i = 1; i < templateSize; i++)
					{
						if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
					
					for (int i = 1; i < templateSize; i++)
					{
						for (int j = 1; j < templateSize; j++)
						{
							if (HRImage3DBool[z + i][y + j][x + 0] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					
					for (int i = 1; i < templateSize; i++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (HRImage3DBool[z + i][y + 0][x + k] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//if (DlPos[tmpsum].size() == 0)
					//{
					//	cout << "字典不完备！！！" << endl;
					//	return;
					//}
					//else
					{
						
						for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
						{
							AndKBit = tmpKey & dicDetail[tmpknum].first;
							int tmpmatch = AndKBit.count();
							if (Kpos.second < tmpmatch)
							{
								Kpos.first = tmpknum;
								Kpos.second = tmpmatch;
							}
						}
						
						for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
						{
							int tmpmatch = 0;
							for (int mm = 25; mm < 61; mm++)
							{
								if (tmpValue[mm - 25] == dicDetail[Kpos.first].second[tmpvnum][mm])
								{
									tmpmatch++;
								}
							}
							if (Vpos.second < tmpmatch)
							{
								Vpos.first = tmpvnum;
								Vpos.second = tmpmatch;
							}
						}
					}
					
					index = 0;
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + 0][y + j][x + k] = 1;
							else
								HRImage3DBool[z + 0][y + j][x + k] = 0;

							index++;
						}
					}
					
					index += 4;
					
					index += 16;
					
					index += 16;
					
					for (int i = 1; i < templateSize; i++)
					{
						for (int j = 1; j < templateSize; j++)
						{
							for (int k = 1; k < templateSize; k++)
							{
								if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
									HRImage3DBool[z + i][y + j][x + k] = 1;
								else
									HRImage3DBool[z + i][y + j][x + k] = 0;

								index++;
							}
						}
					}
				}

			}
		}
	}

	//1.
	x = 0, y = 0;
	for (z = 4; z < PadZSize - 4; z += 4)
	{
		bitset<27> tmpKey(0);
		int index = 0;
		int tmpsum = 0;
		//Key
		for (int i = 0; i < templateSize; i += 2)
		{
			for (int j = 0; j < templateSize; j += 2)
			{
				for (int k = 0; k < templateSize; k += 2)
				{
					if (HRImage3DBool[z + i][y + j][x + k] == 1)
					{
						tmpKey.set(index, 1);
						tmpsum++;
					}
					index++;
				}
			}
		}
		//
		if (tmpsum == 0)
		{
			for (int i = 0; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						HRImage3DBool[z + i][y + j][x + k] = 0;
					}
				}
			}
		}
		else if (tmpsum == 27)
		{
			for (int i = 0; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						HRImage3DBool[z + i][y + j][x + k] = 1;
					}
				}
			}
		}
		else
		{
			//Value
			bitset<25> tmpValue(0);
			//bitset<25> AndVBit;//Value相与结果
			bitset<27> AndKBit;//Key相与结果
			pair<int, int>Kpos(0, 0);
			pair<int, int>Vpos(0, 0);
			index = 0;
			//
			for (int j = 0; j < templateSize; j++)
			{
				for (int k = 0; k < templateSize; k++)
				{
					if (HRImage3DBool[z + 0][y + j][x + k] == 1)
						tmpValue.set(index, 1);
					index++;
				}
			}
			//if (DlPos[tmpsum].size() == 0)
			//{
			//	cout << "字典不完备！！！" << endl;
			//	return;
			//}
			//else
			{
				//
				//for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
				for (int tmpknum = 0; tmpknum < dicDetail.size(); tmpknum++)
				{
					AndKBit = tmpKey & dicDetail[tmpknum].first;
					int tmpmatch = AndKBit.count();
					if (Kpos.second < tmpmatch)
					{
						Kpos.first = tmpknum;
						Kpos.second = tmpmatch;
					}
				}
				//
				for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
				{
					int tmpmatch = 0;
					for (int mm = 0; mm < 25; mm++)
					{
						if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
						{
							tmpmatch++;
						}
					}
					if (Vpos.second < tmpmatch)
					{
						Vpos.first = tmpvnum;
						Vpos.second = tmpmatch;
					}
				}
			}
			
			index = 25;
			
			for (int i = 1; i < templateSize; i++)
			{
				if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
					HRImage3DBool[z + i][y + 0][x + 0] = 1;
				else
					HRImage3DBool[z + i][y + 0][x + 0] = 0;

				index++;
			}
			
			for (int i = 1; i < templateSize; i++)
			{
				for (int j = 0; j < templateSize; j++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + i][y + j][x + 0] = 1;
					else
						HRImage3DBool[z + i][y + j][x + 0] = 0;

					index++;
				}
			}
			
			for (int i = 1; i < templateSize; i++)
			{
				for (int k = 1; k < templateSize; k++)
				{
					if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
						HRImage3DBool[z + i][y + 0][x + k] = 1;
					else
						HRImage3DBool[z + i][y + 0][x + k] = 0;

					index++;
				}
			}
			
			for (int i = 1; i < templateSize; i++)
			{
				for (int j = 1; j < templateSize; j++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + j][x + 0] = 1;
						else
							HRImage3DBool[z + i][y + j][x + 0] = 0;

						index++;
					}
				}
			}
		}

	}
	//2.
	x = 0;
	for (z = 4; z < PadZSize - 4; z += 4)
	{
		for (y = 4; y < PadYSize - 4; y += 4)
		{
			bitset<27> tmpKey(0);
			int index = 0;
			int tmpsum = 0;
			//Key
			for (int i = 0; i < templateSize; i += 2)
			{
				for (int j = 0; j < templateSize; j += 2)
				{
					for (int k = 0; k < templateSize; k += 2)
					{
						if (HRImage3DBool[z + i][y + j][x + k] == 1)
						{
							tmpKey.set(index, 1);
							tmpsum++;
						}
						index++;
					}
				}
			}
			if (tmpsum == 0)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 0;
						}
					}
				}
			}
			else if (tmpsum == 27)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 1;
						}
					}
				}
			}
			else
			{
				//Value
				bitset<45> tmpValue(0);
				//bitset<25> AndVBit;
				bitset<27> AndKBit;
				pair<int, int>Kpos(0, 0);
				pair<int, int>Vpos(0, 0);
				index = 0;
				//
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (HRImage3DBool[z + 0][y + j][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (HRImage3DBool[z + i][y + 0][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//if (DlPos[tmpsum].size() == 0)
				//{
				//	cout << "字典不完备！！！" << endl;
				//	return;
				//}
				//else
				{
					
					for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					{
						AndKBit = tmpKey & dicDetail[tmpknum].first;
						int tmpmatch = AndKBit.count();
						if (Kpos.second < tmpmatch)
						{
							Kpos.first = tmpknum;
							Kpos.second = tmpmatch;
						}
					}
					
					for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
					{
						int tmpmatch = 0;
						for (int mm = 0; mm < 25 + 4; mm++)
						{
							if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						for (int mm = 25 + 4 + 16; mm < 25 + 4 + 16 + 16; mm++)
						{
							if (tmpValue[mm - 16] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						if (Vpos.second < tmpmatch)
						{
							Vpos.first = tmpvnum;
							Vpos.second = tmpmatch;
						}
					}
				}
				
				index = 25;
				
				index += 4;
				
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + j][x + 0] = 1;
						else
							HRImage3DBool[z + i][y + j][x + 0] = 0;

						index++;
					}
				}
				
				index += 16;
				
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + i][y + j][x + k] = 1;
							else
								HRImage3DBool[z + i][y + j][x + k] = 0;

							index++;
						}
					}
				}
			}
		}


	}
	//3.
	y = 0;
	for (z = 4; z < PadZSize - 4; z += 4)
	{
		//if (z % 10 == 0)
		//{
		//	cout << "Reconstruction Layer z=" << z << endl;
		//}
		for (x = 4; x < PadXSize - 4; x += 4)
		{
			bitset<27> tmpKey(0);
			int index = 0;
			int tmpsum = 0;
			//Key
			for (int i = 0; i < templateSize; i += 2)
			{
				for (int j = 0; j < templateSize; j += 2)
				{
					for (int k = 0; k < templateSize; k += 2)
					{
						if (HRImage3DBool[z + i][y + j][x + k] == 1)
						{
							tmpKey.set(index, 1);
							tmpsum++;
						}
						index++;
					}
				}
			}
			if (tmpsum == 0)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 0;
						}
					}
				}
			}
			else if (tmpsum == 27)
			{
				for (int i = 0; i < templateSize; i++)
				{
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							HRImage3DBool[z + i][y + j][x + k] = 1;
						}
					}
				}
			}
			else
			{
				//Value
				bitset<45> tmpValue(0);
				//bitset<25> AndVBit;
				bitset<27> AndKBit;
				pair<int, int>Kpos(0, 0);
				pair<int, int>Vpos(0, 0);
				index = 0;
				//
				for (int j = 0; j < templateSize; j++)
				{
					for (int k = 0; k < templateSize; k++)
					{
						if (HRImage3DBool[z + 0][y + j][x + k] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
						tmpValue.set(index, 1);
					index++;
				}
				//
				for (int i = 1; i < templateSize; i++)
				{
					for (int j = 1; j < templateSize; j++)
					{
						if (HRImage3DBool[z + i][y + j][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
				}
				//if (DlPos[tmpsum].size() == 0)
				//{
				//	cout << "字典不完备！！！" << endl;
				//	return;
				//}
				//else
				{
					//
					for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
					{
						AndKBit = tmpKey & dicDetail[tmpknum].first;
						int tmpmatch = AndKBit.count();
						if (Kpos.second < tmpmatch)
						{
							Kpos.first = tmpknum;
							Kpos.second = tmpmatch;
						}
					}
					//
					for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
					{
						int tmpmatch = 0;
						for (int mm = 0; mm < 25 + 4 + 16; mm++)
						{
							if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
							{
								tmpmatch++;
							}
						}
						if (Vpos.second < tmpmatch)
						{
							Vpos.first = tmpvnum;
							Vpos.second = tmpmatch;
						}
					}
				}
				
				index = 25;
				
				index += 4;
				
				index += 16;
				
				for (int i = 1; i < templateSize; i++)
				{
					for (int k = 1; k < templateSize; k++)
					{
						if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
							HRImage3DBool[z + i][y + 0][x + k] = 1;
						else
							HRImage3DBool[z + i][y + 0][x + k] = 0;

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
							if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
								HRImage3DBool[z + i][y + j][x + k] = 1;
							else
								HRImage3DBool[z + i][y + j][x + k] = 0;

							index++;
						}
					}
				}
			}

		}
	}
	//4.
	for (z = 4; z < PadZSize - 4; z += 4)
	{
		if (z % 10 == 0)
		{
			cout << "Reconstruction Layer z=" << z << endl;
		}
		for (y = 4; y < PadYSize - 4; y += 4)
		{
			for (x = 4; x < PadXSize - 4; x += 4)
			{
				bitset<27> tmpKey(0);
				int index = 0;
				int tmpsum = 0;
				//Key
				for (int i = 0; i < templateSize; i += 2)
				{
					for (int j = 0; j < templateSize; j += 2)
					{
						for (int k = 0; k < templateSize; k += 2)
						{
							if (HRImage3DBool[z + i][y + j][x + k] == 1)
							{
								tmpKey.set(index, 1);
								tmpsum++;
							}
							index++;
						}
					}
				}
				if (tmpsum == 0)
				{
					for (int i = 0; i < templateSize; i++)
					{
						for (int j = 0; j < templateSize; j++)
						{
							for (int k = 0; k < templateSize; k++)
							{
								HRImage3DBool[z + i][y + j][x + k] = 0;
							}
						}
					}
				}
				else if (tmpsum == 27)
				{
					for (int i = 0; i < templateSize; i++)
					{
						for (int j = 0; j < templateSize; j++)
						{
							for (int k = 0; k < templateSize; k++)
							{
								HRImage3DBool[z + i][y + j][x + k] = 1;
							}
						}
					}
				}
				else
				{
					//Value
					bitset<61> tmpValue(0);
					//bitset<25> AndVBit;
					bitset<27> AndKBit;
					pair<int, int>Kpos(0, 0);
					pair<int, int>Vpos(0, 0);
					index = 0;
					//底层25
					for (int j = 0; j < templateSize; j++)
					{
						for (int k = 0; k < templateSize; k++)
						{
							if (HRImage3DBool[z + 0][y + j][x + k] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//
					for (int i = 1; i < templateSize; i++)
					{
						if (HRImage3DBool[z + i][y + 0][x + 0] == 1)
							tmpValue.set(index, 1);
						index++;
					}
					//
					for (int i = 1; i < templateSize; i++)
					{
						for (int j = 1; j < templateSize; j++)
						{
							if (HRImage3DBool[z + i][y + j][x + 0] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//
					for (int i = 1; i < templateSize; i++)
					{
						for (int k = 1; k < templateSize; k++)
						{
							if (HRImage3DBool[z + i][y + 0][x + k] == 1)
								tmpValue.set(index, 1);
							index++;
						}
					}
					//if (DlPos[tmpsum].size() == 0)
					//{
					//	cout << "字典不完备！！！" << endl;
					//	return;
					//}
					//else
					{
						
						for (int tmpknum = DlArrayLeft[tmpsum]; tmpknum < DlArrayRight[tmpsum]; tmpknum++)
						{
							AndKBit = tmpKey & dicDetail[tmpknum].first;
							int tmpmatch = AndKBit.count();
							if (Kpos.second < tmpmatch)
							{
								Kpos.first = tmpknum;
								Kpos.second = tmpmatch;
							}
						}
						
						for (int tmpvnum = 0; tmpvnum < dicDetail[Kpos.first].second.size(); tmpvnum++)
						{
							int tmpmatch = 0;
							for (int mm = 0; mm < 61; mm++)
							{
								if (tmpValue[mm] == dicDetail[Kpos.first].second[tmpvnum][mm])
								{
									tmpmatch++;
								}
							}
							if (Vpos.second < tmpmatch)
							{
								Vpos.first = tmpvnum;
								Vpos.second = tmpmatch;
							}
						}
					}
					
					index = 25;
					
					index += 4;
					
					index += 16;
					
					index += 16;
					
					for (int i = 1; i < templateSize; i++)
					{
						for (int j = 1; j < templateSize; j++)
						{
							for (int k = 1; k < templateSize; k++)
							{
								if (dicDetail[Kpos.first].second[Vpos.first][index] == 1)
									HRImage3DBool[z + i][y + j][x + k] = 1;
								else
									HRImage3DBool[z + i][y + j][x + k] = 0;

								index++;
							}
						}
					}
				}

			}
		}
	}

}

void DetailDic::get3DImageBool(vector<vector<vector<bool>>>& tmp3DImage)
{
	LRImage3DBool = tmp3DImage;
}

bool DetailDic::getLRImagePath(string tmpImagePath, string tmpImageName, string tmpImageExt)
{
	LRImagePath = tmpImagePath;
	LRImageName = tmpImageName;
	LRImageExt = tmpImageExt;
	return true;
}

bool DetailDic::imgLoad()
{
	Mat image2D;
	string tempString;
	do
	{
		stringstream temp;
		temp << setw(4) << setfill('0') << imgLoadStart;
		tempString = temp.str();
		string tempPath = LRImagePath + LRImageName + tempString + LRImageExt;
		image2D = cv::imread(tempPath, 0);

		if (!image2D.empty())
		{
			LRImage3D.push_back(image2D);
		}
		imgLoadStart++;

	} while (!image2D.empty());

	LRImg3DReadBool.resize(LRImage3D.size());
	for (int i = 0; i < LRImg3DReadBool.size(); i++)
	{
		LRImg3DReadBool[i].resize(LRImage3D[i].rows);
		for (int j = 0; j < LRImg3DReadBool[i].size(); j++)
		{
			LRImg3DReadBool[i][j].resize(LRImage3D[i].cols);
			for (int k = 0; k < LRImg3DReadBool[i][j].size(); k++)
			{
				LRImg3DReadBool[i][j][k] = 0;
			}
		}
	}
	for (int z = 0; z < LRImage3D.size(); z++)
	{
		for (int y = 0; y < LRImage3D[z].rows; y++)
		{
			for (int x = 0; x < LRImage3D[z].cols; x++)
			{
				if (LRImage3D[z].at<uchar>(y, x) != 0)
					LRImg3DReadBool[z][y][x] = 1;
			}
		}
	}

	return true;
}

bool DetailDic::getHRImagePath(string tmpImagePath, string tmpImageName, string tmpImageExt)
{
	HRImagePath = tmpImagePath;
	HRImageName = tmpImageName;
	HRImageExt = tmpImageExt;
	return true;
}

bool DetailDic::imgLoadHR2D()
{
	Mat image2D;
	string tempPath = HRImagePath + HRImageName + HRImageExt;
	image2D = cv::imread(tempPath, 0);
	HR2D.resize(image2D.rows);
	for (int i = 0; i < image2D.rows; i++)
	{
		HR2D[i].resize(image2D.cols);
	}
	for (int i = 0; i < image2D.rows; i++)
	{
		for (int j = 0; j < image2D.cols; j++)
		{
			if (image2D.at<uchar>(i, j) != 0)
				HR2D[i][j] = 1;
			else
				HR2D[i][j] = 0;
		}
	}
	return true;
}

bool DetailDic::DicSPoreCreate()
{
	srand((unsigned)time(0));//Initialize the random seed
	fstream dicSmallPoreFile;
	dicSmallPoreFile.open("DictionaryLib\\SmallPoreLib.txt", ios_base::in);
	if (!dicSmallPoreFile)
	{
		cout << "Please build micro-pores dictionary!!!" << endl;
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
		SPDistribution.resize(tmpSize);
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

	return true;
}

bool DetailDic::AddSmallPore(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<uchar>>>& dstImage3D, int ObjGray,double AddRate)
{
	int layer = srcImage3D.size();
	int height = srcImage3D[0].size();
	int width = srcImage3D[0][0].size();

	
	dstImage3D.resize(srcImage3D.size());
	for (int i = 0; i != srcImage3D.size(); ++i)
	{
		dstImage3D[i].resize(srcImage3D[0].size());
		for (int j = 0; j < srcImage3D[0].size(); j++)
		{
			dstImage3D[i][j].resize(srcImage3D[0][0].size());
			for (int k = 0; k < srcImage3D[0][0].size(); k++)
			{
				dstImage3D[i][j][k] = 0;
			}
		}
	}

	
	for (int i = 0; i < srcImage3D.size() - 4; i += 4)
	{
		for (int j = 0; j < srcImage3D[0].size() - 4; j += 4)
		{
			for (int kk = 0; kk < srcImage3D[0][0].size() - 4; kk += 4)
			{
				int tmpSum = 0;
				for (int z = 0; z < 5; z += 2)
				{
					for (int y = 0; y < 5; y += 2)
					{
						for (int x = 0; x < 5; x += 2)
						{
							tmpSum += srcImage3D[i + z][j + y][kk + x];
						}
					}
				}
				if (tmpSum != 0)
				{
					for (int z = 0; z < 5; z++)
					{
						for (int y = 0; y < 5; y++)
						{
							for (int x = 0; x < 5; x++)
							{
								dstImage3D[i + z][j + y][kk + x] = 255;
							}
						}
					}
				}
			}
		}
	}

	if (SPDistribution.size() == 0)
	{
		cout << "Micro-pores dictionary read fail！！！" << endl;
		return false;
	}

	for (int i = 0; i < SPDistribution.size(); i++)
	{
		
		if (SPDistribution[i].size() > 0)
		{
			
			if (AddRate > 1)
			{
				
				for (int tmpNum = 0; tmpNum < SPDistribution[i].size(); tmpNum++)
				{
					bool AddYorN = 1;
					
					int tmpzL = ((SmallPoreDic[tmpNum].second.second.x - SmallPoreDic[tmpNum].second.first.x) - (SmallPoreDic[tmpNum].second.second.x - SmallPoreDic[tmpNum].second.first.x) % 4) / 4;
					int tmpyL = ((SmallPoreDic[tmpNum].second.second.y - SmallPoreDic[tmpNum].second.first.y) - (SmallPoreDic[tmpNum].second.second.y - SmallPoreDic[tmpNum].second.first.y) % 4) / 4;
					int tmpxL = ((SmallPoreDic[tmpNum].second.second.z - SmallPoreDic[tmpNum].second.first.z) - (SmallPoreDic[tmpNum].second.second.z - SmallPoreDic[tmpNum].second.first.z) % 4) / 4;
					
					int a = 0, b = 0, c = 0; int tmpTHNum = 0;
					while (!(a + b + c))
					{
						tmpTHNum++;
						if (tmpTHNum > 10000)
						{
							cout << "Micro-pores padding error！！！" << endl;
							AddYorN = 0;
							break;
						}
						int tmpxPos = rand() % (width / 4 - 1);
						int tmpyPos = rand() % (height / 4 - 1);
						int tmpzPos = rand() % (layer / 4 - 1);

						int tmpSumsum = 0;
						if ((tmpzPos + tmpzL + 1) * 4 > layer || (tmpyPos + tmpyL + 1) * 4 > height || (tmpxPos + tmpxL + 1) * 4 > width)
						{
							
							continue;
						}
						for (int ii = 0; ii < tmpzL + 1; ii++)
						{
							for (int jj = 0; jj < tmpyL + 1; jj++)
							{
								for (int kk = 0; kk < tmpxL + 1; kk++)
								{
									tmpSumsum += dstImage3D[4 * (tmpzPos + ii)][4 * (tmpyPos + jj)][4 * (tmpxPos + kk)];
								}
							}
						}
						if (tmpSumsum == 0)
						{
							a = tmpxPos, b = tmpyPos, c = tmpzPos;
						}
					}
					if (AddYorN)
					{
						
						for (int k = 0; k < SmallPoreDic[tmpNum].first.size(); k++)
						{
							//（c+SmallPoreDic[tmpRandNum].first[k].x-SmallPoreDic[tmpRandNum].second.first.x）
							int AddPixel_z = 4 * c + SmallPoreDic[tmpNum].first[k].x - SmallPoreDic[tmpNum].second.first.x;
							int AddPixel_y = 4 * b + SmallPoreDic[tmpNum].first[k].y - SmallPoreDic[tmpNum].second.first.y;
							int AddPixel_x = 4 * a + SmallPoreDic[tmpNum].first[k].z - SmallPoreDic[tmpNum].second.first.z;
							srcImage3D[AddPixel_z][AddPixel_y][AddPixel_x] = 1;
						}
						
						for (int ii = 4 * c; ii < 4 * (c + tmpzL + 1); ii++)
						{
							for (int jj = 4 * b; jj < 4 * (b + tmpyL + 1); jj++)
							{
								for (int kk = 4 * a; kk < 4 * (a + tmpxL + 1); kk++)
								{
									dstImage3D[ii][jj][kk] = 128;
								}
							}
						}
					}
				}
				
				for (int tmpRandNum = 0; tmpRandNum < SPDistribution[i].size() * (AddRate - 1); tmpRandNum++)
				{
					
					int tmpNum = rand() % SPDistribution[i].size();
					bool AddYorN = 1;
					
					int tmpzL = ((SmallPoreDic[tmpNum].second.second.x - SmallPoreDic[tmpNum].second.first.x) - (SmallPoreDic[tmpNum].second.second.x - SmallPoreDic[tmpNum].second.first.x) % 4) / 4;
					int tmpyL = ((SmallPoreDic[tmpNum].second.second.y - SmallPoreDic[tmpNum].second.first.y) - (SmallPoreDic[tmpNum].second.second.y - SmallPoreDic[tmpNum].second.first.y) % 4) / 4;
					int tmpxL = ((SmallPoreDic[tmpNum].second.second.z - SmallPoreDic[tmpNum].second.first.z) - (SmallPoreDic[tmpNum].second.second.z - SmallPoreDic[tmpNum].second.first.z) % 4) / 4;
					
					int a = 0, b = 0, c = 0; int tmpTHNum = 0;
					while (!(a + b + c))
					{
						tmpTHNum++;
						if (tmpTHNum > 10000)
						{
							cout << "Micro-pores padding error！！！" << endl;
							AddYorN = 0;
							break;
						}
						int tmpxPos = rand() % (width / 4 - 1);
						int tmpyPos = rand() % (height / 4 - 1);
						int tmpzPos = rand() % (layer / 4 - 1);

						int tmpSumsum = 0;
						if ((tmpzPos + tmpzL + 1) * 4 > layer || (tmpyPos + tmpyL + 1) * 4 > height || (tmpxPos + tmpxL + 1) * 4 > width)
						{
							
							continue;
						}
						for (int ii = 0; ii < tmpzL + 1; ii++)
						{
							for (int jj = 0; jj < tmpyL + 1; jj++)
							{
								for (int kk = 0; kk < tmpxL + 1; kk++)
								{
									tmpSumsum += dstImage3D[4 * (tmpzPos + ii)][4 * (tmpyPos + jj)][4 * (tmpxPos + kk)];
								}
							}
						}
						if (tmpSumsum == 0)
						{
							a = tmpxPos, b = tmpyPos, c = tmpzPos;
						}
					}
					if (AddYorN)
					{
						
						for (int k = 0; k < SmallPoreDic[tmpNum].first.size(); k++)
						{
							//（c+SmallPoreDic[tmpRandNum].first[k].x-SmallPoreDic[tmpRandNum].second.first.x）
							int AddPixel_z = 4 * c + SmallPoreDic[tmpNum].first[k].x - SmallPoreDic[tmpNum].second.first.x;
							int AddPixel_y = 4 * b + SmallPoreDic[tmpNum].first[k].y - SmallPoreDic[tmpNum].second.first.y;
							int AddPixel_x = 4 * a + SmallPoreDic[tmpNum].first[k].z - SmallPoreDic[tmpNum].second.first.z;
							srcImage3D[AddPixel_z][AddPixel_y][AddPixel_x] = 1;
						}
						
						for (int ii = 4 * c; ii < 4 * (c + tmpzL + 1); ii++)
						{
							for (int jj = 4 * b; jj < 4 * (b + tmpyL + 1); jj++)
							{
								for (int kk = 4 * a; kk < 4 * (a + tmpxL + 1); kk++)
								{
									dstImage3D[ii][jj][kk] = 128;
								}
							}
						}
					}
				}
			}
			else
			{
			//
			for (int tmpRandNum = 0; tmpRandNum < SPDistribution[i].size() * AddRate; tmpRandNum++)
			{
				//
				int tmpNum = rand() % SPDistribution[i].size();
				bool AddYorN = 1;
				//
				int tmpzL = ((SmallPoreDic[tmpNum].second.second.x - SmallPoreDic[tmpNum].second.first.x) - (SmallPoreDic[tmpNum].second.second.x - SmallPoreDic[tmpNum].second.first.x) % 4) / 4;
				int tmpyL = ((SmallPoreDic[tmpNum].second.second.y - SmallPoreDic[tmpNum].second.first.y) - (SmallPoreDic[tmpNum].second.second.y - SmallPoreDic[tmpNum].second.first.y) % 4) / 4;
				int tmpxL = ((SmallPoreDic[tmpNum].second.second.z - SmallPoreDic[tmpNum].second.first.z) - (SmallPoreDic[tmpNum].second.second.z - SmallPoreDic[tmpNum].second.first.z) % 4) / 4;
				//
				int a = 0, b = 0, c = 0; int tmpTHNum = 0;
				while (!(a + b + c))
				{
					tmpTHNum++;
					if (tmpTHNum > 10000)
					{
						cout << "Micro-pores padding error！！！" << endl;
						AddYorN = 0;
						break;
					}
					int tmpxPos = rand() % (width / 4 - 1);
					int tmpyPos = rand() % (height / 4 - 1);
					int tmpzPos = rand() % (layer / 4 - 1);

					int tmpSumsum = 0;
					if ((tmpzPos + tmpzL + 1) * 4 > layer || (tmpyPos + tmpyL + 1) * 4 > height || (tmpxPos + tmpxL + 1) * 4 > width)
					{
						//
						continue;
					}
					for (int ii = 0; ii < tmpzL + 1; ii++)
					{
						for (int jj = 0; jj < tmpyL + 1; jj++)
						{
							for (int kk = 0; kk < tmpxL + 1; kk++)
							{
								tmpSumsum += dstImage3D[4 * (tmpzPos + ii)][4 * (tmpyPos + jj)][4 * (tmpxPos + kk)];
							}
						}
					}
					if (tmpSumsum == 0)
					{
						a = tmpxPos, b = tmpyPos, c = tmpzPos;
					}
				}
				if (AddYorN)
				{
					//
					for (int k = 0; k < SmallPoreDic[tmpNum].first.size(); k++)
					{
						//（c+SmallPoreDic[tmpRandNum].first[k].x-SmallPoreDic[tmpRandNum].second.first.x）
						int AddPixel_z = 4 * c + SmallPoreDic[tmpNum].first[k].x - SmallPoreDic[tmpNum].second.first.x;
						int AddPixel_y = 4 * b + SmallPoreDic[tmpNum].first[k].y - SmallPoreDic[tmpNum].second.first.y;
						int AddPixel_x = 4 * a + SmallPoreDic[tmpNum].first[k].z - SmallPoreDic[tmpNum].second.first.z;
						srcImage3D[AddPixel_z][AddPixel_y][AddPixel_x] = 1;
					}
					
					for (int ii = 4 * c; ii < 4 * (c + tmpzL + 1); ii++)
					{
						for (int jj = 4 * b; jj < 4 * (b + tmpyL + 1); jj++)
						{
							for (int kk = 4 * a; kk < 4 * (a + tmpxL + 1); kk++)
							{
								dstImage3D[ii][jj][kk] = 128;
							}
						}
					}
				}
			}
            }
		}

	}

	for (int i = 0; i != dstImage3D.size(); ++i)
	{
		string ImagePath = "DIST\\"; //The path of MASK
		string ImageExt = ".bmp"; 
		Mat image = Mat::zeros(height, width, CV_8UC1);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				image.at<uchar>(y, x) = dstImage3D[i][y][x];
			}
		}
		//
		stringstream templayer;
		templayer << setw(4) << setfill('0') << i;
		string tempString2 = templayer.str();
		string tempPath = ImagePath + "Image" + tempString2 + ImageExt;
		imwrite(tempPath, image);
	}

	return true;
}

bool DetailDic::MarkConnetedRegion_3D(vector<vector<vector<bool>>>& srcImage3D, vector<vector<vector<uchar>>>& dstImage3D, int ObjGray)
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
				/*uchar &temp1 = Notaccessed.at<uchar>(i, j);*/
				int temp1 = Notaccessed[k][i][j];
				if (temp1 && srcImage3D[k][i][j] == ObjGray) //
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
	cout << "共" << ALL_Regions.size() << "个连通区域" << endl;

	//
	dstImage3D.resize(srcImage3D.size());
	for (int i = 0; i != srcImage3D.size(); ++i)
	{
		dstImage3D[i].resize(srcImage3D[0].size());
		for (int j = 0; j < srcImage3D[0].size(); j++)
		{
			dstImage3D[i][j].resize(srcImage3D[0][0].size());
			for (int k = 0; k < srcImage3D[0][0].size(); k++)
			{
				dstImage3D[i][j][k] = 0;
			}
		}
	}

	//
	for (int i = 0; i < srcImage3D.size() - 4; i += 4)
	{
		for (int j = 0; j < srcImage3D[0].size() - 4; j += 4)
		{
			for (int kk = 0; kk < srcImage3D[0][0].size() - 4; kk += 4)
			{
				int tmpSum = 0;
				for (int z = 0; z < 5; z+=2)
				{
					for (int y = 0; y < 5; y+=2)
					{
						for (int x = 0; x < 5; x+=2)
						{
							tmpSum += srcImage3D[i + z][j + y][kk + x];
						}
					}
				}
				if (tmpSum != 0)
				{
					for (int z = 0; z < 5; z++)
					{
						for (int y = 0; y < 5; y++)
						{
							for (int x = 0; x < 5; x++)
							{
								dstImage3D[i + z][j + y][kk + x]=255;
							}
						}
					}
				}
			}
		}
	}


	int tmpThresholdNum = 126;
	if (SPDistribution.size() == 0)
	{
		cout << "小孔信息未读取！！！" << endl;
		return false;
	}
	else
	{
		LR3D_SPDist.resize(SPDistribution.size());
	}
	for (int k = 0; k != ALL_Regions.size(); k++)
	{
		cout << "第" << k + 1 << "个连通区域的像素点数为" << ALL_Regions[k].size() << endl;
		if (ALL_Regions[k].size() < tmpThresholdNum)
		{
			
			int tmpLp;
			for (tmpLp = 0; tmpLp * tmpLp * tmpLp <= ALL_Regions[k].size(); tmpLp++);
			LR3D_SPDist[tmpLp - 1].push_back(k);
		}
	}

	//
	for (int i = 0; i < SPDistribution.size(); i++)
	{
		cout << SPDistribution[i].size() << " " << LR3D_SPDist[i].size() << endl;
	}

	//
	for (int i = 0; i < SPDistribution.size(); i++)
	{
		//
		if (SPDistribution[i].size() > LR3D_SPDist[i].size())
		{
			//
			int RepeatNum = SPDistribution[i].size() - LR3D_SPDist[i].size();

			for (int j = 0; j < RepeatNum; j++)
			{
				//
				int tmpRandNum = rand() % SPDistribution[i].size();
				//
				int tmpzL = (SmallPoreDic[tmpRandNum].second.second.x - SmallPoreDic[tmpRandNum].second.first.x)/4;
				int tmpyL = (SmallPoreDic[tmpRandNum].second.second.y - SmallPoreDic[tmpRandNum].second.first.y)/4;
				int tmpxL = (SmallPoreDic[tmpRandNum].second.second.z - SmallPoreDic[tmpRandNum].second.first.z)/4;
				//
				int a = 0, b = 0, c = 0; int tmpTHNum = 0;
				while (!(a + b + c))
				{
					tmpTHNum++;
					if (tmpTHNum > 10000)
					{
						cout << "添加小孔失败！！！" << endl;
						return false;
					}
					int tmpxPos = rand() % (width / 4 - 1);
					int tmpyPos = rand() % (height / 4 - 1);
					int tmpzPos = rand() % (layer / 4 - 1);

					int tmpSumsum = 0;
					if ((tmpzPos + tmpzL + 1) * 4 > layer || (tmpyPos + tmpyL + 1) * 4 > height || (tmpxPos + tmpxL + 1) * 4 > width)
					{
						//
						continue;
					}
					for (int ii = 0; ii < a+tmpzL + 1; ii++)
					{
						for (int jj = 0; jj < tmpyL + 1; jj++)
						{
							for (int kk = 0; kk < tmpxL + 1; kk++)
							{
								tmpSumsum += dstImage3D[4 * (tmpzPos + ii)][4 * (tmpyPos + jj)][4 * (tmpxPos + kk)];
							}
						}
					}
					if (tmpSumsum == 0)
					{
						a = tmpxPos, b = tmpyPos, c = tmpzPos;
					}
				}
				//
				for (int k = 0; k < SmallPoreDic[tmpRandNum].first.size(); k++)
				{
					//（c+SmallPoreDic[tmpRandNum].first[k].x-SmallPoreDic[tmpRandNum].second.first.x）
					int AddPixel_z = 4 * c + SmallPoreDic[tmpRandNum].first[k].x - SmallPoreDic[tmpRandNum].second.first.x;
					int AddPixel_y = 4 * b + SmallPoreDic[tmpRandNum].first[k].y - SmallPoreDic[tmpRandNum].second.first.y;
					int AddPixel_x = 4 * a + SmallPoreDic[tmpRandNum].first[k].z - SmallPoreDic[tmpRandNum].second.first.z;
					srcImage3D[AddPixel_z][AddPixel_y][AddPixel_x] = 1;
					//dstImage3D[AddPixel_z][AddPixel_y][AddPixel_x] = 128;
				}
				//
				for (int ii = 4 * c; ii < 4 * (c + tmpzL + 1); ii++)
				{
					for (int jj = 4 * b; jj < 4 * (b + tmpyL + 1); jj++)
					{
						for (int kk = 4 * a; kk < 4 * (a + tmpxL + 1); kk++)
						{
							dstImage3D[ii][jj][kk] = 128;
						}
					}
				}

			}

		}
	}

	for (int i = 0; i != dstImage3D.size(); ++i)
	{
		string ImagePath = "J:\\新实验\\融合重建\\MySuperDim_字典文件读入重建\\MySuperDim\\DIST\\"; //重建图像的路径
		string ImageExt = ".bmp"; //

		Mat image = Mat::zeros(height, width, CV_8UC1);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
					image.at<uchar>(y, x) = dstImage3D[i][y][x];
			}
		}
		//
		stringstream templayer;
		templayer << setw(4) << setfill('0') << i;
		string tempString2 = templayer.str();

		string tempPath = ImagePath + "Image" + tempString2 + ImageExt;
		imwrite(tempPath, image);
	}

	return true;
}

