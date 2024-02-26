/**
 * aminoacid.cpp Copyright 2005, 2008, 2009
 * Dr. Homayoun Valafar, Zach Swearingen, Mikhail Simin
 *
 * This file is part of Redcraft.
 *
 * Redcraft is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Redcraft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Redcraft.  If not, see <http://www.gnu.org/licenses/>.
 */

/* **********************************************************************************
 * This section was moved here from the constructor and commented out by HV on 2/5/15 
#define COORDS_MOLMOL  // could also define COORDS_MOLMOL
#ifdef COORDS_MOLMOL
        atoms[N] =   Atom( 0.000,  0.000,  0.000);
        atoms[H] =   Atom(-0.480,  0.889, -0.002);	// 1.01
//	atoms[H] =   Atom(-0.46602, 0.86311, -0.002);	// 0.98
        atoms[Ca] =  Atom( 1.449,  0.000,  0.000);
        atoms[Ha] =  Atom(1.8029, -0.5031,  0.8998);
        atoms[Ha2] = Atom(1.8029, -0.5031, -0.8998);
        atoms[C] =   Atom(1.9644,  1.4321,  0.0000);
        atoms[O] =   Atom(1.1782,  2.3767,  0.0001);

        atoms[N2] =  Atom(3.2898,  1.5908, 0.0002);
        atoms[H2] =  Atom( 3.892,  0.781, -0.002);
        atoms[Ca2] = Atom( 3.902,  2.904,  0.003);
#endif
#ifdef COORDS_MOLMOL_CIS
        atoms[N] =   Atom( 0.000,  0.000, 0.000);
        atoms[H] =   Atom(-1.009, -0.032, 0.000);	// 1.01
//	atoms[H] =   Atom(-0.980, -0.031, 0.000);	// This needs to be adjusted 0.98
        atoms[Ca] =  Atom(-0.650, 1.295, -0.000);
        atoms[Ha] =  Atom(-1.274, 1.385, -0.890);
        atoms[Ha2] = Atom(-1.274, 1.386, 0.889);
        atoms[C] =   Atom(0.389, 2.407, -0.001);
        atoms[O] =   Atom(1.589, 2.139, -0.001);

        atoms[N2] =  Atom(-0.073, 3.660, -0.002);
        atoms[H2] =  Atom( -1.069, 3.828, -0.002);
        atoms[Ca2] = Atom( 0.820, 4.801, -0.003);
#endif
#ifdef COORDS_MIKE
        atoms[N] = Atom(0.000,   0.000,   0.000);
        atoms[H] = Atom(-0.335,   0.918,   0.253);
        atoms[Ca] = Atom(1.150,  -0.533,   0.703);
        atoms[Ha] = Atom(0.869,  -1.462,   1.200);
        atoms[Ha2] = Atom(1.948,  -0.734,  -0.011);
        atoms[C] = Atom(1.642,   0.465,   1.741);
        atoms[O] = Atom(1.075,   1.545,   1.886);

        atoms[N2] = Atom(2.703,   0.100,   2.467);
        atoms[H2] = Atom(3.134,  -0.799,   2.310);
        atoms[Ca2] = Atom(3.262,   0.966,   3.485);
#endif
#ifdef COORDS_RISHI
        atoms[N] = Atom(-1.1330,    2.9350,   -1.4170);
        atoms[H] = Atom(-1.0540,    3.8630,   -1.8060);
        atoms[Ca] = Atom(0.0840,    2.2010,   -1.1320);
        atoms[Ha] = Atom(0.6400,    2.0640,   -2.0590);
        atoms[C] = Atom(-0.2610,    0.8350,   -0.5560);
        atoms[O] = Atom(0.6300,    0.0490,   -0.2440);

        atoms[Ha2] = Atom(1.948,  -0.734,  -0.011);

        atoms[N2] = Atom(-1.5590,    0.5550,   -0.4160);
        atoms[H2] = Atom(-2.2520,    1.2370,   -0.6900);
        atoms[Ca2] = Atom(-2.0140,   -0.7110,    0.1230);
#endif
 ************************************************************/
#include <cstdlib>
#include "aminoacid.h"
#include "tensor.h"

string AminoAcid::svn_id() {
    return "$Id: aminoacid.cpp 413 2012-09-28 17:54:18Z siminm $";
}

