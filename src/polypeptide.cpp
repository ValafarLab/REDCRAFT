/**
 * polypeptide.cpp Copyright 2009, 2010, 2020
 * Dr. Homayoun Valafar, Mikhail Simin, Zach Swearingen, Julian Rachele
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

#include <cstdlib>
#include "polypeptide.h"
#include <Eigen/SVD>

#ifdef _WIN32
#include <ctime>
#endif 

string Polypeptide::svn_id() {
    return "$Id: polypeptide.cpp 414 2012-10-03 15:32:06Z browndd3 $";
}

void Polypeptide::init() {
    add_remark("Generated with REDCRAFT engine");
    time_t rt;
    time(&rt);
    string genDate = string(ctime(&rt));
    genDate.resize(genDate.length() - 2);
    add_remark("Generation Date: " + genDate);
    add_remark(string("Polypeptide compilation date: ") + string(__DATE__));
    add_remark(string("Polypeptide subversion id: ") + svn_id());
    appendAminoAcid();


    this->id = -1;
}

Polypeptide::Polypeptide(double *angles, int count) {
    init();
    for (int i = 0; i < count; i += 2)
        appendAminoAcid(angles[i], angles[i + 1]);
}

void Polypeptide::setAngles(double* angles, int count) {
     for (int i = 0; i < count; i += 2)
        appendAminoAcid(angles[i], angles[i + 1]);
}

Polypeptide::Polypeptide() {
    init();
}

Polypeptide::~Polypeptide() {
    for (int i = 0; i < mResidues.size(); i++) {
        delete mResidues[i];
    }
    /* delete data;
     delete totalDataCount;
     delete dataCount;
     delete shifts;/**/
}

Polypeptide::Polypeptide(const Polypeptide &copy) {
    for (size_t aminoAcid = 0; aminoAcid < copy.mResidues.size(); aminoAcid++) {
        AminoAcid* temp = new AminoAcid(copy.mResidues[aminoAcid]);
        mResidues.push_back(temp);
    }
    for (int i = 0; i < copy.remarks.size(); i++)
        add_remark(copy.remarks[i]);

    this->id = copy.id;
}

double Polypeptide::mass() {
    // should be a loop if real amino acids are used
    return mResidues.size() * 10;
}

Atom Polypeptide::center_of_mass() {
    Atom center;
    for (int i = 0; i < mResidues.size(); i++) {
        Atom iCenter = mResidues[i]->center_of_mass();
        center.translate(
                iCenter[0] / mResidues.size(),
                iCenter[1] / mResidues.size(),
                iCenter[2] / mResidues.size());
    }
    return center;
}

double Polypeptide::bbrmsd(const Polypeptide & compare) {
    Polypeptide copy(compare);
    cout << "comparing " << getResidueCount() << " res vs " << copy.getResidueCount() << " res" << endl;


    Atom copycm = copy.center_of_mass(); // target center of mass
    Atom mycm = center_of_mass(); // current center of mass
    translate(-mycm[0], -mycm[1], -mycm[2]);
    copy.translate(-copycm[0], -copycm[1], -copycm[2]);

    // Centers of mass are overlaying at the origin. Find optimal rotation

    Eigen::MatrixXd A(getBBCoordinates().toEigen());

    Eigen::MatrixXd B(copy.getBBCoordinates().toEigen());
    Eigen::MatrixXd rot(3, 3);

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    rot = svd.solve(B);

    // create rotation object that can be utilzied by REDCRAFT & rotate
    Rotation R(Matrix(rot).Transpose());
    rotate(R);

    // get the new coordinates & compute RMSD
    Matrix newA(getBBCoordinates());

    double sum = 0;
    for (int i = 0; i < newA.GetRow(); i++) {
        sum += pow(newA.Get_Mij(i, 0) - B(i, 0), 2); // x
        sum += pow(newA.Get_Mij(i, 1) - B(i, 1), 2); // y
        sum += pow(newA.Get_Mij(i, 2) - B(i, 2), 2); // z
        cout << "Atom " << i << " ("
                << newA.Get_Mij(i, 0) << ","
                << newA.Get_Mij(i, 1) << ","
                << newA.Get_Mij(i, 2) << ")"
                << " ("
                << B(i, 0) << ","
                << B(i, 1) << ","
                << B(i, 2) << ")" << endl;
    }
    return sqrt(sum / newA.GetRow());
}

