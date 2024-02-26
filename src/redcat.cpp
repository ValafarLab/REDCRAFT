/**
 * redcat.cpp Copyright 2004, 2006, 2008, 2009, 2010
 * Dr. Homayoun Valafar, Mikhail Simin, Zach Swearingen
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

#include "redcat.h"
#include "polypeptide.h"
#include "tensor.h"
#include "matrix.h"
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <Eigen/SVD>
#include <bits/stl_vector.h>

string Redcat::svn_id() {
    return "$Id: redcat.cpp 428 2012-12-05 21:57:31Z siminm $";
}

Redcat::Redcat(string dataPath, string rdcFilePrefix, int media) {
    mOffset = 0;
    mMediaCount = media;
    if (mMediaCount < 1) {
        cerr << "Redcat: invalid media count " << mMediaCount << ". Changed to 1" << endl;
        mMediaCount = 1;
    }
    mUnreducedRDCs = new Matrix*[mMediaCount];
    mErrors = new Matrix*[mMediaCount];

    mOTED = new double[mMediaCount];
    mOTTolerance = 1.0;
    mOTEweight = 1.0;
    museOTE = false;

    ifstream counter((dataPath + "/" + rdcFilePrefix + ".1").c_str());
    if (!counter.is_open()) {
        cerr << "could not open the file " << dataPath << "/" << rdcFilePrefix << ".1 for generating an rdc count" << endl;
        exit(1);
    }

    for (int medium = 0; medium < mMediaCount; medium++) {
        stringstream filename;
        filename << dataPath << "/" << rdcFilePrefix << "." << medium + 1;
        ifstream in(filename.str().c_str());
        vector<int> dataCountTemp;
        int residueCount = 0; //TODO Change this variable
        int dataInResidue = 0;
        int totalData = 0; //total number of lines with data
        string line;
        string lastLine;
        while (!in.eof()) {
            getline(in, line);
            if (line.empty())
                break;
            if (totalData == 1)
                lastLine = line;
            dataInResidue++;
            totalData++;
            if ((int) line.at(0) > 64 && (int) line.at(0) < 91) {
                if (!in.eof()) {
                    dataInResidue--;
                    totalData--;
                    dataCountTemp.push_back(dataInResidue);
                    residueCount++;
                    dataInResidue = 0;
                } else if (in.eof()) {
                    dataCountTemp.push_back(dataInResidue);
                    residueCount--;
                }
            }
        }
        dataCountTemp.push_back(dataInResidue);
        // dataCountTemp.erase(dataCountTemp.begin());//gets rid of the first 0 entry
        //cout << dataCountTemp.size() << endl;

        //int **mData;
        //mData = new int*[totalData];
        vector < vector <int> > mData;
        mData.reserve(totalData);
        totalDataCount.push_back(totalData);

        in.close(); //reset ifstream
        in.open(filename.str().c_str());

        mUnreducedRDCs[medium] = new Matrix(residueCount * 6, 1, 999);
        //NOTE: we use same residueCount for both media// Not any more
        mErrors[medium] = new Matrix(residueCount * 6, 1, 1);
        mOTE.push_back(NULL);



        if (!in.is_open()) {
            cerr << "could not open the file " << filename.str() << " for reading rdcs" << endl;
            exit(1);
        }

        if (lastLine.size() < 20) {
            format = "Legacy";
            for (int currentResidue = 0; currentResidue < residueCount; currentResidue++) {
                in.ignore(256, '\n'); // first line is "RES j Comments"

                for (int i = 0; i < 6; i++) {
                    in >> mUnreducedRDCs[medium]->M[currentResidue * 6 + i][0]
                       >> mErrors[medium]->M[currentResidue * 6 + i][0];
                    in.ignore(256, '\n');
                    if ((mErrors[medium]->M[currentResidue * 6 + i][0]) == 0) {
                        cerr << "The margin error is 0 at RDC value: " << mUnreducedRDCs[medium]->M[currentResidue * 6 + i][0] << ". Residue " << currentResidue << ", vector " << i << endl;
                    }
                }
            }
        } else {
            format = "NEF";
            //int placeholder = -1;
            for (int currentResidue = 0; currentResidue < residueCount; currentResidue++) {
                in.ignore(256, '\n');

                string RDCString, errorString, trash;
                for (int i = 0; i < dataCountTemp[currentResidue + 1]; i++) {
                    vector <int> atomInfo;
                    // placeholder++;
                    //mData[placeholder] = new int[4];
                    /* in >> trash1 >> trash2 >> mUnreducedRDCs[medium]->M[currentResidue * 6 + i][0]
                             >> mErrors[medium]->M[currentResidue * 6 + i][0];*/

                    for (int j = 0; j < 8; j++) {
                        in>>trash;
                        if (j == 0)
                            //  mData[placeholder][0] = atof(trash.c_str());
                            atomInfo.push_back(atof(trash.c_str()));
                        if (j == 2)
                            // mData[placeholder][1] = map(trash);
                            atomInfo.push_back(map(trash));
                        if (j == 3)
                            // mData[placeholder][2] = atof(trash.c_str());
                            atomInfo.push_back(atof(trash.c_str()));
                        if (j == 5)
                            //  mData[placeholder][3] = map(trash);
                            atomInfo.push_back(map(trash));
                        if (j == 6)
                            RDCString = trash;
                        if (j == 7) {
                            errorString = trash;
                        }
                    }
                    //REPLACE THESE 6s
                    mUnreducedRDCs[medium]->M[currentResidue * 6 + i][0] = strtod(RDCString.c_str(), NULL); //string to double
                    mErrors[medium]->M[currentResidue * 6 + i][0] = strtod(errorString.c_str(), NULL);
                    in.ignore(256, '\n');

                    if ((mErrors[medium]->M[currentResidue * 6 + i][0]) == 0) {
                        cerr << "The margin error is 0 at RDC value: " << mUnreducedRDCs[medium]->M[currentResidue * 6 + i][0] << ". Residue " << currentResidue << ", vector " << i << endl;
                    }
                    mData.push_back(atomInfo);
                }

            }

        }
        if (medium == mMediaCount - 1) {
            mResidueIndices = new int*[mMediaCount];
            for (int medium = 0; medium < mMediaCount; medium++) {
                mResidueIndices[medium] = new int[residueCount];
                int currentIndex = 0;
                int residueIndex = 0;
                for (int rdc = 0; rdc < residueCount * 6; rdc++) {
                    if (rdc != 0 && rdc % 6 == 0)
                        mResidueIndices[medium][residueIndex++] = currentIndex;
                    if (mUnreducedRDCs[medium]->M[rdc][0] < 998)
                        currentIndex++;
                }
                mResidueIndices[medium][residueCount - 1] = currentIndex;
            }
        }
        if (format == "NEF") {
            data.push_back(mData);
            dataCount.push_back(dataCountTemp);
            vector <int> shift;
            shift.reserve(totalData);
            for (int i = 0; i < totalDataCount[medium]; i++) {
                shift.push_back(data.at(medium).at(i).at(0) - data.at(medium).at(i).at(2));
            }
            shifts.push_back(shift);
        }
    }

}

