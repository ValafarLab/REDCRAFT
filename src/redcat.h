#ifndef REDCAT_H
#define REDCAT_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include "matrix.h"
#include "polypeptide.h"
#include "tensor.h"

#define SMALL_NUMBER 0.000001

class Redcat {

public:
	static void printVersion(){ cout << "Redcat.....v1.0\n"; }

	Redcat(string dataPath, string rdcFilePrefix, int media);
        Redcat(string dataPath, string rdcFilePrefix, int media,string fileFormat);

	~Redcat();
        
        static string svn_id();

	double calculateRMSD(Polypeptide &pp);
	double calculateRMSD(Polypeptide &pp, int medium);
	double calculateRMSD(Polypeptide &pp, int medium, Matrix * orderTensor);

	void setOTE(int m, const Matrix & ot);

	void createRedcatFile(Polypeptide &pp, const char *filename, int medium);
	int getReducedRDCCount(int medium, int residue);
	void getRDCDiff(Polypeptide &p, int medium, double *x, int &start);
	int getMedia() { return mMediaCount; }
       // void fillAtomsVector(vector<int> &a,vector<int> &b);

	int offset() { return mOffset; }
	void offset(int r) { mOffset = r; }
	double OTTolerance() { return mOTTolerance; }
	void OTTolerance(double t) { mOTTolerance = t; }
	double OTweight() { return mOTEweight; }
	void OTweight(double t) { mOTEweight = t; }
	void useOTE(bool q) { museOTE = q; }
        int map(string);
         int  mapMaxValue(int a,int b);
         string getFirstComment(int a);
         string getSecondComment(int a);
	
	void setDMax(double newDMax){ dMax = newDMax;}
	void populateWorkMatricies(int medium, Matrix * a, Matrix* unreducedA, Matrix * reducedRDCs, Matrix * reducedErrors);
	Matrix * calculateBCRDCFromReduced(Matrix * a, Matrix * reducedRDCs, Matrix * orderTensor);
	Matrix * calculateBCRDCFromOT(Matrix * a, Matrix * orderTensor);
	double calculateRMSD(Matrix * reducedRDCs, Matrix * backCompRDCs, Matrix * reducedErrors);
        vector< vector < vector <int> > >& getData(){return data;}//order: res1 atom1 res2 atom2
        vector<int>&   gettotalDataCount(){return totalDataCount;}//total number of rdc lines with data
        vector< vector<int> >&  getdataCount(){return dataCount;} //number of rdc data per residue --index starting at 0
        vector< vector<int> >&  getshifts(){return shifts;}


private:
	Matrix **mUnreducedRDCs;
	Matrix **mErrors;
	//Matrix **mReducedRDCs;
	int **mResidueIndices;
	int mMediaCount;
	int mOffset;
        vector< vector < vector <int> > > data;//order: res1 atom1 res2 atom2
        vector<int> totalDataCount;//total number of rdc lines with data
        vector< vector<int> > dataCount; //number of rdc data per residue --index starting at 1
        vector< vector<int> > shifts;
	double mOTTolerance;
	double mOTEweight;
	vector<Tensor*> mOTE;
	double *mOTED;
	bool museOTE;
	double dMax;
        string format;
};
#endif //REDCAT_H
