#ifndef REDCRAFT_H
#define REDCRAFT_H

#include "polypeptide.h"
#include "redcat.h"
#include "tstorage.h"
#include "cluster_collection.h"
#include "diagnostics.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <queue>
#include <cmath>

//Used for getting environment variables from Windows or Linux
#ifdef WIN32
    #include <Windows.h>
    #include <tchar.h>
#else
    #include <unistd.h>
#endif

class Redcraft{
public:
	Redcraft(string confFile = "./default.conf");
	~Redcraft();

	void run();

	static string svn_id();

	Matrix evaluatePolypeptide(Polypeptide &pp);
	void generateStructure(Polypeptide& pp, string name, ostream &out);

	void set_continue(int from) { mRunType = 1; mCurrentResidue = from; }
	RunState get_run_state();
	void print_state();
	void create_output_files();
private:
	static const int kNewRun = 0;

	void readNextAngles();
	void createStructures();
	//void arrangeStructures();

	int mStartResidue;
	int mStopResidue;
	int mCurrentResidue;
        int mMaxStructures = -1;
	int mMediaCount;

	double mClusterGranularity;

	string mDataPath;
	string mConfigFile;
	string mRDCFilePrefix;

	int mDefaultDepth;
	int mCurrentDepth;
	int mRunType;

	Redcat *mRedcat;
	char mOTEuse;

	vector<bool> mDecimationMask;
	vector<bool> mOTEstimationMask;

	bool mDecimationEnabled;
	double mDecimationScoreThreshold;

	bool mLJEnabled;
	double mLJScoreThreshold;
        
        bool mSequence;
        string mSeqString;

	vector< vector<double> > mNextAngles;
	TStorage<double,Polypeptide*> *mAngles;
	TStorage<double,Polypeptide*> *mHolding;
	TStorage<double,Polypeptide*> *mAnglesBuffer;
	ClusterCollection *mCluster;

	vector<string> mResidueNames;
        /*vector<int> mAtomNames1;
        vector<int> mAtomNames2;*/
        string mFormat;
	string getResidueNameAt(int);//possibly change logic to auto detect format based of line length
	void fillResidueNames();
        /*void fillResidueNames(int);
        int map (string);*/
	
	double minCaX, minCaY, minCaZ, maxCaX, maxCaY, maxCaZ;
	double mClusterCount;
	bool userEnteredClusterCount;
        
        int **mSearchDepth;
        int mSearchDepthSize;
};
#endif