Redcat::~Redcat() {
    for (int medium = 0; medium < mMediaCount; medium++) {
        delete mUnreducedRDCs[medium];
        delete mErrors[medium];
        delete [] mResidueIndices[medium];
        delete mOTE[medium];

    }
    delete [] mUnreducedRDCs;
    delete [] mErrors;
    delete [] mResidueIndices;
    delete [] mOTED;
}

double Redcat::calculateRMSD(Polypeptide & poly) {
    poly.setVectors(data, totalDataCount, dataCount, shifts);
    //cerr << "redcat::calcrmsd() pp.rescount:" << poly.getResidueCount() << endl;
    double total = 0;
    int rdcs = 0;
    Matrix * ots[mMediaCount];
    vector<Tensor*> computed(mMediaCount);
    vector<Tensor*> input(mMediaCount);
    for (int medium = 0; medium < mMediaCount; medium++) {
        ots[medium] = new Matrix(5, 1);
        total += pow(calculateRMSD(poly, medium, ots[medium]), 2) * getReducedRDCCount(medium, poly.getResidueCount());
        rdcs += getReducedRDCCount(medium, poly.getResidueCount());
        // cout<<"calcRMSD squared "<<pow(calculateRMSD(poly, medium, ots[medium]), 2)<<" reduecedRDCcount "<<getReducedRDCCount(medium, poly.getResidueCount())<<endl;
    }
    // cout<<"total "<<total<<endl;
    //cout<<"rdcs "<<rdcs<<endl;
    double rmsd = sqrt(total / (double) rdcs);

    // do all stuff to modify the score
    double OTEdiff = 0.0;

    if (museOTE) {
        //cout << "museOTE = true" << endl;
        for (int m = 0; m < mMediaCount; m++) {
            if (mOTE[m] == NULL) {
                // Estimation not defined for this OT
                continue;
            }
            input[m] = new Tensor(*(mOTE[m]));
            computed[m] = new Tensor(*(ots[m]));
        }

        vector<Tensor*> cComputed = canonicalize_tensors(computed);
        vector<Tensor*> cInput = canonicalize_tensors(input);

        for (int m = 0; m < mMediaCount; m++) {
            double dist = cInput[m]->otmDistance(*(cComputed[m]));
            //cout << "Distance between " << endl << (*cInput[m]) << "and" << endl << (*cComputed[m]) << endl << dist << endl;
            OTEdiff += pow(dist, 2);
            delete input[m];
            delete cInput[m];
            delete cComputed[m];
        }
        OTEdiff = sqrt(OTEdiff / mMediaCount);
        //cout << "DMax(inner): " << dMax << endl;
        OTEdiff *= dMax; //dMax is either set in the default.conf file or defaulted to 23630 (Dmax/r3 for N-H assuming r = 1.01)
        OTEdiff -= mOTTolerance;
        if (OTEdiff < 0) OTEdiff = 0;
        OTEdiff *= mOTEweight;
    }

    for (int m = 0; m < mMediaCount; m++) {
        delete computed[m];
        delete ots[m];
    }
    //CHECK TO SEE IF OTE IS YOUR PROBLEM IN THE rmsd
    //cout << "OTEdiff " << OTEdiff << endl;
    double test = rmsd + OTEdiff;
    //cout<<" "<<test<<endl;
    return rmsd + OTEdiff;
}