void Polypeptide::setAngles(int residue, double phi, double psi, double omega) {
    residue++;
    if (residue < 0 || residue >= mResidues.size()) {
        cerr << "residue number out of bounds in call to Polypeptide::setAngles(" << residue
                << ", " << phi << ", " << psi << ", " << omega << ");" << endl;
        exit(1);
    }
    /* if (mResidues[residue]->getType() == "CPR "||mResidues[residue]->getType() == "CPR") {
         mResidues[residue]->setAngles(phi, psi, 0);
     } else {*/
    mResidues[residue]->setAngles(phi, psi, omega);
    // }
    updateAtoms(residue - 1);
}

void Polypeptide::rotate(Rotation r) {
    //if (r.isIdentity()) return; // no need to rotate by identity

    for (int aa = 0; aa < mResidues.size(); aa++)
        mResidues[aa]->rotate(r);
}

void Polypeptide::translate(double x, double y, double z) {
    for (int aa = 0; aa < mResidues.size(); aa++)
        mResidues[aa]->translate(x, y, z);
}

AminoAcid * Polypeptide::getAminoAcid(int index) {
    index++;
    if (index < 0 || index >= mResidues.size()) {
        cerr << "index out of bounds in call to Polypeptide::getAminoAcid(" << index << ");" << endl;
        cerr << "last residue is " << mResidues.size() - 1 << endl;
        exit(1);
    }

    return mResidues[index];
}

void Polypeptide::appendAminoAcid(double phi, double psi, string name, double omega) {
    int* tempType1;
    int* tempType2;
    int* tempshifts;
    int size=0;

    AminoAcid * temp = new AminoAcid(phi, psi, name, omega);
    if (mResidues.size() >= 1) {
        fillVectors(tempType1, tempType2, tempshifts, 0, mResidues.size() - 1,size);
        temp->updatePosition(mResidues[mResidues.size() - 1]);
        temp->calculateAMatrix(*mResidues[mResidues.size() - 1], tempType1, tempType2, tempshifts, size);
        delete[] tempType1;
        delete[] tempType2;
        delete[] tempshifts;
    }
    mResidues.push_back(temp);
}

void Polypeptide::appendAminoAcid2(double phi, double psi, string name, double omega) {
    AminoAcid * temp = new AminoAcid(phi, psi, name, omega);
    if (mResidues.size() > 0) {
        temp->updatePosition(mResidues[mResidues.size() - 1]);
        temp->calculateAMatrix(*mResidues[mResidues.size() - 1]);
    }
    mResidues.push_back(temp);
}

void Polypeptide::prependAminoAcid(double phi, double psi, string name, double omega) {
    AminoAcid * temp = new AminoAcid(180, 180, "ALA", 180); // create new dummy

    mResidues.insert(mResidues.begin(), temp); // insert new dummy

    mResidues[1]->setAngles(phi, psi, omega);
    mResidues[1]->setType(name);

    mResidues[1]->updatePrevious(mResidues[2]);
    mResidues[0]->updatePrevious(mResidues[1]);

    /*cout << "cming " << phi << " " << psi << endl;
    for (int i = mResidues.size() - 1; i > 0; i--) {
            //cout << i << ".cm(" << i-1 << ")" << endl;
            mResidues[i].calculateAMatrix(mResidues[i-1]);
    }
    //cout << "done";
     */

    //MAKE WORK BACKWARDS

    mResidues[2]->calculateAMatrix(*mResidues[1]);
    mResidues[1]->calculateAMatrix(*mResidues[0]);
}

