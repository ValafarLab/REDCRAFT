/**
 * lm-min.cpp Copyright 2011
 * Dr. Homayoun Valafar, Mikhail Simin
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
#include <stdlib.h>
#include <math.h>
#include "redcat.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include "lm/lm.h"
#include "rude_config/config.h"
#include <cstdio>

using namespace std;

Redcat * mRedcat;
int loop;
bool use_LJ;
double LJThreshold;

void rdcdiff(double *angles, double *rdc, int angle_count, int rdc_count, void *data) {
	Polypeptide pp;
        pp.setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());

	for (int i = 0; i < angle_count; i += 2) {
		pp.appendAminoAcid(angles[i], angles[i + 1]);
	}
	pp.appendAminoAcid(180, 180);

	//cerr << "rdcdiff("<<(++loop)<<"): " << mRedcat->calculateRMSD(pp) << endl;
	++loop;
	
	int start = 0;
	for (int i = 0; i < mRedcat->getMedia(); i++) {
		mRedcat->getRDCDiff(pp, i, rdc, start); // TODO: check this function for correctness and leaks
	}

	if (use_LJ) {
		rdc[start] = pp.vdwca() - LJThreshold; // this encompases *all* Ca atoms, not just last one
		if (rdc[start] < 0) rdc[start] = 0;
//		cout << loop << ": " << rdc[start] << endl;
	}

}
int main(int argc, char *argv[]) {
	
	if(argc < 3 || (argc - 2) % 2 != 0){
		cerr << "Usage: " << argv[0] << " <redcraft config> <angles>" << endl;
		exit(1);
	}
	char* redcraft_config = argv[1];
	//string mRDCFilePrefix = argv[2];
	//int mMediaCount = atoi(argv[3]);
	loop = 0;

	//first we'll check to see if the config file even exists
	ifstream fileCheck(redcraft_config);
	if (!fileCheck) {
		cerr << "Error: could not find the configuration file " << redcraft_config << endl;
		exit(0);
	}
	fileCheck.close(); //close the file before rude_config wants it

	//try to open up our config file.
	rude::Config config;
	if (!config.load(redcraft_config)) {
		cerr << "Error: could not load the configuration file " << endl;
		cerr << "       this is likely an error with the format of default.conf" << endl;
		exit(0);
	}

	//try to get to our Run_Settings. this is the only mission critical section,
	//so we need to check for it only. the other sections are optional and their
	//absence means very little
	if (!config.setSection("Run_Settings")) {
		cerr << "Error: could not load the section 'Run_Settings' in " << redcraft_config << endl;
		cerr << "       please make sure the section exists and its label is spelled correctly" << endl;
		exit(0);
	}
	string mDataPath = config.getStringValue("Data_Path");
	string mRDCFilePrefix = config.getStringValue("RDC_File_Prefix");

	int angle_count = argc - 2;
	int mMediaCount = config.getIntValue("Media_Count");
	int mOffset = config.getIntValue("Start_Residue") - 1; // start from 1 = offset 0

	if (config.getIntValue("Stop_Residue") < config.getIntValue("Start_Residue")) {
		// Running in reverse
		mOffset = config.getIntValue("Start_Residue") - (angle_count / 2);
	}

	// FIXME: don't have independent vairables for each of these,
	// store as one, maybe even make the entire rude config global
	use_LJ = config.exists("LJ_Threshold");
	LJThreshold = config.getDoubleValue("LJ_Threshold");

	mRedcat = new Redcat(mDataPath, mRDCFilePrefix, mMediaCount);
	mRedcat->offset(mOffset);

	//cerr << "Angles: " << angle_count << endl;
	//exit(0);

	Polypeptide pp;
        pp.setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());
	double angles[angle_count];
	for (int i = 2; i < argc; i++) angles[i-2] = atof(argv[i]) + 360.0;
	for (int i = 2; i < argc; i += 2) {
		pp.appendAminoAcid(atof(argv[i]), atof(argv[i+1]));
		//cerr << "phi: " << atof(argv[i]) << endl;
		//cerr << "psi: " << atof(argv[i+1]) << endl;
	}
	pp.appendAminoAcid(180, 180);
	int residueCount = pp.getResidueCount(); // NOTE: this includes the final dummy
	int totalRDC = 0;
	for (int m = 0; m < mMediaCount; m++) {
		totalRDC += mRedcat->getReducedRDCCount(m, residueCount);
	}
	//cerr << "Total RDC: " << totalRDC << endl;
	//cerr << "Original RDC fitness: " << mRedcat->calculateRMSD(pp) << endl;

	/*
	cerr << "residues: " << pp.getResidueCount() << endl;
	pp.print();
	cerr << endl;
	*/


	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	opts[0]=LM_INIT_MU;	// tau	\mu
	opts[1]=1E-5;		// esp1 ||J^T e||_int
	opts[2]=1E-5;		// eps2 ||Dp||_2
	opts[3]=1E-3;		// eps3 ||e||_2
	//opts[4]		// delta

	if (use_LJ) totalRDC += 1; // fake 1 RDC value for steric collision

	double rdcvalues[totalRDC];
	for (int i = 0; i < totalRDC; i++) rdcvalues[i] = 0.0;
	int ret = dlevmar_dif(rdcdiff, angles, rdcvalues, angle_count, totalRDC, 1000, opts, info, NULL, NULL, NULL);

	Polypeptide minpp;
        minpp.setVectors(mRedcat->getData(), mRedcat->gettotalDataCount(), mRedcat->getdataCount(), mRedcat->getshifts());
	for (int i = 0; i < angle_count; i += 2) {
		while (angles[i] > 180) angles[i] -= 360;
		while (angles[i] <-180) angles[i] += 360;
		while (angles[i+1] > 180) angles[i+1] -= 360;
		while (angles[i+1] <-180) angles[i+1] += 360;
		cout << angles[i] << " ";
		cout << angles[i+1] << " ";
		minpp.appendAminoAcid(angles[i], angles[i+1]);
	}
	minpp.appendAminoAcid(180,180);
	cout << mRedcat->calculateRMSD(minpp) << endl;

	/*
	cerr << "Stopped by ";
	switch ((int)(info[6] + 0.5)) {
		case 1 : cerr << "small gradient J^T e"; break;
		case 2 : cerr << "small Dp"; break;
		case 3 : cerr << "itmax"; break;
		case 4 : cerr << "singular matrix (increase \\mu)"; break;
		case 5 : cerr << "no reduction possible (increase \\mu)"; break;
		case 6 : cerr << "small ||e||_2"; break;
		case 7 : cerr << "invalid func values"; break;
	}
	cerr << endl;
	*/

	delete mRedcat;

	return 0;	
}
