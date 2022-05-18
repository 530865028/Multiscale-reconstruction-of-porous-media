
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <bitset>
#include <math.h>
//#include <string>
using namespace cv;
using namespace std;

//#define IntervalSampling 1 
//#define NonIntervalSampling 0

class Dictionary
{
public:
	Dictionary(void);
	~Dictionary(void);
public:
	int templateSize = 5;
	string MarkovPadDicPath;

	vector<pair<bitset<375>, vector<bitset<125>>>> dicMarkovPad;
	vector<int>MZCArrayLeft;//MZC
	vector<int>MZCArrayRight;//MZC
	vector<vector<int>>dicPos;//MZC

	vector<pair<bitset<27>, vector<bitset<125>>>> dicDetail;//MZC
	vector<vector<int>>DlPos;//MZC
	vector<int>DlArrayLeft;//MZC
	vector<int>DlArrayRight;//MZC

	void dictionaryLoad(string tmpMarkovPadPath);
	void DldicLoad(string tmpDetailDicPath);
};