void Redcat::getRDCDiff(Polypeptide &poly, int medium, double *x, int &start) {
    poly.setVectors(data, totalDataCount, dataCount, shifts);

    Matrix unreducedA = poly.getAMatrix();

    int aRows = getReducedRDCCount(medium, poly.getResidueCount());
    Matrix a(aRows, 5);
    Matrix backCompRDCs(a.GetRow(), 1);
    Matrix reducedRDCs(a.GetRow(), 1);
    Matrix reducedErrors(a.GetRow(), 1);

    int coordWriteIndex = 0;
    int URO = mOffset * 6; // unreduced RDC offset
    for (int i = 0; i < unreducedA.GetRow(); i++) {
        if (
                i + URO < mUnreducedRDCs[medium]->GetRow()
                && mUnreducedRDCs[medium]->M[i + URO][0] < 998
                && i != 5
                && i != unreducedA.GetRow() - 3
                && i != unreducedA.GetRow() - 2) {
            reducedRDCs.M[coordWriteIndex][0] = mUnreducedRDCs[medium]->M[i + URO][0];
            reducedErrors.M[coordWriteIndex][0] = mErrors[medium]->M[i + URO][0];

            a.M[coordWriteIndex][0] = unreducedA.M[i][0];
            a.M[coordWriteIndex][1] = unreducedA.M[i][1];
            a.M[coordWriteIndex][2] = unreducedA.M[i][2];
            a.M[coordWriteIndex][3] = unreducedA.M[i][3];
            a.M[coordWriteIndex][4] = unreducedA.M[i][4];

            coordWriteIndex++;
        }
    }
    /*
    eigen::MatrixXd eigen_a = a.toEigen();
    eigen::MatrixXd eigen_rdc(reducedRDCs.toEigen());
    eigen::MatrixXd eigen_ot(5,1);
    eigen::JacobiSVD<eigen::MatrixXd> svd(eigen_a, eigen::ComputeThinU | eigen::ComputeThinV);
    eigen_ot = svd.solve(eigen_rdc);
    eigen::MatrixXd eigen_bcrdc(eigen_a * eigen_ot);

    backCompRDCs = Matrix(eigen_bcrdc);
     */
    bool inverse_failed = true;
    Matrix * orderTensor = new Matrix(5, 1);
    if (a.GetRow() >= 5) { // determined -- use left inverse
        Matrix preInverse(5, 5);
        preInverse = a.Transpose() * a;
        Matrix *inverse = invertSquareMatrix(&preInverse);
        orderTensor->operator=(*inverse * a.Transpose() * reducedRDCs);
        delete inverse;

        inverse_failed = isnan(orderTensor->M[0][0]) || isnan(orderTensor->M[1][0]) ||
                         isnan(orderTensor->M[2][0]) || isnan(orderTensor->M[3][0]) ||
                         isnan(orderTensor->M[4][0]);

        if (!inverse_failed) {
            backCompRDCs = a * *orderTensor;
        }

    }
    if (inverse_failed || a.GetRow() < 5) { // underdetermined -- use SVD
        Eigen::MatrixXd eigen_a = a.toEigen();
        Eigen::MatrixXd eigen_rdc(reducedRDCs.toEigen());
        Eigen::MatrixXd eigen_ot(5, 1);

        Eigen::JacobiSVD<Eigen::MatrixXd> svd(eigen_a, Eigen::ComputeThinU | Eigen::ComputeThinV);
        eigen_ot = svd.solve(eigen_rdc);

        orderTensor->operator=(eigen_ot);

        Eigen::MatrixXd eigen_bcrdc(eigen_a * eigen_ot);

        backCompRDCs = Matrix(eigen_bcrdc);
    }
    delete orderTensor;

    for (int rdc = 0; rdc < reducedRDCs.GetRow(); rdc++) {
        x[start + rdc] = sqrt(pow(reducedRDCs.M[rdc][0] - backCompRDCs.M[rdc][0], 2) * reducedErrors.M[rdc][0]);
    }
    start += reducedRDCs.GetRow();

}

