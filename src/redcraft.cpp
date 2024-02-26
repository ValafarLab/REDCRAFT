/**
 * redcraft.cpp Copyright 2004, 2006, 2008, 2009, 2010
 * Dr. Homayoun Valafar, Mikhail Simin, Zach Swearingen, Mike
 * Bryson, Paul Shealy, Dalton Brown.
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

#include "redcraft.h"
#include "aminoacid.h"
#include "polypeptide.h"
#include "rude_config/config.h"
#include "cluster_collection.h"
#include "functions.cpp"

#include <time.h>
#include <bits/basic_string.h>
//#include <typeinfo>
#ifdef _OPENMP
#include <omp.h>
#endif

string Redcraft::svn_id() {
    return "$Id: redcraft.cpp 423 2012-11-15 20:29:11Z siminm $";
}


double gWorstScore = 0.0;

Redcraft::Redcraft(string confFile) {
    rude::Config conf;

    //these are guaranteed to be ok because we checked them in main.
    conf.load(confFile.c_str());
    conf.setSection("Run_Settings");

    mStartResidue = conf.getIntValue("Start_Residue");
    mStopResidue = conf.getIntValue("Stop_Residue");
    mMediaCount = conf.getIntValue("Media_Count");
    mDataPath = conf.getStringValue("Data_Path");
    mRDCFilePrefix = conf.getStringValue("RDC_File_Prefix");
    mDefaultDepth = conf.getIntValue("Default_Search_Depth");
    mLJEnabled = conf.exists("LJ_Threshold");
    mLJScoreThreshold = conf.getDoubleValue("LJ_Threshold");
    mCurrentResidue = mStartResidue;
    mCurrentDepth = mDefaultDepth;
    mConfigFile = confFile;
    mFormat = conf.getStringValue("RDC_File_Format");
    mSequence = conf.exists("Chain");
    if (mSequence) {
        mSeqString = conf.getStringValue("Chain");
    }
    if(conf.setSection("Depth_Search_Settings")){
        /* cout<<conf.getNumDataMembers()<<endl;
         cout<<conf.getDataNameAt(0)<<endl;*/
        mSearchDepthSize=conf.getNumDataMembers();
        if(mSearchDepthSize>0){
            mSearchDepth=new int*[conf.getNumDataMembers()];
            for (int i =0;i<conf.getNumDataMembers();i++){
                mSearchDepth[i]=new int[2];
                string line = conf.getDataNameAt(i);
                int j=line.find_last_of('_');
                int w=line.find_last_of('=');
                int diff=w-j;
                string res=line.substr(j+1,diff);
                int newDepth=conf.getIntValue(line.c_str());
                mSearchDepth[i][0]=atoi(res.c_str());
                mSearchDepth[i][1]=newDepth;
                //cout<<res<<" "<<newDepth<<endl;
                //mSearchDepth[i][0]=
            }
        }
    }


    if (conf.setSection("Decimation_Settings")) {
        mDecimationEnabled = true;

        if (conf.exists("Decimation_Ranges")) {
            if (conf.exists("Cluster_Count")) //If the user wants to set a target number of clusters
            {
                mClusterCount = conf.getIntValue("Cluster_Count");
                //cout << "Cluster_Count " << mClusterCount << endl;
                mClusterGranularity = 1.0; //This is being set to one every time right now for use in line 85
                userEnteredClusterCount = true;
            } else if (conf.exists("Cluster_Sensitivity")) //Instead of target number, the user enters a sensitivity.
            {
                mClusterGranularity = conf.getDoubleValue("Cluster_Sensitivity");
                userEnteredClusterCount = false;
            } else {
                cerr << "This should have been caught in stage2's error checking." << endl;
            }
        } else {
            mDecimationEnabled = false;
        }

        if (mDecimationEnabled) {
            mDecimationScoreThreshold = conf.getDoubleValue("Score_Threshold");
            //cout<<mDecimationScoreThreshold<<endl;
            if (conf.exists("Decimation_Ranges"))
                createMask(conf.getStringValue("Decimation_Ranges"), &mDecimationMask, mStopResidue, mStartResidue);
            else
                createMask((toString(mStartResidue).append("-").append(toString(mStopResidue))).c_str(), &mDecimationMask, mStopResidue, mStartResidue);
            if(conf.exists("Maximum_Number_of_Additional_Structures")){
                mMaxStructures = conf.getIntValue("Maximum_Number_of_Additional_Structures");
                if(mMaxStructures<1){
                    mMaxStructures=-1;
                }
            }
        }

    }

    conf.setSection("Run_Settings");

    mRedcat = new Redcat(mDataPath, mRDCFilePrefix, mMediaCount);

    mCluster = new ClusterCollection(1.0 / mClusterGranularity, (mStartResidue > mStopResidue));

    mAngles = new TStorage<double, Polypeptide*>;
    mAnglesBuffer = new TStorage<double, Polypeptide*>;

    if (strcmp(conf.getStringValue("Run_Type"), "new") == 0) {
        mRunType = kNewRun;
        if (mStartResidue > 1) {
            mRedcat->offset(mStartResidue - 1);
        }
    } else {
        mRunType = 1;
    }

    if (conf.setSection("OTEstimation", false)) {
#ifndef STDTENSOR_H
        cerr << "Warning: Provided OrderTensor estimation, but Tensor class is not loaded." << endl;
#else
        string tempOT; // for storing the config entry
        double dij;
        Matrix myOT(5, 1); // for temp storage of OT parsed from tempOT
        for (int m = 0; m < mMediaCount; m++) {
            string myS = "S"; // itterator S1, S2 etc..
            myS.insert(1, toString(m + 1));
            if (!conf.exists(myS.c_str())) continue;
            tempOT = conf.getValue(myS.c_str()); // config value
            stringstream ij; // for parsing tempOT as 5 tokens
            ij << tempOT; // take all 5 values

            // Config file requires these to be in correct order
            // Sxx Syy Sxy Sxz Syz
            for (int q = 0; q < 5; q++) {
                ij >> dij; // treat as tokens into double
                myOT.Set_Mij(q, 0, dij);
            }
            mRedcat->setOTE(m, myOT);
        }
        if (conf.exists("Tolerance"))
            mRedcat->OTTolerance(conf.getDoubleValue("Tolerance"));
        if (conf.exists("Weight"))
            mRedcat->OTweight(conf.getDoubleValue("Weight"));

        if (conf.exists("Dmax"))
            mRedcat->setDMax(conf.getDoubleValue("Dmax"));
        else
            mRedcat->setDMax(23630);


        if (conf.exists("Estimation_Range")) {
            createMask(conf.getStringValue("Estimation_Range"), &mOTEstimationMask, mStopResidue, mStartResidue);
        } else if (conf.exists("S1")) {
            createMask((toString(mStartResidue).append("-").append(toString(mStopResidue))).c_str(), &mOTEstimationMask, mStopResidue, mStartResidue);
        } else {
            createMask("0-0", &mOTEstimationMask, mStopResidue, mStartResidue);
        }
#endif
    }

    fillResidueNames();
    //mRedcat->fillAtomsVector(mAtomNames1, mAtomNames2);