int Polypeptide::getResidueCount() {
    return mResidues.size() - 1;
}

void Polypeptide::updateAtoms(int start) {
    if (start > 0) {
        int* Type1;
        int* Type2;
        int* Shifts;
        int Size=0;
        fillVectors(Type1, Type2, Shifts, start - 1, mResidues.size() - 2,Size); //added -2  
        /*cout<<"\n****************\n";
         for(int i=0;i<Type1.size();i++)
        cout<<Type1[i]<<" "<<Type2[i]<<" "<<Shifts[i]<<endl;
         cout<<" \n*******************"<<endl;/**/
        //  mResidues[start]->calculateAMatrix(*mResidues[start - 1]);
        mResidues[start]->calculateAMatrix(*mResidues[start - 1], Type1, Type2, Shifts, Size);
        if (Size > 0) {
            delete[] Type1;
            delete[] Type2;
            delete[] Shifts;
        }
    }
    for (int i = start + 1; i < mResidues.size(); i++) {
        int* type1;
        int* type2;
        int* shifts;
        int size=0;
        fillVectors(type1, type2, shifts, 0, i - 1,size);
        mResidues[i]->updatePosition(*mResidues[i - 1]);
        mResidues[i]->calculateAMatrix(*mResidues[i - 1], type1, type2, shifts, size);
        if (size > 0) {
            delete[] type1;
            delete[] type2;
            delete[] shifts;
        }
    }
}

Matrix Polypeptide::getCoordinates() {
    int type1[] = {AminoAcid::C, AminoAcid::N, AminoAcid::C, AminoAcid::Ca, AminoAcid::Ha, AminoAcid::Ha};
    int type2[] = {AminoAcid::N, AminoAcid::H, AminoAcid::H, AminoAcid::Ha, AminoAcid::H, AminoAcid::H};
    int shift1[] = {-1, 0, -1, 0, 0, -1};
    int shift2[] = {0, 0, 0, 0, 0, 0};

    Matrix coords(6 * (getResidueCount()), 6);

    int index = 0;
    for (int i = 1; i < mResidues.size(); i++) {
        for (int k = 0; k < 6; k++) {
            if (index >= coords.GetRow()) {
                std::cerr << "error: coordinates matrix too small" << std::endl;
                exit(1);
            }
            for (int j = 0; j < 3; j++) {
                coords.M[index][j] = (i + shift1[k] < 0 || i + shift1[k] >= mResidues.size()) ? 0 : mResidues[i + shift1[k]]->getAtom(type1[k])[j];
                coords.M[index][j + 3] = (i + shift2[k] < 0 || i + shift2[k] >= mResidues.size()) ? 0 : mResidues[i + shift2[k]]->getAtom(type2[k])[j];
                // cout<<mResidues[i + shift1[k]]->getAtom(type1[k])[j]<<"/ "<<mResidues[i + shift2[k]]->getAtom(type2[k])[j]<<" Residue:"<<i<<" index:"<<index<<endl;
            }
            index++;
        }
    }

    return coords;
}