double Redcat::calculateRMSD(Polypeptide &poly, int medium, Matrix * orderTensor) {
    poly.setVectors(data, totalDataCount, dataCount, shifts);

    int aRows = getReducedRDCCount(medium, poly.getResidueCount());
    Matrix *a = new Matrix(aRows, 5);
    Matrix *unreducedA = new Matrix(poly.getAMatrix());
    /* cout.precision(std::numeric_limits<double>::digits10);
     cout << "\nGETAMATRIX" << endl;
     poly.getAMatrix().Print();
     cout << "\n-------------" << endl;
     cout << "\nPOLYPEPTIDE" << endl;
     for(int w=0;w<poly.getAminoAcid(1)->getAtomCount();w++){
         poly.getAminoAcid(1)->getAtom(w).print();
     }
     cout << "\n-------------" << endl;*/
    Matrix *reducedRDCs = new Matrix(a->GetRow(), 1);
    Matrix *reducedErrors = new Matrix(a->GetRow(), 1);
    populateWorkMatricies(medium, unreducedA, a, reducedRDCs, reducedErrors);
    Matrix *backCompRDCs = calculateBCRDCFromReduced(a, reducedRDCs, orderTensor);
    double rmsd = calculateRMSD(reducedRDCs, backCompRDCs, reducedErrors);
    // cout << rmsd << endl;
    delete a;
    delete unreducedA;
    delete reducedRDCs;
    delete reducedErrors;
    delete backCompRDCs;

    return rmsd;

}

void Redcat::populateWorkMatricies(int medium, Matrix* unreducedA, Matrix * a, Matrix * reducedRDCs, Matrix * reducedErrors) {
    // cout.precision(std::numeric_limits<double>::digits10);
    /* for(int i=0;i<mMediaCount;i++){
         cout<<"\nUnreducedRDCs"<<endl;
         mUnreducedRDCs[i]->Print();
     }*/
    int coordWriteIndex = 0;
    int URO = mOffset * 6; // unreduced RDC offset
    int unreducedAStop = (unreducedA->GetRow() > mUnreducedRDCs[medium]->GetRow()) ?
                         mUnreducedRDCs[medium]->GetRow() : unreducedA->GetRow();

    for (int i = 0; i < unreducedAStop; i++) //After this, a, reducedRDCs, and reducedErrors is populated
    {
        if (
                (i + URO) < mUnreducedRDCs[medium]->GetRow()
                && mUnreducedRDCs[medium]->M[i + URO][0] < 998
                && i != 5
                && i != unreducedA->GetRow() - 3
                && i != unreducedA->GetRow() - 2) {
            reducedRDCs->M[coordWriteIndex][0] = mUnreducedRDCs[medium]->M[i + URO][0];
            reducedErrors->M[coordWriteIndex][0] = mErrors[medium]->M[i + URO][0];

            a->M[coordWriteIndex][0] = unreducedA->M[i][0];
            a->M[coordWriteIndex][1] = unreducedA->M[i][1];
            a->M[coordWriteIndex][2] = unreducedA->M[i][2];
            a->M[coordWriteIndex][3] = unreducedA->M[i][3];
            a->M[coordWriteIndex][4] = unreducedA->M[i][4];
            coordWriteIndex++;
        }

    }
    /* cout << "\nA MATRIX and unreducedA" << endl;
     a->Print();
     cout<<"\n";
     unreducedA->Print();
    // mUnreducedRDCs[medium]->Print();
     //cout << "\n-------------" << endl;
     /**/
}