AminoAcid::AminoAcid(double _phi, double _psi, string type, double _omega) : mAMatrix(9, 5) {

    //if (type != "CPR") {
    // COORDS_MOLMOL
    atoms[N] = Atom(0.000, 0.000, 0.000);
//    atoms[H] = Atom(-0.480, 0.889, -0.002); // 1.01
    atoms[H] =   Atom(-0.46602, 0.86311, -0.002);	// 0.98
    atoms[Ca] = Atom(1.449, 0.000, 0.000);
    atoms[Ha] = Atom(1.8029, -0.5031, 0.8998);
    atoms[Ha2] = Atom(1.8029, -0.5031, -0.8998);
    atoms[C] = Atom(1.9644, 1.4321, 0.0000); //Cb?
    atoms[O] = Atom(1.1782, 2.3767, 0.0001);

    atoms[N2] = Atom(3.2898, 1.5908, 0.0002);
//    atoms[H2] = Atom(3.892, 0.781, -0.002);
    atoms[H2] = Atom(3.818, 0.766, -0.002); // 0.98
    atoms[Ca2] = Atom(3.902, 2.904, 0.003);
    /* } else {
         // COORDS_MOLMOL_CIS
         atoms[N] = Atom(0.000, 0.000, 0.000);
         atoms[H] = Atom(-0.480, 0.889, -0.002); // 1.01A
 //	atoms[H] =   Atom(0, 0, 0);	// This needs to be adjusted 0.98
         atoms[Ca] = Atom(1.449, 0.000, 0.000);
         atoms[Ha] = Atom(1.8029, -0.5031, 0.8998);
         atoms[Ha2] = Atom(1.8029, -0.5031, -0.8998); //These coordinates may not be correct, but should not matter for CPR
         atoms[C] = Atom(1.9644, 1.4321, 0.000);
         atoms[O] = Atom(1.1782, 2.3767, 0.0001);

         atoms[N2] = Atom(3.290, 1.591, 0.000);
         atoms[H2] = Atom(3.683, 2.521, 0.000);
         atoms[Ca2] = Atom(4.196, 0.460, -0.001);
     }*/

    phi = 180;
    psi = 180;
    omega = 180;
    mType = type;

    // Default CS:
    //CSA[0] = new Tensor(-108e-6,0,0,46e-6,0); // N
    //CSA[1] = new Tensor(-6e-6,0,0,0,0); // H
    //CSA[2] = new Tensor(-75e-6,0,0,-12e-6,0); // C 	Cornilescu, Bax. JACS 2000

    // CS rotated in MOLMOL M.F.
    CSA[0] = new Tensor(2.44e-05, -8.989e-05, 1.97e-07, 1.56e-05, -1.05e-07); // N
    CSA[1] = new Tensor(-3.00e-06, 3.50e-06, -5.58e-09, -2.66e-06, 1.91e-08); // H
    CSA[2] = new Tensor(40.1e-6, 37.4e-6, 0.02e-6, 44.9e-6, 0.03e-6); // C
    setPhi(_phi);
    setPsi(_psi);
   /*if(type=="CPR"||type=="CPR ")
        setOmega(0);
    /* cout.precision(std::numeric_limits<double>::digits10);
    cout<<"\nAMINO ACID COORDS"<<endl;
    for(int i=0;i<getAtomCount();i++)
    {
        getAtom(i).print();
    }
    cout<<"----------------------"<<endl;*/
}

AminoAcid::AminoAcid(const AminoAcid& old_aa) : mAMatrix(old_aa.mAMatrix) {
    phi = old_aa.phi;
    psi = old_aa.psi;
    omega = old_aa.omega;
    mType = old_aa.mType;

    for (int i = 0; i < AAAtomCount; i++) {
        atoms[i] = Atom(old_aa.atoms[i]);
    }
    CSA[0] = new Tensor(*old_aa.CSA[0]);
    CSA[1] = new Tensor(*old_aa.CSA[1]);
    CSA[2] = new Tensor(*old_aa.CSA[2]);
}

AminoAcid::AminoAcid(const AminoAcid* old_aa) : mAMatrix(old_aa->mAMatrix) {
    phi = old_aa->phi;
    psi = old_aa->psi;
    omega = old_aa->omega;
    mType = old_aa->mType;

    for (int i = 0; i < AAAtomCount; i++) {
        atoms[i] = Atom(old_aa->atoms[i]);
    }
    CSA[0] = new Tensor(*old_aa->CSA[0]);
    CSA[1] = new Tensor(*old_aa->CSA[1]);
    CSA[2] = new Tensor(*old_aa->CSA[2]);
}

