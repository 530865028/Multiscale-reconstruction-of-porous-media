
#include "Dictionary.h"

Dictionary::Dictionary(void)
{
}

Dictionary::~Dictionary(void)
{
}

void Dictionary::dictionaryLoad(string tmpMarkovPadPath)
{
	string MarkovPadDicPath = tmpMarkovPadPath;

	fstream dicMarkovPadFile;
	dicMarkovPadFile.open(MarkovPadDicPath, ios_base::in);
	if (!dicMarkovPadFile)
	{
		cout << "MarkovÌî³ä×Öµä¶ÁÈëÊ§°Ü£¡" << endl;
	}
	else
	{
		int tmpSize;
		dicMarkovPadFile >> tmpSize;
		//
		dicPos.resize(tmpSize + 1);
		MZCArrayLeft.resize(tmpSize + 1);
		MZCArrayRight.resize(tmpSize + 1);

		for (int i = 0; i < tmpSize + 1; i++)
		{
			//cout << i << " ";
			int tmphistSize;
			dicMarkovPadFile >> tmphistSize;
			if (tmphistSize != 0)
			{
				for (int cnt = 0; cnt < tmphistSize; cnt++)
				{
					int tmpValue;
					dicMarkovPadFile >> tmpValue;
					dicPos[i].push_back(tmpValue);
					//cout << dicPos[i][cnt] << " ";
				}
			}
			//cout << endl;
		}

		for (int i = 0; i < tmpSize + 1; i++)
		{
			int tmpValueLeft;
			int tmpValueRight;
			dicMarkovPadFile >> tmpValueLeft;
			dicMarkovPadFile >> tmpValueRight;
			MZCArrayLeft[i] = tmpValueLeft;
			MZCArrayRight[i] = tmpValueRight;
		}

		//
		while (!dicMarkovPadFile.eof())
		{
			char ch;
			bitset<375> bufKey;
			bitset<125> tmpValue;
			vector<bitset<125>> bufValue;
			ch = dicMarkovPadFile.peek();
			if (ch == EOF)
				break;
			dicMarkovPadFile >> bufKey;
			do
			{
				dicMarkovPadFile >> tmpValue;
				bufValue.push_back(tmpValue);

				ch = dicMarkovPadFile.get(); //
				ch = dicMarkovPadFile.peek(); //
			} while (ch != '\n');

			dicMarkovPad.push_back(pair<bitset<375>, vector<bitset<125>>>(bufKey, bufValue));
			ch = dicMarkovPadFile.get();//
		}
	    dicMarkovPadFile.close();
	}
}

void Dictionary::DldicLoad(string tmpDetailDicPath)
{
	string DetailDicPath = tmpDetailDicPath;

	fstream dicDlFile;
	dicDlFile.open(DetailDicPath, ios_base::in);

	if (!dicDlFile)
	{
		cout << "Edge dictionary read fail!!£¡" << endl;
	}
	else
	{
		//
		int tmpSize;
		dicDlFile >> tmpSize;
		DlPos.resize(tmpSize + 1);
		DlArrayLeft.resize(tmpSize + 1);
		DlArrayRight.resize(tmpSize + 1);

		//
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
			DlArrayLeft[i] = tmpValueLeft;
			DlArrayRight[i] = tmpValueRight;

		}

		//
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

}