Matrix * Redcat::calculateBCRDCFromReduced(Matrix * a, Matrix * reducedRDCs, Matrix * orderTensor) {
    /*cout << "\nBACK CALCULATED RDC MATRICES" << endl;
    a->Print();
    reducedRDCs->Print();*/
    if (a->GetRow() == 0) { // No RDCs are available
        Matrix * backCompRDCs = new Matrix(0, 0);
        // No action is done to orderTensor -- it's initialized before being sent here.
        return backCompRDCs;
    }
    bool inverse_failed = true;
    if (a->GetRow() >= 5) // determined -- use left inverse
    {
        //cout<<"\nLeft Inverse"<<endl;
        Matrix preInverse(5, 5);

        /* cout<<"A Transpose"<<endl;
          Matrix   ATranspose =  a->Transpose();
        ATranspose =a->Transpose();
         ATranspose.Print();
         /**/
        preInverse = a->Transpose() * *a;
        /* cout<<"preInverse "<<endl;
         preInverse.Print();/**/
        Matrix *inverse = invertSquareMatrix(&preInverse);
        /*cout<<"Inverse"<<endl;
         inverse->Print();/**/
        orderTensor->operator=(*inverse * a->Transpose() * *reducedRDCs);
        /* cout<<"\nOrderTensor"<<endl;
         orderTensor->Print();
        cout << "\n" << endl;/**/
        delete inverse;

        inverse_failed = isnan(orderTensor->M[0][0]) || isnan(orderTensor->M[1][0]) ||
                         isnan(orderTensor->M[2][0]) || isnan(orderTensor->M[3][0]) ||
                         isnan(orderTensor->M[4][0]);

        if (!inverse_failed) {

            Matrix * backCompRDCs = calculateBCRDCFromOT(a, orderTensor);
            return backCompRDCs;
        }

    }
    if (inverse_failed || a->GetRow() < 5) { // underdetermined -- use SVD
        // cout<<"\nSVD"<<endl;
        Eigen::MatrixXd eigen_a = a->toEigen();
        Eigen::MatrixXd eigen_rdc(reducedRDCs->toEigen());

        /* Matrix * testEigen_a = new Matrix(eigen_a);
         Matrix * testEigenRDCs = new Matrix(eigen_rdc);
         cout<<"\nEigen A"<<endl;
         testEigen_a->Print();
         cout<<"\nEigen RDC "<<endl;
         testEigenRDCs->Print();
          /**/
        Eigen::MatrixXd eigen_ot(5, 1);

        Eigen::JacobiSVD<Eigen::MatrixXd> svd(eigen_a, Eigen::ComputeThinU | Eigen::ComputeThinV);
        /**/

        /**/
        eigen_ot = svd.solve(eigen_rdc);

        orderTensor->operator=(eigen_ot);
        /* cout<<"\nOrderTensor "<<endl;
         orderTensor->Print();/**/
        Eigen::MatrixXd eigen_bcrdc(eigen_a * eigen_ot);

        Matrix * backCompRDCs = new Matrix(eigen_bcrdc);
        //  cout << "\nBACK COMP FROM RDC" << endl;
        /*  backCompRDCs->Print();
          cout << "\n" << endl;/**/
        return backCompRDCs;
    }
    return nullptr;
}