AminoAcid::~AminoAcid() {
    delete CSA[0];
    delete CSA[1];
    delete CSA[2];
}

Atom& AminoAcid::operator[](const int type) {
    return atoms[type];
}

Atom& AminoAcid::getAtom(const int type) {
    return atoms[type];
}

double AminoAcid::mass() {
    return AAAtomCount;
}

Atom AminoAcid::center_of_mass() {
    Atom center;
    for (int i = 0; i < AAAtomCount; i++) {
        center.translate(
                atoms[i][0] / AAAtomCount,
                atoms[i][1] / AAAtomCount,
                atoms[i][2] / AAAtomCount);
    }
    return center;
}

void AminoAcid::setAngles(double phi, double psi, double omega) {
    setPhi(phi);
    setPsi(psi);
    setOmega(omega);
}

void AminoAcid::setOmegaPublic(double _omega) {
    setOmega(_omega);
}

void AminoAcid::setPhi(double _phi) {
    /* move N to the origin */
    Atom originalN(atoms[N]);
    translate(-atoms[N][0], -atoms[N][1], -atoms[N][2]);

    double theta = (_phi - phi) * PI / 180;
    Rotation r = Rotation(atoms[Ca][0], atoms[Ca][1], atoms[Ca][2], theta);
    if (true || !r.isIdentity()) {
        for (int i = Ha; i <= Ca2; i++) {
            atoms[i].rotate(r);
        }
    }

    /* move N back to the original position */
    translate(originalN[0], originalN[1], originalN[2]);

    phi = _phi;
    // skipping CSA[0] (N)
    // skipping CSA[1] (H)
    CSA[2]->Rotate(r); // (C)
}

void AminoAcid::setPsi(double _psi) {
    /* move Ca to the origin */
    Atom originalCa(atoms[Ca]);
    translate(-atoms[Ca][0], -atoms[Ca][1], -atoms[Ca][2]);

    double theta = (_psi - psi) * PI / 180;
    Rotation r = Rotation(atoms[C][0], atoms[C][1], atoms[C][2], theta);
    if (true || !r.isIdentity()) {
        for (int i = O; i <= Ca2; i++) {
            atoms[i].rotate(r);
        }
    }

    /* move Ca back to the original position */
    translate(originalCa[0], originalCa[1], originalCa[2]);

    psi = _psi;
    CSA[2]->Rotate(r);
}

void AminoAcid::setOmega(double _omega) {
    // omega=_omega;
    Atom originalC(atoms[C]);
    translate(-atoms[C][0], -atoms[C][1], -atoms[C][2]);

    double theta = (_omega - omega) * PI / 180;
    Rotation r = Rotation(atoms[N2][0], atoms[N2][1], atoms[N2][2], theta);
    if (true || !r.isIdentity()) {
        for (int i = C; i <= Ca2; i++) {
            atoms[i].rotate(r);
        }
    }

    //move Ca back to the original position 
    translate(originalC[0], originalC[1], originalC[2]);

    omega = _omega;
    CSA[2]->Rotate(r);

}

void AminoAcid::updatePrevious(const AminoAcid& next_aa) {
    double temp, angle;
    Atom shift(next_aa.atoms[N]);
    AminoAcid copy(next_aa);
    // this copy will be moved to (0,0,0) for computations
    copy.translate(
            -copy[N][0],
            -copy[N][1],
            -copy[N][2]);
    // move self to origin for computations; intuitive solution would involve subtraction of N for every calculation
    translate(
            -atoms[N2][0],
            -atoms[N2][1],
            -atoms[N2][2]);

    // get rotation axis for H
    Atom Haxis = atoms[H2].cross(copy[H]);

    // get angle for H and rotate
    //TODO: acos may return "nan" if the angle is exactly 0 or 180
    // if the angle is 0 -- do not rotate, but we DO need to rotate for 180

    temp = atoms[H2].dot(copy[H]) / atoms[H2].mag() / copy[H].mag();

    if (temp > -1 && temp < 1) angle = acos(temp);
    else if (temp <= -1) angle = 180;
    else angle = 0;

    rotate(Rotation(Haxis[0], Haxis[1], Haxis[2], angle));

    // get angle for Ca (rotate around H bond)
    Atom axis(atoms[H2]);
    axis.normalize();

    Atom N1 = axis.cross(atoms[Ca2]);
    Atom N2 = axis.cross(copy[Ca]);

    temp = N2.dot(N1) / N2.mag() / N1.mag();

    if (temp > -1 && temp < 1) angle = acos(temp);
    else if (temp <= -1) angle = 180;
    else angle = 0;

    double sign = N1.cross(N2).dot(axis) < 0 ? -1 : 1;

    rotate(Rotation(axis[0], axis[1], axis[2], angle * sign));

    // move N from origin to where it should be
    translate(shift[0], shift[1], shift[2]);
    copy.translate(shift[0], shift[1], shift[2]);
}