#ifdef _OPENMP
    //if(omp_get_dynamic())
    //	omp_set_dynamic(0);

    cout << "*** Parallel mode. Max Threads: " << omp_get_max_threads() << endl;

#endif

}

Redcraft::~Redcraft() {
    delete mRedcat;
    TStorageNode<double, Polypeptide*> *node = mAngles->GetFirst();
    while (node != NULL) {
        delete node->GetData();
        node = node->GetNext();
    }
    for(int i =0;i<mSearchDepthSize;i++){
        delete [] mSearchDepth[i];
    }
    //delete [] mSearchDepth;
    delete mAngles;
    delete mAnglesBuffer;
    delete mCluster;
}

void Redcraft::run() {


    if (mRunType == kNewRun) {
        readNextAngles();
        int limit = mNextAngles.size(); //(mNextAngles.size() > mDefaultDepth)?mDefaultDepth:mNextAngles.size();
        for (int currentAngles = 0; currentAngles < limit; currentAngles++) {
            Polypeptide *structure = new Polypeptide();
            structure->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

            structure->id = currentAngles;
            structure->appendAminoAcid(mNextAngles[currentAngles][0], mNextAngles[currentAngles][1], getResidueNameAt(mCurrentResidue));
            structure->appendAminoAcid(180, 180, getResidueNameAt(mCurrentResidue + 1));
            mAngles->Add(0.0, structure);
        }
        mCurrentResidue++;
        cout << "mRunType == kNewRun" << endl;
        //  mCurrentResidue++;

    }
    else {
        stringstream lastAngleFileName;
        lastAngleFileName << mDataPath << "/" << mCurrentResidue - 1 << ".out";
        ifstream lastAngleFile(lastAngleFileName.str().c_str());

        if (!lastAngleFile.is_open()) {
            cerr << "could not read from the file " << mCurrentResidue - 1 << ".out" << endl;
            exit(1);
        }

        cout << "Reading " << mCurrentResidue - 1 << ".out file...\r" << flush;
        char angleCountLine[4096];
        lastAngleFile.getline(angleCountLine, 4096); // get the first line to figure out the start
        int angleCount = 0;
        int angleCountIndex = 0;

        while (angleCountLine[angleCountIndex] != '\0') { // loop through each character
            if (angleCountLine[angleCountIndex] == ' ') // count number of spaces (angles)
                angleCount++;
            angleCountIndex++;
        }

        mRedcat->offset(((mCurrentResidue - 1)*2 - angleCount) / 2);
        cout << endl << "Residue offset: " << mRedcat->offset() << endl;
        // TODO: I'm going to use mRedcat->offset() to retreive the
        // number calculated here when we come to refinement ADD
        // this needs to be fixed so that i'm not relying on REDCAT
        // to hold this temporary data
        // maybe we can change mStartResidue to the offset+1


        lastAngleFile.close();



        lastAngleFile.open(lastAngleFileName.str().c_str());
        while (!lastAngleFile.eof()) { // read angles from previous file (start from 5, read from 4.out)
            double angles[angleCount];
            for (int angle = 0; angle < angleCount; angle++) {
                lastAngleFile >> angles[angle];
                //cout << "read angle " << angles[angle] << endl;
            }
            double rmsd;
            lastAngleFile >> rmsd;
            if (lastAngleFile.eof()) break;
            Polypeptide *pp = new Polypeptide();
            pp->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

            pp->setAngles(angles, angleCount);

            pp->id = mAngles->GetCount();
            pp->appendAminoAcid();

            for (int q = 1; q <= pp->getResidueCount(); q++)
                pp->getAminoAcid(q - 1)->setType(getResidueNameAt(q));
            mAngles->Insert(rmsd, pp);
            //cout << "Tree size: " << mAngles->GetCount() << endl;
        }
        //delete mAngles->GetLast()->GetData();
        //mAngles->Delete(mAngles->GetLast());
    } //End Else


    rude::Config conf;
    conf.load(mConfigFile.c_str());

    long unsigned int startTime = time(0);


    bool stopHammerTime = false;
    bool reverse = false;
    if (mCurrentResidue > mStopResidue) {
        mCurrentResidue -= 2;
        mRedcat->offset(mCurrentResidue - 1); //Decrement the offset once here then every iteration in the while loop below if reverse.
        reverse = true;
        cout << "Running in reverse configuration." << endl;
    }

    while (stopHammerTime != true) {
        gWorstScore = 0.0;

        print_state();

        cout << "Reading next angles...\r" << flush;
        readNextAngles();
        cout << "Creating Structures..." << endl;
        createStructures();
        //cout << "                      \r" << flush;
        cout << "Function CreateStructure Completed" << endl;
        create_output_files();

        if (conf.setSection("Refinement")) {
            // for each of the scripts we push the output into a file then work with the file
            // maybe a better approach(?) could be that we open a stream to the script then
            // push out angles one line at a time as STDIN
            // that way if a choice of angles is rejected by refinement, it can return REJECT and
            // the loop will delete it from the queue
            int sid = 1;
            string temp = "script" + toString(sid);
            while (conf.exists(temp.c_str())) {
                string script = conf.getStringValue(temp.c_str());
                cout << "Refinement: " << script << " " << flush;
                script += " " + toString(mCurrentResidue) + " > redcraft_refinement_script.out";
                cout << "script: " << script << endl;
                if (!system(script.c_str())) { // no error
                    ifstream sout("redcraft_refinement_script.out");
                    string action;
                    Polypeptide copy(*(mAngles->GetFirst()->GetData())); // copying for residue names
                    copy.setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

                    int added = 0;
                    while (true) {
                        //cerr << "Parsing results" << endl;
                        sout >> action;
                        if (sout.eof()) {
                            //cerr << "No more text" << endl;
                            break;
                        }
                        //cerr << "received action: " << action << endl;
                        double phi, psi, score;
                        Polypeptide *pp = new Polypeptide(copy); // copying for residue names
                        pp->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

                        if (!action.compare("ADD")) {
                            //cerr << "received ADD from " << script.c_str() << endl;
                            int rescount = mCurrentResidue - mRedcat->offset();
                            //The following if-else was added by HV on 3/17/14 to fix a problem with reverse folding.
                            if (mStopResidue > mStartResidue) {
                                rescount = mCurrentResidue - mStartResidue + 1;
                            } else {
                                rescount = mStartResidue - mCurrentResidue + 1;
                            }

                            /*if (mRunType != kNewRun) { // we are continuing a previous run, so mStartResidue is incorrect
                              rescount = mCurrentResidue;
                              }*/
                            //cerr << "Adding ";
                            for (int i = 0; i < rescount; i++) {
                                sout >> phi;
                                sout >> psi;
                                //	cerr << phi << " " << psi << " ";
                                pp->setAngles(i, phi, psi, pp->getAminoAcid(i)->getOmega());
                            }
                            sout >> score;
                            //cerr << score << endl;
                            mAngles->Insert(score, pp);
                            added++;
                            //cerr << "+";
                        } else if (!action.compare("TRUNCATE")) {
                            cout << "T" << flush;
                            //cerr << "received TRUNCATE from " << script.c_str() << endl;
                            TStorageNode<double, Polypeptide*> * node = mAngles->GetFirst();
                            while (node != NULL) {
                                delete node->GetData();
                                node = node->GetNext();
                            }
                            mAngles->Clear();
                            //cerr << "called Clear();\n" << endl;
                        } else if (!action.compare("COMMENT")) {
                            char comment[256];
                            sout.getline(comment, 256, '\n');
                            cerr << "Comment: " << comment << endl;
                        } else {
                            cerr << "Undefined command \"" << action << "\"" << endl;
                            break;
                        }
                    }
                    cout << "A" << added;
                }
                cout << endl; // refinement comments
                sid++;
                temp = "script" + toString(sid);

                // Overwrite the .out file to prepare for the next script TODO: make this a function
                create_output_files();
            }

        }
        // rewriting .out file from scratch
        if (reverse == false) {
            mCurrentResidue++;
            if (mCurrentResidue > mStopResidue) {
                stopHammerTime = true;
            }
        } else {
            mCurrentResidue--;
            mRedcat->offset(mCurrentResidue - 1);
            if (mCurrentResidue < mStopResidue) {
                stopHammerTime = true;
            }
        }

    }//End Hammer Time While Loop


    Polypeptide best(*(mAngles->GetFirst()->GetData()));
    best.setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

    stringstream bestFilename;
    bestFilename << mDataPath << "/best.out";
    ofstream out(bestFilename.str().c_str());
    for (int i = 0; i < best.getResidueCount() - 1; i++)
        out << best.getAminoAcid(i)->getPhi() << " " << best.getAminoAcid(i)->getPsi() << " ";
    out << endl;
}