Matrix * Redcat::calculateBCRDCFromOT(Matrix * a, Matrix * orderTensor) {
    Matrix * backCompRDCs = new Matrix(a->GetRow(), 1);
    *backCompRDCs = *a * *orderTensor;
    //  cout << "\nBACK COMP FROM OT" << endl;
    /*backCompRDCs->Print();
    cout << "\n" << endl;/**/
    return backCompRDCs;
}

double Redcat::calculateRMSD(Matrix * reducedRDCs, Matrix * backCompRDCs, Matrix * reducedErrors) {
    double rmsd = 0;
    for (int rdc = 0; rdc < reducedRDCs->GetRow(); rdc++) {
        rmsd += (pow(reducedRDCs->M[rdc][0] - backCompRDCs->M[rdc][0], 2)) * reducedErrors->M[rdc][0];
        /*  cout.precision(std::numeric_limits<double>::digits10);
          cout << reducedRDCs->M[rdc][0] << endl;
          cout << backCompRDCs->M[rdc][0] << endl;
          cout << reducedRDCs->M[rdc][0] - backCompRDCs->M[rdc][0] << endl;
          cout << reducedErrors->M[rdc][0] << endl;
          cout << "*" << rmsd << "*" << endl;/**/

    }
    /* cout<<"\nreducedRDCs"<<endl;
     reducedRDCs->Print();
     cout<<"\nbackcompRDCs"<<endl;
     backCompRDCs->Print();
    /* cout<<"\nreducedErrors"<<endl;
     reducedErrors->Print();/**/

    return sqrt(rmsd / (double) reducedRDCs->GetRow());
}

int Redcat::getReducedRDCCount(int medium, int residue) {
    if (residue < 1) return 0;
    residue += mOffset;
    int count = 0;
    int last = residue * 6; // note: residue is actually (residue - 1 + 1); -1 for array index, +1 for one more dummy residue
    //if (last > mUnreducedRDCs[medium]->GetRow()) last = mUnreducedRDCs[medium]->GetRow();

    for (int rdc = (mOffset * 6); rdc < last; rdc++) {
        if (rdc != 5 + (mOffset * 6)
            && rdc != last - 2
            && rdc != last - 3
            && rdc < mUnreducedRDCs[medium]->GetRow()
            && mUnreducedRDCs[medium]->M[rdc][0] < 998) {
            count++;
        }
    }
    //cout << "count: "  << count << endl; exit(1);
    return count;
}

void Redcat::setOTE(int m, const Matrix & ot) {
    if (m > mMediaCount) {
        cerr << "Redcat::setOTE medium out of bounds." << m << " > " << mMediaCount << endl;
        exit(1);
    }
    mOTE[m] = new Tensor(ot);
    if (m != 0 && mOTE[0] != NULL) {
        mOTED[m] = mOTE[m]->otmDistance(*mOTE[0]);
    }
}