Matrix Polypeptide::getCoordinates(vector< vector < vector <int> > > &data, vector<vector<int> > &dataCount, vector<int> &totalDataCount, vector< vector<int> > &shifts, int medium) {
    //int size = totalDataCount[medium];
    /*int type1[] = {AminoAcid::C, AminoAcid::N, AminoAcid::C, AminoAcid::Ca, AminoAcid::Ha, AminoAcid::Ha};
    int type2[] = {AminoAcid::N, AminoAcid::H, AminoAcid::H, AminoAcid::Ha, AminoAcid::H, AminoAcid::H};
    int shift1[] = {-1, 0, -1, 0, 0, -1};
    int shift2[] = {0, 0, 0, 0, 0, 0};/**/
    cout << "mResidues.size: " << mResidues.size() << endl;
    int size = 0;
    for (int i = 1; i < mResidues.size(); i++) {
        if (i < dataCount[medium].size()) {
            size += dataCount[medium][i];
            // cout << i << ": " << dataCount[medium][i]<< endl;

        } else if (i = dataCount[medium].size()) {
           // size = size + 6;
        }
    }
    //  cout << size << endl;
    Matrix coords(size, 6);
    // Matrix coords(6 * (getResidueCount()), 6);
    int index = 0;
    for (int i = 1; i < mResidues.size() + 1; i++) {
        if (i <= dataCount[medium].size()) {
            for (int k = 0; k < dataCount[medium][i - 1]; k++) {
                if (index >= coords.GetRow()) {
                    std::cerr << "error: coordinates matrix too small" << std::endl;
                    exit(1);
                }
                //int test=i+shifts[medium][index];
                //  cout<< test<<endl;
                for (int j = 0; j < 3; j++) {
                    coords.M[index][j] = mResidues[i + shifts[medium][index] - 1]->getAtom(data[medium][index][1])[j]; //minus 1 for shifts
                    // cout<< mResidues[i+shifts[medium][index]]->getAtom(data[medium][index][1])[j]<<endl;
                    coords.M[index][j + 3] = mResidues[i - 1]->getAtom(data[medium][index][3])[j]; //minus 1 becuse it works for right now

                }
                //cout << index << endl;
                // mResidues[i + shifts[medium][index]]->getAtom(data[medium][index][1]).print();
                index++;
            }
        } /*else {
            for (int k = 0; k < 6; k++) {
                if (index >= coords.GetRow()) {
                    std::cerr << "error: coordinates matrix too small" << std::endl;
                    exit(1);
                }
                for (int j = 0; j < 3; j++) {
                    coords.M[index][j] = (i + shift1[k] - 1 < 0 || i + shift1[k] - 1 >= mResidues.size()) ? 0 : mResidues[i + shift1[k] - 1]->getAtom(type1[k])[j];
                    coords.M[index][j + 3] = (i + shift2[k] - 1 < 0 || i + shift2[k] - 1 >= mResidues.size()) ? 0 : mResidues[i + shift2[k] - 1]->getAtom(type2[k])[j];
                    // cout<<mResidues[i + shift1[k]]->getAtom(type1[k])[j]<<"/ "<<mResidues[i + shift2[k]]->getAtom(type2[k])[j]<<" Residue:"<<i<<" index:"<<index<<endl;
                }
                index++;
            }
        }*/
    }
    return coords;


}

/*int Polypeptide::shift(int a, int b)
{
    
    if(a==AminoAcid::C&&b==AminoAcid::N)
    {
        return -1;
    }
    else if(a==AminoAcid::C&&b==AminoAcid::H)
    {
        return -1;
    }
    else if(a==AminoAcid::C&&b==AminoAcid::N)
    {
        return -1;
    }
}*/
Matrix Polypeptide::getPDBCoordinates() {
    int total_atoms = 0;
    for (int i = 0; i < getResidueCount(); i++) {
        total_atoms += getAminoAcid(i)->getAtomCount();
    }
    Matrix coords(total_atoms, 3);

    int index = 0;
    for (int i = 0; i < getResidueCount(); i++) {
        for (int a = 0; a < getAminoAcid(i)->getAtomCount(); a++) {
            coords.M[index][0] = getAminoAcid(i)->getAtom(a)[0];
            coords.M[index][1] = getAminoAcid(i)->getAtom(a)[1];
            coords.M[index][2] = getAminoAcid(i)->getAtom(a)[2];
            index++;
        }
    }

    return coords;
}