void Redcraft::readNextAngles() {
    for (size_t pair = 0; pair < mNextAngles.size(); pair++)
        mNextAngles[pair].clear();
    mNextAngles.clear();

    stringstream angleFile;
    angleFile << mDataPath << "/" << mCurrentResidue << ".angles";
    ifstream in(angleFile.str().c_str());
    if (!in.is_open()) {
        cerr << "could not open the angle file for residue " << mCurrentResidue << "." << endl
             << "check to see if it exists." << endl;
        exit(1);
    }

    double phi, psi;

    while (!in.eof()) {
        in >> phi;
        in >> psi;
        if (in.eof()) break;
        in.ignore(256, '\n');

        vector<double> tempStruct;
        tempStruct.push_back(phi);
        tempStruct.push_back(psi);
        mNextAngles.push_back(tempStruct);
    }

    if (mNextAngles.size() == 0) {
        cerr << "Error: empty angle file " << mCurrentResidue << ".angles" << endl;
        exit(0);
    }
    cout << "Completed function readNextAngles" << endl;
}

void Redcraft::createStructures() {
    //string type = getResidueNameAt(mCurrentResidue + 1);

    //TStorage<double, Polypeptide*> *mAnglesBuffer = new TStorage<double,Polypeptide*>();
    TStorage<double, Polypeptide*> *temp;
    TStorageNode<double, Polypeptide*> *node = mAnglesBuffer->GetFirst();
    mAnglesBuffer->Clear();
    temp = mAngles;
    mAngles = mAnglesBuffer;
    mAnglesBuffer = temp;

    //Set Search Depth
    mCurrentDepth=mDefaultDepth;
    if(mSearchDepthSize>0){
        for(int q=0;q<mSearchDepthSize;q++){
            if(mCurrentResidue==mSearchDepth[q][0]){
                mCurrentDepth=mSearchDepth[q][1];
            }
        }
    }

    minCaX = minCaY = minCaZ = numeric_limits<double>::max();
    maxCaX = maxCaY = maxCaZ = -numeric_limits<double>::max(); //Reset the hypercube coordinates.

    if (userEnteredClusterCount == true) //Sets the scale of the cluster to the last calculated vaule of mClusterGranularity
        mCluster->setScale(1.0 / mClusterGranularity);

    //TStorageNode<double,Polypeptide*> *
    node = mAnglesBuffer->GetFirst();

    string type;
    if (mStartResidue <= mStopResidue) {
        type = getResidueNameAt(mCurrentResidue + 1);
        cout << "CurentRes: " << mCurrentResidue << " Sequence: " << type << endl;
        /*



         mCurrentResidue+1



         */
    } else {
        type = getResidueNameAt(mCurrentResidue);
    }
    int ljremoved = 0;
    double lj_max = -1e20;
    double lj_min = 1e20;

    mRedcat->useOTE(mOTEstimationMask[mCurrentResidue]);

    int mAnglesBufferCount = mAnglesBuffer->GetCount();
    //#pragma omp parallel
    //cout << "CreateStructure: before for loop" << endl;
    {

        omp_set_max_active_levels(2);
#pragma omp parallel for private(node)
        for (int currentStructure = 0; currentStructure < mAnglesBufferCount; ++currentStructure) {
            Polypeptide *pp;
            // cout << "CreateStructure: before pp.setVectors" << endl;
            // cout<<typeid(mRedcat->getData()).name()<<endl;
            /*for(int num =0;num<mRedcat->gettotalDataCount()[0];num++){
                 cout<<mRedcat->getData()[0][num][0]<<endl;
             }
             cout<<"loop end"<<endl;*/
            // vector< vector < vector <int> > > tempA(mRedcat->getData());
            //  cout << "CreateStructure: after pp.setVectors" << endl;


#pragma omp critical(ANGLESBUFFER_LOOKUP)
            {
                node = mAnglesBuffer->GetFirst();
                for (int i = 0; i < currentStructure; i++) {
                    if (node != NULL)
                        node = node->GetNext();
                    else
                        cout << "grabbed null node in mAnglesBuffer. this is a serious problem..." << endl;
                }
                pp = node->GetData();
                pp->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

                // pp->setVectors(mRedcat->getData(),mRedcat->gettotalDataCount(),mRedcat->getdataCount(),mRedcat->getshifts());
            }

            if (mStartResidue <= mStopResidue)
                pp->appendAminoAcid(180, 180, type);
            else
                pp->prependAminoAcid(180, 180, type);
            /*cout<<"Before res -2 angles update"<<endl;
                pp->print();
            cout<<"\n";*/
            double _omega = 180;
            type.resize(4);
            if (type == "CPR ")
                _omega = 0;
            for (size_t nextPair = 0; nextPair < mNextAngles.size(); nextPair++) {
                pp->id = currentStructure * mNextAngles.size() + nextPair;
                if (mStartResidue <= mStopResidue)
                    pp->setAngles(pp->getResidueCount() - 2, mNextAngles[nextPair][0], mNextAngles[nextPair][1], _omega);
                else
                    pp->setAngles(0, mNextAngles[nextPair][0], mNextAngles[nextPair][1], _omega);
                /* delet dis
                 * cout<<"After res -2 angles update"<<endl;
                 pp->print();
             cout<<"\n";
                 */
                //  cout<<"Set angles for Resiude: "<<(pp->getResidueCount() - 2)<<endl;
                // FIXME: LJDistance() should be direction-dependent
                // FIXME: Don't bother computing the LJ if it's not enabled? maybe
                double lj = pp->LJDistance();
                if (mLJEnabled && lj > mLJScoreThreshold) { // less than because lj is actually distance
                    ++ljremoved;
                    continue;
                }

                double score = mRedcat->calculateRMSD(*pp);
                double delScore = 0;
                //pp->print();
                //cout << "original score: " << score << endl;

#pragma omp critical(WORSTSCORE_UPDATE)
                gWorstScore = (score > gWorstScore) ? score : gWorstScore;
#pragma omp critical(HYPERCUBE_SIZE)
                {
                    /**
                     * This is where we get the negative and positive extremities of the hypercube in
                     * order to calculate the volume of the cube and calculate the scale needed to
                     * maintain the correct cluster count.
                     **/

                    if (userEnteredClusterCount == true) {
                        //Grab the last amino acid and get it's Ca2 atom
                        AminoAcid lastAA = pp->getAminoAcid(pp->getResidueCount() - 1);
                        Atom carbonAlpha = lastAA.getAtom(AminoAcid::Ca2);
                        double caX, caY, caZ;
                        caX = carbonAlpha.getCoordinate(0);
                        caY = carbonAlpha.getCoordinate(1);
                        caZ = carbonAlpha.getCoordinate(2);

                        //Going to be 6 ugly if statements right now, Might put it into a for loop later with an array to clean it up
                        if (caX < minCaX) {
                            minCaX = caX;
                        } else if (caX > maxCaX) {
                            maxCaX = caX;
                        }
                        if (caY < minCaY) {
                            minCaY = caY;
                        } else if (caY > maxCaY) {
                            maxCaY = caY;
                        }
                        if (caZ < minCaZ) {
                            minCaZ = caZ;
                        } else if (caZ > maxCaZ) {
                            maxCaZ = caZ;
                        }
                    }

                }

                Polypeptide *deletedPP = NULL;
                bool uniqueStructure = true;

#pragma omp critical(ANGLES_LOOKUP)
                {
                    //bool for legibility
                    bool is_full = mAngles->GetCount() >= mCurrentDepth;


                    if (!is_full) {
                        //cout << "adding structure to not full list" << endl;
                        Polypeptide *temp = new Polypeptide(*pp);
                        temp->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

                        mAngles->Insert(score, temp);
                        if (lj_max < lj) lj_max = lj;
                        if (lj_min > lj) lj_min = lj;
                    } else {
                        //bool for legibility
                        bool is_worse = score > mAngles->GetLast()->GetKey();
                        if (!is_worse) {
                            if (lj_max < lj) lj_max = lj;
                            if (lj_min > lj) lj_min = lj;
                            //cout << "bumping polypeptide off the end of the top structures" << endl;
                            Polypeptide *temp = new Polypeptide(*pp);
                            temp->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

                            deletedPP = mAngles->GetLast()->GetData();
                            deletedPP->setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());
                            delScore = mAngles->GetLast()->GetKey();

                            mAngles->Delete(mAngles->GetLast());
                            mAngles->Insert(score, temp);
                        } else {
                            //cout << "this structure didn't belong" << endl;
                            deletedPP = pp;
                            delScore = score;
                            uniqueStructure = false;
                        }

                    }
                }
                if(mMaxStructures<=0){
                    if (mDecimationEnabled && mDecimationMask[mCurrentResidue] && deletedPP != NULL) {
                        //if (delScore < mDecimationScoreThreshold) {
                        double threshold=0;
                        if(mAnglesBuffer->GetCount()<1){
                            threshold=1;
                        }
                        else{
                            threshold=mAnglesBuffer->GetLast()->GetKey()+mAnglesBuffer->GetLast()->GetKey()*mDecimationScoreThreshold/100;
                        }
                        if (delScore < threshold) {
#pragma omp critical(DECIMATION)
                            {
                                // if this pp stays inside decimation - it will be copied
                                mCluster->addPolypeptide(deletedPP, delScore);
                            }
                        }
                    }

                    if (deletedPP != NULL && uniqueStructure)
                        delete deletedPP;
                }
                else{
                    if (mDecimationEnabled && mDecimationMask[mCurrentResidue] && deletedPP != NULL&&mCluster->GetCount()<=mMaxStructures) {
                        //if (delScore < mDecimationScoreThreshold) {
                        double threshold=0;
                        if(mAnglesBuffer->GetCount()<1){
                            threshold=1;
                        }
                        else{
                            threshold=mAnglesBuffer->GetLast()->GetKey()+mAnglesBuffer->GetLast()->GetKey()*mDecimationScoreThreshold/100;
                        }
                        if (delScore < threshold) {
#pragma omp critical(DECIMATION)
                            {
                                // if this pp stays inside decimation - it will be copied
                                mCluster->addPolypeptide(deletedPP, delScore);
                            }
                        }
                    }

                    if (deletedPP != NULL && uniqueStructure)
                        delete deletedPP;
                }
            }
            // pp->print();
            //  cout<<"\n";

            delete pp; // deleting buffer stuffer :)
            //node = node->GetNext();
        }
    }

    /* //Non optimal attempt at adjusting the score threshold

    if (mDecimationEnabled && mDecimationMask[mCurrentResidue] && mCluster->GetCount() == 0) {
            cout << "Decimation was enabled for this block but no structures have made it into the cluster tree" << endl;
            cout << "Current worst score: " << mAngles->GetLast()->GetKey() << endl;
            cout << "Current decmiation threshold: " << mDecimationScoreThreshold << endl;
            mDecimationScoreThreshold = mAngles->GetLast()->GetKey() * 1.2;
    }
     */

    //if (ljremoved)
    //	cout << "LJ violations: " << ljremoved << endl;
    cout << "LJ min/max: " << lj_min << "/" << lj_max << endl;

    TStorageNode<ClusterCoordinates, Polypeptide*> *clusterNode = mCluster->getNode();
    /*if(mDecimationEnabled&&mMaxStructures>=0){
        for (int w =0;w<mMaxStructures;w++){
            if(clusterNode != NULL){
            mAngles->Insert(clusterNode->GetKey().rmsd, clusterNode->GetData());
            clusterNode = clusterNode->GetNext();
            }
            else{
                break;
            }
        }
    }
    else{*/
    while (clusterNode != NULL) {
        // TODO: Optimally would disable sorting here
        mAngles->Insert(clusterNode->GetKey().rmsd, clusterNode->GetData());
        clusterNode = clusterNode->GetNext();
    }
    // }
    //cout<<"Number of clusters: "<<mCluster->GetCount()<<endl;
    mCluster->clear();


    //If the user defines a cluster count, we need to do the calculations to update the cluster granularity for the next iteration
    if (userEnteredClusterCount == true) {
        double length, width, height;
        length = (maxCaX - minCaX);
        width = (maxCaY - minCaY);
        height = (maxCaZ - minCaZ);

        //Get max out of length width and height.
        double max = length;
        if (width > max) max = width;
        if (height > max) max = height;
        max += 10; //Mikhail thinks that adding 10 to the max will help buff the cube size in the case of residue n being too small to predict res n+1
        double granularity = max / pow(mClusterCount, (1.0 / 3.0));
        double euClusterDistance = pow((pow(granularity, 2) + pow(granularity, 2) + pow(granularity, 2)), (.5));

        mClusterGranularity = euClusterDistance;
    }
}

