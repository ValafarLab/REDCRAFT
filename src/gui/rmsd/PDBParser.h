/***************************************************************************
 *   Copyright (C) 2013 by Casey Cole and Earron Twitty                    *
 *                                                                         *
 *   This header file maintains a pdb parser for use in various            *
 *   parts of our project.                                                 *
 *                                                                         *
 ***************************************************************************/

#include <fstream>
#include <vector>
#include <string>
#include "Matrix.h"

using namespace std;

class PDBParser
{
    
private:
	string** store;
	string fileName;
	string Atoms;
    FloatArray x, y, z, w, v;
    int size, option,resInt,realSize,Start;
    void trim(string &s);
    
public:
    //constuctor
    PDBParser(ifstream& inFile, int, int, int, string);
    
    int getSize();
    
	void readFile(ifstream& inFile, int, int, string);
    
    vector<float> calcCentroid();
    bool fillMatrix(Matrix& chuckNorris, Matrix&);
    void translate();
    void PDBFill(string, string, string, string, string);
	void formatWrite(Matrix, string, Matrix,int, int);
	int stringSize(string );
	void addFileName(string);
	int getResInt();
	int getRealSize();
    int bypass(int);
	void setRealSize(int);
	string getAtom(int);
	string getChain(int);
    void checkRes(Matrix, string, Matrix, int, int);
    int getStart();
};



