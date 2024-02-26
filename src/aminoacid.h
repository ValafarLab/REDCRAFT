#ifndef AMINOACID_H
#define AMINOACID_H
#include <string>
#include "atom.h"
#include "rotation.h"
#include "tensor.h"

#define AA_SMALL_NUMBER 0.000001

class AminoAcid {

public:
	enum AAAtomType {N, H, Ca, Ha2, Ha, C, O, N2, H2, Ca2, AAAtomCount};

	AminoAcid (double _phi=180, double _psi=180, string type = "ALA", double _omega=180);
	AminoAcid(const AminoAcid& old_aa);
	AminoAcid(const AminoAcid* old_aa);
        ~AminoAcid();

        static string svn_id();

	int getAtomCount() { return AAAtomCount; }
	Atom& operator[](const int type);
        Atom& getAtom(const int type);

	double mass();
	Atom center_of_mass();

	void setAngles(double phi, double psi, double omega);
        void setOmegaPublic(double omega);
        
	float getPhi(){ return phi; }
	float getPsi(){ return psi; }
	float getOmega(){ return omega; }

	void updatePosition (const AminoAcid& previous_aa);
	void updatePrevious (const AminoAcid& previous_aa);

	void translate (double x, double y, double z);
	void rotate (Rotation r);

	void fillAMatrix(Matrix &matrix, AminoAcid &prev, int offset);
        void fillAMatrix2(Matrix &matrix, AminoAcid &prev, int start);
	void calculateAMatrix(AminoAcid &prev);
        void calculateAMatrix(AminoAcid &prev, int* type1, int* type2, int* shift1,int rdcEntries);


	string getType(){return mType;}
	void setType(string type){
        mType = type;
        /*if(type=="CPR ")
        setAngles(phi,psi,0);*/
        }

        Tensor * getCSA(int n) { return CSA[n]; }
        double getCSAdd(int n, Tensor *);
        void setVectors(vector<int> a,vector<int> b,vector<int> &c);
        void Print();
        int mapMaxValue(int a, int b);
private:
	void setPhi (double _phi);
	void setPsi (double _psi);
	void setOmega (double _omega);
        
	float phi;
	float psi;
	float omega;
	string mType;
	Matrix mAMatrix;
	bool mAMatrixCalcd;
        /*vector<int> atomslist1;
        vector <int> atomslist2;
        vector<int> atomNumber;*/
	Atom atoms[AAAtomCount];

        Tensor * CSA[3];
};
#endif //AMINOACID_H