string Redcraft::getResidueNameAt(int index) {
    // index--;
    if (index < 0 || index >= mResidueNames.size()) {
        //cerr << "index out of bounds. defaulting to ALA" << endl;
        //added so i can comment
        return "ALA";
    }

    return mResidueNames[index];
}

void Redcraft::fillResidueNames() {
    if (mSequence) {
        string temp;
        stringstream ss;
        ss << mSeqString;
        for (std::string::iterator it = mSeqString.begin(); it != mSeqString.end(); ++it) {
            ss>>temp;
            mResidueNames.push_back(temp);


        }
        return;
    }
    stringstream inName;
    inName << mDataPath << "/" << mRDCFilePrefix << ".1";
    ifstream in(inName.str().c_str());
    if (!in.is_open()) {
        cerr << "could not read from the rdc file " << inName.str() << " in Redcraft::fillResidueNames()." << endl;
        exit(1);
    }

    mResidueNames.clear();
    string placeholder = "FaK";
    mResidueNames.push_back(placeholder);
    string sequence;
    string line;
    //int i=0;
    while (!in.eof()) {
        getline(in, line);
        if (line.empty())
            break;
        if ((int) line.at(0) > 64 && (int) line.at(0) < 91) {
            sequence = line.substr(0, 4);
            mResidueNames.push_back(sequence);
            //  cout<<"i: "<<mResidueNames[i]<<endl;
            //  i++;
        }

    }

    /*ifstream in(inName.str().c_str());
    if (!in.is_open()) {
        cerr << "could not read from the rdc file " << inName.str() << " in Redcraft::fillResidueNames()." << endl;
        exit(1);
    }

    mResidueNames.clear();

    string sequence;
    string temp;
    string atom1;
    string atom2;
    while (!in.eof()) {
        in >> sequence;
        in.ignore(256, '\n');

        if (in.eof()) break;
        mResidueNames.push_back(sequence);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                in >> temp;

                if (j == 2) {
                    atom1 = temp;
                    mAtomNames1.push_back(map(atom1));
                }
                if (j == 5) {
                    atom2 = temp;
                    mAtomNames2.push_back(map(atom2));
                }

            }
            in.ignore(256, '\n');

        }
    }*/

}