void AminoAcid::updatePosition(const AminoAcid& previous_aa) {
    double temp, angle;

    Atom shift(previous_aa.atoms[N2]);
    AminoAcid copy(previous_aa);
    // this copy will be moved to (0,0,0) for computations
    copy.translate(
            -copy[N2][0],
            -copy[N2][1],
            -copy[N2][2]);
    // move self to origin for computations; intuitive solution would involve subtraction of N for every calculation
    translate(
            -atoms[N][0],
            -atoms[N][1],
            -atoms[N][2]);

    // get rotation axis for H
    Atom Haxis = atoms[H].cross(copy[H2]);

    // get angle for H and rotate
    temp = atoms[H].dot(copy[H2]) / atoms[H].mag() / copy[H2].mag();

    if (temp > -1 && temp < 1) angle = acos(temp);
    else if (temp <= -1) angle = 180;
    else angle = 0;

    rotate(Rotation(Haxis[0], Haxis[1], Haxis[2], angle));

    // get angle for Ca (rotate around H bond)
    Atom axis(atoms[H]);
    axis.normalize();

    Atom N1 = axis.cross(atoms[Ca]);
    Atom N2 = axis.cross(copy[Ca2]);

    temp = N1.dot(N2) / N1.mag() / N2.mag();

    if (temp > -1 && temp < 1) angle = acos(temp);
    else if (temp <= -1) angle = 180;
    else angle = 0;

    double sign = N1.cross(N2).dot(axis) < 0 ? -1 : 1;

    rotate(Rotation(axis[0], axis[1], axis[2], angle * sign));

    // move N from origin to where it should be
    translate(shift[0], shift[1], shift[2]);
    copy.translate(shift[0], shift[1], shift[2]);

}

void AminoAcid::translate(double x, double y, double z) {
    for (int i = 0; i < AAAtomCount; i++) {
        atoms[i].translate(x, y, z);
    }
}

void AminoAcid::rotate(Rotation r) {
    for (int i = 0; i < AAAtomCount; i++) {
        atoms[i].rotate(r);
    }
    CSA[0]->Rotate(r);
    CSA[1]->Rotate(r);
    CSA[2]->Rotate(r);
}

void AminoAcid::calculateAMatrix(AminoAcid &prev) {
    int type1[] = {C, N, C, Ca, Ca, Ha, Ha2, Ha, Ha2};
    int type2[] = {N, H, H, Ha, Ha2, H, H, H, H};
    int shift1[] = {-1, 0, -1, 0, 0, 0, 0, -1, -1};
    int shift2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    long int maxValues[] = {6125, 24350, -60400, -60400, -60400, -240200, -240200, -240200, -240200};

    AminoAcid * arr[] = {&prev, this}; //this does not allow for rdc from non adjacent amino acids

    for (int vector = 0; vector < 9; vector++) {
        double dx = (*arr[shift2[vector] + 1])[type2[vector]][0] - (*arr[shift1[vector] + 1])[type1[vector]][0]; //change in x
        double dy = (*arr[shift2[vector] + 1])[type2[vector]][1] - (*arr[shift1[vector] + 1])[type1[vector]][1]; //y
        double dz = (*arr[shift2[vector] + 1])[type2[vector]][2] - (*arr[shift1[vector] + 1])[type1[vector]][2]; //z
        double r = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
        double factor;

        if (r > 0) {
            dx /= r;
            dy /= r;
            dz /= r;
            factor = maxValues[vector % 9] / pow(r, 3);
        } else { // this should never happen
            cerr << "AminoAcid::calculateAMatrix" << endl;
            cerr << "vector: " << vector << endl;
            cerr << "x1: " << (*arr[shift1[vector] + 1])[type1[vector]][0] << " ";
            cerr << "x2: " << (*arr[shift2[vector] + 1])[type2[vector]][0] << endl;
            cerr << "vector magnitude of zero; cannot normalize" << std::endl;
            cerr << "dx: " << dx << " dy: " << dy << " dz: " << dz << std::endl;
            cerr << "Angles are: " << phi << " " << psi << endl;
            factor = 1;
        }

        mAMatrix.M[vector][0] = factor * (pow(dx, 2) - pow(dz, 2));
        mAMatrix.M[vector][1] = factor * (pow(dy, 2) - pow(dz, 2));
        mAMatrix.M[vector][2] = factor * 2 * dx * dy;
        mAMatrix.M[vector][3] = factor * 2 * dx * dz;
        mAMatrix.M[vector][4] = factor * 2 * dy * dz;
    }

}