void Redcat::createRedcatFile(Polypeptide &pp, const char *filename, int medium) {
    pp.setVectors(data, totalDataCount, dataCount, shifts);

    long int maxValues[] = {6125, 24350, -60400, -60400, -240200, -240200};
    /*for (int res = 0; res < pp.getResidueCount(); res++) {
         cout << "Amino Acid\n------------" << endl;
         pp.getAminoAcid(res)->Print();
       
     }
      cout << "\nPolypeptide\n------------" << endl;
         pp.Print();*/
    string comments1[] = {"/* C from residue ",
                          "/* N from residue ",
                          "/* C from residue ",
                          "/* Ca from residue ",
                          "/* Ha from residue ",
                          "/* Ha from residue "};
    string comments2[] = {" to N from residue ",
                          " to H from residue ",
                          " to H from residue ",
                          " to Ha from residue ",
                          " to H from residue ",
                          " to H from residue "};
    if (format == "NEF") {
        Matrix coords = pp.getCoordinates(data, dataCount, totalDataCount, shifts, medium);
        // coords.Print();
        int lineOffset = 0;

        for (int i = 0; i <= mOffset; i++) {
            lineOffset += dataCount[medium][i];
        }
        ofstream out(filename);
        int coordsStop = (coords.GetRow() > mUnreducedRDCs[medium]->GetRow()) ?
                         mUnreducedRDCs[medium]->GetRow() : coords.GetRow();


        for (int i = 0; i < coordsStop; i++) {
            int io = i + lineOffset;
            if (io >= mUnreducedRDCs[medium]->GetRow()) continue;
            // int vectorStartResidue = (i % 6 == 0 || i % 6 == 2 || i % 6 == 5) ? i / 6 : (i / 6) + 1;
            //vectorStartResidue += mOffset;
            out <<
                coords.M[i][0] << " " << coords.M[i][1] << " " << coords.M[i][2] << " " << coords.M[i][3] << " " << coords.M[i][4] << " " << coords.M[i][5]
                << " " <<
                mUnreducedRDCs[medium]->M[io][0] << " " << mapMaxValue(data[medium][i][1], data[medium][i][3]) << " " << mErrors[medium]->M[io][0] << " " <<
                getFirstComment(data[medium][i][1]) << data[medium][i][0] << getSecondComment(data[medium][i][3]) << data[medium][i][2] << " */" << endl;

        }
    } else {
        Matrix coords = pp.getCoordinates();

        ofstream out(filename);
        int coordsStop = (coords.GetRow() > mUnreducedRDCs[medium]->GetRow()) ?
                         mUnreducedRDCs[medium]->GetRow() : coords.GetRow();
        for (int i = 0; i < coordsStop; i++) {
            int io = i + (mOffset * 6);
            if (io >= mUnreducedRDCs[medium]->GetRow()) continue;
            int vectorStartResidue = (i % 6 == 0 || i % 6 == 2 || i % 6 == 5) ? i / 6 : (i / 6) + 1;
            vectorStartResidue += mOffset;
            out <<
                coords.M[i][0] << " " << coords.M[i][1] << " " << coords.M[i][2] << " " << coords.M[i][3] << " " << coords.M[i][4] << " " << coords.M[i][5]
                << " " <<
                mUnreducedRDCs[medium]->M[io][0] << " " << maxValues[i % 6] << " " << mErrors[medium]->M[io][0] << " " <<
                comments1[i % 6] << vectorStartResidue << comments2[i % 6] << (io / 6) + 1 << " */" << endl;

        }
    }
}

/*void Redcat::fillAtomsVector(vector<int> &a, vector<int> &b) {
    mAtomNames1 = a;
    mAtomNames2 = b;
}*/
int Redcat::map(string atom) {

    if (atom == "C") {
        return AminoAcid::C;
    } else if (atom == "N") {
        return AminoAcid::N;
    } else if (atom == "CA") {
        return AminoAcid::Ca;
    } else if (atom == "C") {
        return AminoAcid::C;
    } else if (atom == "H") {
        return AminoAcid::H;
    } else if (atom == "HA") {
        return AminoAcid::Ha; //Ha2 to match legacy
    }
    return -1; //should not return -1
}

int Redcat::mapMaxValue(int a, int b) {
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

string Redcat::getFirstComment(int a) {
    string comments1[] = {"/* C from residue ",
                          "/* N from residue ",
                          "/* C from residue ",
                          "/* Ca from residue ",
                          "/* Ha from residue ",
                          "/* Ha from residue ",
                          "/* H from residue "};
    if (a == AminoAcid::C) {
        return comments1[0];
    } else if (a == AminoAcid::N || a == AminoAcid::N2) {
        return comments1[1];
    } else if (a == AminoAcid::Ca || a == AminoAcid::Ca2) {
        return comments1[3];
    } else if (a == AminoAcid::Ha || a == AminoAcid::Ha2) {
        return comments1[4];
    } else if (a == AminoAcid::H || a == AminoAcid::H2) {
        return comments1[6];
    }
    return "/*Unknown Atom from residue ";
}

string Redcat::getSecondComment(int a) {
    string comments2[] = {" to N from residue ",
                          " to H from residue ",
                          " to H from residue ",
                          " to Ha from residue ",
                          " to Ca from residue ",
                          " to H from residue "};

    if (a == AminoAcid::H || a == AminoAcid::H2) {
        return comments2[1];
    } else if (a == AminoAcid::N || a == AminoAcid::N2) {
        return comments2[0];
    } else if (a == AminoAcid::Ha || a == AminoAcid::Ha2) {
        return comments2[3];
    } else if (a == AminoAcid::Ca || a == AminoAcid::Ca2) {
        return comments2[4];
    }
    return "to Unknown Atom from residue ";
}