Matrix Polypeptide::getBBCoordinates() {
    int total_atoms = getResidueCount() * 3; // N, Ca, C
    Matrix coords(total_atoms, 3);

    int i = 0;
    for (int aa = 0; aa < getResidueCount(); aa++) {
        coords.M[i][0] = getAminoAcid(aa)->getAtom(AminoAcid::N)[0];
        coords.M[i][1] = getAminoAcid(aa)->getAtom(AminoAcid::N)[1];
        coords.M[i][2] = getAminoAcid(aa)->getAtom(AminoAcid::N)[2];

        coords.M[i + 1][0] = getAminoAcid(aa)->getAtom(AminoAcid::Ca)[0];
        coords.M[i + 1][1] = getAminoAcid(aa)->getAtom(AminoAcid::Ca)[1];
        coords.M[i + 1][2] = getAminoAcid(aa)->getAtom(AminoAcid::Ca)[2];

        coords.M[i + 2][0] = getAminoAcid(aa)->getAtom(AminoAcid::C)[0];
        coords.M[i + 2][1] = getAminoAcid(aa)->getAtom(AminoAcid::C)[1];
        coords.M[i + 2][2] = getAminoAcid(aa)->getAtom(AminoAcid::C)[2];

        i += 3;
    }

    return coords;
}

Matrix Polypeptide::getAMatrix() {
    Matrix temp(getResidueCount() * 6, 5);
    int offset = 0;

    for (int residue = 1; residue < mResidues.size(); residue++) {
        //if(residue==1){
        mResidues[residue]->fillAMatrix(temp, *mResidues[residue - 1], offset);
        //}
        /*else{
            mResidues[residue]->fillAMatrix2(temp, *mResidues[residue - 1], offset);
        }*/
        offset += 6;
    }
    //temp.Print();
    return temp;
}

void Polypeptide::print() {
    for (int i = 0; i < mResidues.size(); i++) {
        if (mResidues[i]->getPhi() >= 0) cout << " ";
        if (abs(mResidues[i]->getPhi()) < 10) cout << " ";
        if (abs(mResidues[i]->getPhi()) < 100) cout << " ";
        cout << mResidues[i]->getPhi() << " ";
        if (mResidues[i]->getPsi() >= 0) cout << " ";
        if (abs(mResidues[i]->getPsi()) < 10) cout << " ";
        if (abs(mResidues[i]->getPsi()) < 100) cout << " ";
        cout << mResidues[i]->getPsi() << " ";
        if (mResidues[i]->getOmega() >= 0) cout << " ";
        if (abs(mResidues[i]->getOmega()) < 10) cout << " ";
        if (abs(mResidues[i]->getOmega()) < 100) cout << " ";
        cout << mResidues[i]->getOmega() << ":" << endl;
    }
}