void AminoAcid::calculateAMatrix(AminoAcid &prev, int* type1, int* type2, int* shift1, int rdcEntries) {
    //cout<<type1.size()<<" - "<<type2.size()<<" : "<<shift1.size()<<endl;
    int shift2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    /*   cout << "\nType 1 - Type 2" << endl;
       for (int r = 0; r < type1.size(); r++) {
           cout << type1[r] << " " << type2[r] << endl;
       }
        /**/
    AminoAcid * arr[] = {&prev, this}; //this does not allow for rdc from non adjacent amino acids

    for (int vector = 0; vector < rdcEntries; vector++) {
        double dx = (*arr[shift2[vector] + 1])[type2[vector]][0] - (*arr[shift1[vector] + 1])[type1[vector]][0]; //change in x
        double dy = (*arr[shift2[vector] + 1])[type2[vector]][1] - (*arr[shift1[vector] + 1])[type1[vector]][1]; //y
        double dz = (*arr[shift2[vector] + 1])[type2[vector]][2] - (*arr[shift1[vector] + 1])[type1[vector]][2]; //z
        // cout <<"Atom: "<<vector <<"\ndx: " << dx << "\ndy: " << dy << "\ndz: " << dz << endl;
        double r = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
        // cout << "r: " << r << endl;
        double factor;
        if (r > 0) {
            dx /= r;
            dy /= r;
            dz /= r;
            factor = mapMaxValue(type1[vector], type2[vector]) / pow(r, 3);
            // cout << "factor: " << factor << endl;
            // cout<<mapMaxValue(type1[vector],type2[vector])<<endl;
        } else { // this should never happen
            /* cerr << "AminoAcid::calculateAMatrix" << endl;
             cerr << "vector: " << vector << endl;
             cerr << "x1: " << (*arr[shift1[vector] + 1])[type1[vector]][0] << " ";
             cerr << "x2: " << (*arr[shift2[vector] + 1])[type2[vector]][0] << endl;
             cerr << "vector magnitude of zero; cannot normalize" << std::endl;
             cerr << "dx: " << dx << " dy: " << dy << " dz: " << dz << std::endl;
             cerr << "Angles are: " << phi << " " << psi << endl;*/
            factor = 1;
        }

        mAMatrix.M[vector][0] = factor * (pow(dx, 2) - pow(dz, 2));
        mAMatrix.M[vector][1] = factor * (pow(dy, 2) - pow(dz, 2));
        mAMatrix.M[vector][2] = factor * 2 * dx * dy;
        mAMatrix.M[vector][3] = factor * 2 * dx * dz;
        mAMatrix.M[vector][4] = factor * 2 * dy * dz;
    }
    /* cout<<"mAMAtrix\n";
     mAMatrix.Print();
     cout<<"\n";/**/
}