/*void Redcraft::fillResidueNames(int dummy) {
    stringstream inName;
    inName << mDataPath << "/" << mRDCFilePrefix << ".1";
    ifstream in(inName.str().c_str());
    if (!in.is_open()) {
        cerr << "could not read from the rdc file " << inName.str() << " in Redcraft::fillResidueNames()." << endl;
        exit(1);
    }

    mResidueNames.clear();

    string sequence;
    string temp;
    string atom1;
    string atom2;
    while (!in.eof()) {
        in >> sequence;
        in.ignore(256, '\n');

        if (in.eof()) break;
        mResidueNames.push_back(sequence);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                in >> temp;

                if (j == 2) {
                    atom1 = temp;
                    mAtomNames1.push_back(map(atom1));
                }
                if (j == 5) {
                    atom2 = temp;
                    mAtomNames2.push_back(map(atom2));
                }

            }
            in.ignore(256, '\n');

        }
    }
}*/

/*int Redcraft::map(string atom) {

    if (atom == "C") {
        return AminoAcid::C;
    } else if (atom == "N") {
        return AminoAcid::N;
    } else if (atom == "Ca") {
        return AminoAcid::Ca;
    } else if (atom == "C") {
        return AminoAcid::C;
    } else if (atom == "H") {
        return AminoAcid::H;
    } else if (atom == "Ha") {
        return AminoAcid::Ha;
    }
    return -1; //should not return -1
}*/