void Polypeptide::writePDB(string filename, int start_residue) {
    char * value;
    int decimal, sign;
    string tempString;
    ofstream outputFile;

    try {
        outputFile.open(filename.c_str());
    } catch (exception e) {
        std::cout << "couldn't open " << filename << "printing results to screen" << std::endl;
    }


    //for (int i = 0; i < remarks.size(); i++) outputFile << "REMARK 999 " << remarks[i] << endl;

    int atomNum = 1;
    for (int i = 1; i < mResidues.size() - 1; i++) {//changed from size()-1
        //cout << i << " " << mResidues[i]->getType() << " ";
        for (int j = AminoAcid::N; j <= AminoAcid::O; j++) {
            string msg(80, ' ');

            msg.replace(0, 6, "ATOM  ");

            value = ecvt(atomNum, 5, &decimal, &sign);
            value[decimal] = '\0';
            msg.replace((11 - decimal), decimal, value);

            switch (j) {
                case AminoAcid::N:
                    msg.replace(13, 1, "N");
                    break;
                case AminoAcid::H:
                    if (mResidues[i]->getType().compare("PRO") == 0 || mResidues[i]->getType().compare("PRO ") == 0) continue;
                    msg.replace(13, 1, "H");
                    break;
                case AminoAcid::Ca:
                    msg.replace(13, 2, "CA");
                    break;
                case AminoAcid::Ha:
                    if (mResidues[i]->getType().compare("GLY") == 0 || mResidues[i]->getType().compare("GLY ") == 0)
                        msg.replace(13, 3, "HA1");
                    else
                        continue;
                    //   msg.replace(13, 2, "HA");
                    break;
                case AminoAcid::Ha2:
                    if (mResidues[i]->getType().compare("GLY") == 0 || mResidues[i]->getType().compare("GLY ") == 0)
                        msg.replace(13, 3, "HA2");
                    else
                        msg.replace(13, 2, "HA");
                    break;
                case AminoAcid::C:
                    msg.replace(13, 1, "C");
                    break;
                case AminoAcid::O:
                    msg.replace(13, 1, "O");
                    break;
            }

            //Leaveing altLoc blank!

            msg.replace(17, 3, mResidues[i]->getType());

            value = ecvt((i + start_residue - 1), 4, &decimal, &sign); //might need to change to 6
            value[decimal] = '\0';
            msg.replace((25 - decimal), decimal, value);

            //Leaving iCode blank!

            for (int k = 0, pos = 30; k <= 2; k++) {
                value = ecvt(mResidues[i]->getAtom(j)[k], 6, &decimal, &sign);
                tempString = value;
                if (decimal > 0) {
                    tempString.insert(decimal, ".");
                } else {
                    tempString.insert(0, "0.");
                    for (int x = 0; x > decimal; x--) {
                        tempString.insert(2, "0");
                    }
                }
                if (sign != 0) {
                    tempString.insert(0, "-");
                }
                msg.replace(pos, 7, tempString.substr(0, 7));

                pos += 9;
            }

            // occupancy
            msg.replace(54, 6, "  1.00");

            // temperature factor
            msg.replace(60, 6, "  0.00");

            //Leaving segID blank!
            //Leaving element blank!
            //Leaving charge blank!

            try {
                outputFile << msg << std::endl;
            } catch (exception e) {
                std::cout << msg << std::endl;
            }
            atomNum++;
        }
    }

    outputFile.close();
}

double Polypeptide::LJDistance() {
    return LJDistance(mResidues.size() - 1);
}

double Polypeptide::LJDistance(int anchorIndex) {
    if (anchorIndex < 0 || anchorIndex > mResidues.size() - 1)
        return -1.0;

    AminoAcid *anchor = (mResidues[anchorIndex]);
    AminoAcid *cursor;

    double LJSum = 0.0;
    double mindist = 5;
    double dx, dy, dz, r, A, B;
    double eps = 0.0903;
    double sig = 3.815; // distance between two Ca of the same peptide plane
    for (int residue = 1; residue < mResidues.size() - 1; residue++) {
        cursor = (mResidues[residue]);
        if (anchor == cursor)
            continue;

        dx = cursor->getAtom(2).getCoordinate(0) - anchor->getAtom(2).getCoordinate(0);
        dy = cursor->getAtom(2).getCoordinate(1) - anchor->getAtom(2).getCoordinate(1);
        dz = cursor->getAtom(2).getCoordinate(2) - anchor->getAtom(2).getCoordinate(2);

        r = sqrt(dx * dx + dy * dy + dz * dz);

        double temp = pow(sig / r, 12) - pow(sig / r, 6);
        LJSum += temp;

    }

    //return (LJSum < 0.0) ? 0.0 : LJSum;
    return LJSum * 4 * eps;
    //return mindist;
}