void AminoAcid::fillAMatrix2(Matrix &matrix, AminoAcid &prev, int start) {

    int offset = 0;
    for (int row = 0; row < mAMatrix.GetRow(); row++) {

        // cout << "row: " << row << endl;
        for (int col = 0; col < 5; col++) {
            //  cout<<mAMatrix.M[row][col]<<" ";
            matrix.M[start + offset][col] = mAMatrix.M[row][col];
        }
        // cout<<"\n";
        if (row == 3 || row == 5 || row == 7) {
            row++;
        }

        offset++;
    }/**/
    //matrix.Print();
    /*if (mType.compare("GLY") == 0) {
        for (int col = 0; col < 5; col++) {
            matrix.M[start + 3][col] += mAMatrix.M[4][col];
            matrix.M[start + 3][col] /= 2;
            matrix.M[start + 4][col] += mAMatrix.M[6][col];
            matrix.M[start + 4][col] /= 2;
        }
    }
    if (prev.mType.compare("GLY") == 0) {
        for (int col = 0; col < 5; col++) {
            matrix.M[start + 5][col] += mAMatrix.M[8][col];
            matrix.M[start + 5][col] /= 2;
        }
    }*/
}

void AminoAcid::fillAMatrix(Matrix &matrix, AminoAcid &prev, int start) {
    int offset = 0;
    for (int row = 0; row < mAMatrix.GetRow(); row++) {
        if (row == 3 || row == 5 || row == 7) {
            row++;
        }
        // cout << "row: " << row << endl;
        for (int col = 0; col < 5; col++) {
            //  cout<<mAMatrix.M[row][col]<<" ";
            matrix.M[start + offset][col] = mAMatrix.M[row][col];
        }
        // cout<<"\n";


        offset++;
    }
}

double AminoAcid::getCSAdd(int n, Tensor * S) {
    if (S == NULL) return 0;
    double shift = 0;

    /*	Matrix Sign, Rn;
            CSA[n]->Decompose(Sign, Rn);

            Matrix Sp, R;
            S->Decompose(Sp, R);

            Matrix sum = (R*Sp).Transpose() * Rn * Sign;

            for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                            shift += sum.M[i][j];
     */


    shift = 0;
    shift = S->getXX() * CSA[n]->getXX() + S->getYY() * CSA[n]->getYY() + (S->getXX() + S->getYY())*(CSA[n]->getXX() + CSA[n]->getYY()) + 2 * (S->getXY() * CSA[n ]->getXY() + S->getXZ() * CSA[n]->getXZ() + S->getYZ() * CSA[n]->getYZ());
    /*
    shift =   Sxx * CSAxx
            + Syy * CSAyy
            + (Sxx+Syy)*(CSAxx+CSAyy)
            + 2 * (
                      Sxy * CSAxy
                    + Sxz * CSAxz
                    + Syz * CSAyz
            );
     */
    return shift;
}

void AminoAcid::Print() {
    for (int i = 0; i < getAtomCount(); i++) {
        getAtom(i).print();

    }
}

int AminoAcid::mapMaxValue(int a, int b) {
    if ((a == AminoAcid::C || a == AminoAcid::Ca) && (b == AminoAcid::N || b == AminoAcid::N2)) {
        return 6125;
    }
    if ((a == AminoAcid::N || a == AminoAcid::N2)&&(b == AminoAcid::C || b == AminoAcid::Ca)) {
        return 6125;
    } else if ((a == AminoAcid::N || a == AminoAcid::N2)&& (b == AminoAcid::H || b == AminoAcid::H2)) {
        return 24350;
    } else if ((b == AminoAcid::N || b == AminoAcid::N2)&& (a == AminoAcid::H || a == AminoAcid::H2)) {
        return 24350;
    } else if ((a == AminoAcid::C || a == AminoAcid::Ca || a == AminoAcid::Ca2)&&(b == AminoAcid::H || b == AminoAcid::H2 || b == AminoAcid::Ha || b == AminoAcid::Ha2)) {
        return -60400;
    } else if ((a == AminoAcid::H || a == AminoAcid::H2 || a == AminoAcid::Ha || a == AminoAcid::Ha2)&&(b == AminoAcid::C || b == AminoAcid::Ca || b == AminoAcid::Ca2)) {
        return -60400;
    } else if ((a == AminoAcid::Ha || a == AminoAcid::Ha2) && (b == AminoAcid::H || b == AminoAcid::H2)) {
        return -240200;
    } else if ((a == AminoAcid::C || a == AminoAcid::Ca || a == AminoAcid::Ca2)&&(b == AminoAcid::C || b == AminoAcid::Ca || b == AminoAcid::Ca2)) {
        return 15200;
    }

    return 6125;
}