void Redcraft::print_state() {
    cout << endl;
    cout << "Residue: " << mCurrentResidue << " " << getResidueNameAt(mCurrentResidue) << endl;
    cout << "Flags: ";
    bool minflag = false;
    if (mDecimationEnabled && mDecimationMask[mCurrentResidue]) {
        cout << "D ";
        minflag = true;
    }
    if (mLJEnabled) {
        cout << "V ";
        minflag = true;
    }
    if (mOTEstimationMask[mCurrentResidue]) {
        cout << "OTE ";
        minflag = true;
    }
    if (!minflag) cout << "none";
    cout << endl;
    cout << "depth: " << mCurrentDepth << endl;

    cout << "======================" << endl;
}//end function: print_state

void Redcraft::create_output_files() {
    stringstream filename, redcatName, pdbName;
    filename << mDataPath << "/" << mCurrentResidue << ".out";
    redcatName << mDataPath << "/" << mCurrentResidue << ".redcat";
    pdbName << mDataPath << "/" << mCurrentResidue << ".pdb";
    ofstream out(filename.str().c_str());
    TStorageNode<double, Polypeptide*> *node = mAngles->GetFirst();
    Polypeptide best(*(node->GetData()));
    best.setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

    //cout << "Total Time: " << time(0) - startTime  << endl;
    //cout << "Final Depth:" << mAngles->GetCount() << endl;
    cout << "Scores: " << node->GetKey() << " " << mAngles->GetLast()->GetKey() << " " << gWorstScore << endl;

    // before printing the pdb, rotate best by S1's R to overlay MF on PAF
    //Matrix * orderTensor = new Matrix(5, 1);
    //mRedcat->calculateRMSD(best, 0, orderTensor); // this will populate orderTensor with values
    //Tensor T(*orderTensor);
    //delete orderTensor;
    //Matrix S(3, 3), R(3, 3);
    //T.Decompose(S, R);
    //Rotation PAF(R.Transpose());
    //best.rotate(PAF);
    //best.updateAtoms(0);




    //Grab the user and host name to add as a remark in the polypeptide for later printing in writePDB
    string remark;
    char hostName[150];
    string hName, uName;
#ifdef WIN32
    TCHAR infoBuf[150];
    DWORD bufCharCount = 150;
    if (GetComputerName(infoBuf, &bufCharCount)) {
        hName = string(infoBuf);
    } else {
        hName = "Unknown Host Name.";
    }

#else
    if (gethostname(hostName, 150) == 0) //success
        hName = string(hostName);
    else
        hName = "Unknown Host Name.";
#endif

    //The userName environment variable is the same across all platforms and is passed to redcraft from the terminal
    char * userName;
    userName = std::getenv("USER");
    if (userName != NULL)
        uName = string(userName);
    else
        uName = "Unknown User Name";

    remark = "File created by " + uName + " running on " + hName + ".\t";
    best.add_remark(remark);




    // FIXME: should not rely on REDCAT object for offset()
    if (mStartResidue <= mStopResidue) {
        best.writePDB(pdbName.str(), 1 + mRedcat->offset());
    } else {
        //cout << mStartResidue << " " << mStopResidue << " " << mCurrentResidue << " " << mRedcat->offset() << endl;
        best.writePDB(pdbName.str(), mCurrentResidue);
    }

    for (int medium = 0; medium < mMediaCount; medium++) {
        stringstream fullRedcatName;
        fullRedcatName << redcatName.str() << ".m" << medium + 1;
        mRedcat->createRedcatFile(best, fullRedcatName.str().c_str(), medium);
    }
    for (int i = 0; i < mAngles->GetCount(); i++) {
        for (int residue = 0; residue < node->GetData()->getResidueCount() - 1; residue++)
            out << node->GetData()->getAminoAcid(residue)->getPhi() << " " <<
                node->GetData()->getAminoAcid(residue)->getPsi() << " ";
        out << node->GetKey() << endl;
        node = node->GetNext();
    }

    out.close();
}

//end function: create_output_files

RunState Redcraft::get_run_state() {
    struct RunState s = {
            .mCurrentDepth = mCurrentDepth,
            .mCurrentResidue = mCurrentResidue,
            .mResidueName = getResidueNameAt(mCurrentResidue),
            .mDecimationEnabled = mDecimationEnabled && mDecimationMask[mCurrentResidue],
            .mLJEnabled = mLJEnabled,
            .mOTEstimation = mOTEstimationMask[mCurrentResidue]
    };
    return s;
}