double Polypeptide::vdwca() {

    AminoAcid *anchor;
    AminoAcid *cursor;

    double LJSum = 0.0;
    double mindist = 5;
    double dx, dy, dz, r, A, B;
    double eps = 0.0903;
    double sig = 3.2072;


    for (int i = 1; i < mResidues.size() - 2; i++) {
        for (int j = i + 1; j < mResidues.size() - 1; j++) {
            cursor = (mResidues[i]);
            anchor = (mResidues[j]);

            dx = cursor->getAtom(2).getCoordinate(0) - anchor->getAtom(2).getCoordinate(0);
            dy = cursor->getAtom(2).getCoordinate(1) - anchor->getAtom(2).getCoordinate(1);
            dz = cursor->getAtom(2).getCoordinate(2) - anchor->getAtom(2).getCoordinate(2);

            r = sqrt(dx * dx + dy * dy + dz * dz);

            A = 4 * eps * pow(sig, 12);
            B = 4 * eps * pow(sig, 6);
            LJSum += (A / pow(r, 12)) - (B / pow(r, 6));
        }
    }

    return LJSum;
}

void Polypeptide::Print() {
    int type1[] = {AminoAcid::C, AminoAcid::N, AminoAcid::C, AminoAcid::Ca, AminoAcid::Ha, AminoAcid::Ha};
    int type2[] = {AminoAcid::N, AminoAcid::H, AminoAcid::H, AminoAcid::Ha, AminoAcid::H, AminoAcid::H};
    for (int i = 0; i < getResidueCount(); i++) {
        for (int k = 0; k < 6; k++) {

            cout << "[ " << mResidues[i]->getAtom(type1[k])[0] << ", " << mResidues[i]->getAtom(type1[k])[1] << ", " << mResidues[i]->getAtom(type1[k])[2] << ", " << mResidues[i]->getAtom(type2[k])[0] << ", " << mResidues[i]->getAtom(type2[k])[1] << ", " << mResidues[i]->getAtom(type2[k])[2] << " ]" << endl;
            // cout<<mResidues[i + shift1[k]]->getAtom(type1[k])[j]<<"/ "<<mResidues[i + shift2[k]]->getAtom(type2[k])[j]<<" Residue:"<<i<<" index:"<<index<<endl;

        }
    }
}

void Polypeptide::setVectors(vector< vector < vector <int> > > &dataTemp, vector<int> &totalDataCountTemp, vector< vector<int> > &dataCountTemp, vector< vector<int> > &shiftsTemp) {
    // cout << "started setVectors" << endl;
    //cout << "Capacity before " << data.clear() << endl;
    //shifts.clear();
    data = dataTemp;
    //cout << "Capacity after " << data.size() << endl;
    //cout << "data = dataTemp " << endl;
    totalDataCount = totalDataCountTemp;
    // cout << "totalDataCount = totalDataCountTemp" << endl;
    dataCount = dataCountTemp;
    // cout << "dataCount = dataCountTemp" << endl;
    shifts = shiftsTemp;

}

void Polypeptide::fillVectors(int*& tempType1, int*& tempType2, int*& tempshifts, int startingResidue, int stoppingResidue, int &size) {
    if (stoppingResidue == 0)
        stoppingResidue = 1;
    int counter = 0;
    int offset = 0;
    for (int i = startingResidue; i < stoppingResidue; i++) {
        offset += dataCount[0][i];
    }
    size = dataCount[0][stoppingResidue] + 3; //+3 to make the vectors of size 9 with 6 rdcs; might hard code to 9
    tempType1 = new int[size];
    tempType2 = new int[size];
    tempshifts = new int[size];
    for (int i = 0; i < size; i++) {
        if (i == 3) {
            tempType1[3] = 0;
            tempType2[3] = 0;
            tempshifts[3] = 0;
        } else if (i == 5) {
            tempType1[5] = 0;
            tempType2[5] = 0;
            tempshifts[5] = 0;
        } else if (i == 7) {
            tempType1[7] = 0;
            tempType2[7] = 0;
            tempshifts[7] = 0;
        } else {
            tempType1[i] = data[0][counter + offset][1];
            tempType2[i] = data[0][counter + offset][3];
            tempshifts[i] = shifts[0][counter + offset];
            counter++;
        }
       // cout << "\n" << tempType1[i] << endl;

    }
    
}