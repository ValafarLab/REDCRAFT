#ifndef POLYPEPTIDE_H
#define POLYPEPTIDE_H
#include <string>
#include <fstream>
#include <vector>
#include "aminoacid.h"

class Polypeptide {
public:
    int id;
    void init();
    Polypeptide();
    Polypeptide(double *angles, int count);
    Polypeptide(double *phis, double *psis, int count);
    Polypeptide(const Polypeptide &copy);

    ~Polypeptide();

    static string svn_id();

    int add_remark(string r) {
        remarks.push_back(r);
        return remarks.size();
    }

    double mass();
    Atom center_of_mass();
    double bbrmsd(const Polypeptide &compare);


    void setAngles(int residue, double phi = 180, double psi = 180, double omega = 180);
    void setAngles(double *angles, int count);
    void setAngles(double *phis, double *psis, int count);

    void rotate(Rotation r);
    void translate(double x, double y, double z);

    AminoAcid * getAminoAcid(int index);

    void appendAminoAcid(double phi = 180, double psi = 180, string name = "ALA", double omega = 180);
    void appendAminoAcid2(double phi = 180, double psi = 180, string name = "ALA", double omega = 180);
    void prependAminoAcid(double phi = 180, double psi = 180, string name = "ALA", double omega = 180);

    int getResidueCount();
    Matrix getCoordinates();
    Matrix getCoordinates(vector< vector < vector <int> > > &data, vector<vector<int> > &dataCount, vector<int> &totalDataCount, vector< vector<int> > &shifts, int medium);
    //int shift(int,int);
    Matrix getPDBCoordinates();
    Matrix getBBCoordinates();
    Matrix getAMatrix();
    void writePDB(string filename, int start_residue = 1);
    void print();
    void Print();
    //void passVectors(vector<int> &a,vector<int> &b,vector<int> &c);
    void setVectors( vector< vector < vector <int> > > &dataTemp,  vector<int> &totalDataCountTemp, vector< vector<int> > &dataCountTemp, vector< vector<int> > &shiftsTemp);

    void updateAtoms(int start_residue);
   // void fillVectors(vector <int> &tempType1, vector <int> &tempType2, vector <int> &tempshifts,int startingResidue ,int stoppingResidue);
    void fillVectors(int*& tempType1, int*& tempType2,int*& tempshifts,int startingResidue ,int stoppingResidue, int &size);
    double LJDistance(int anchor);
    double LJDistance();
    double vdwca();

    //void setOmega(int,double);
private:
    vector<AminoAcid*> mResidues;
    vector<string> remarks;
    vector< vector < vector <int> > > data; //order: res1 atom1 res2 atom2
    vector<int> totalDataCount; //total number of rdc lines with data
    vector< vector<int> > dataCount; //number of rdc data per residue --index starting at 1
    vector< vector<int> > shifts;



};

#endif
