/**
 * exmin.cpp Copyright 2009
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

#include "lm/lm.h"

using namespace std;

Redcat * mRedcat;
int loop;

void rdcdiff(double *angles, double *rdc, int angle_count, int rdc_count, void *data) {
	Polypeptide pp;
	for (int i = 0; i < angle_count; i += 2) {
		pp.appendAminoAcid(angles[i], angles[i + 1]);
	}
	pp.appendAminoAcid(180, 180);

	//cerr << "rdcdiff("<<(++loop)<<"): " << mRedcat->calculateRMSD(pp) << endl;
	
	int start = 0;
	for (int i = 0; i < mRedcat->getMedia(); i++) {
		mRedcat->getRDCDiff(pp, i, rdc, start); // TODO: check this function for correctness and leaks
		//cerr << "start: " << start << endl;
	}
//	for (int i = 0; i < rdc_count; i++)
//		cerr << "rdce: " << rdc[i] << endl;

}
int main(int argc, char *argv[]) {
	
	if(argc < 6 || (argc - 5) % 2 != 0){
		cerr << "Usage: " << argv[0] << " <data path> <rdc file prefix> <media> <offset> <angles>" << endl;
		exit(1);
	}
	string mDataPath = argv[1];
	string mRDCFilePrefix = argv[2];
	int mMediaCount = atoi(argv[3]);
	int mOffset = atoi(argv[4]);
	loop = 0;

	mRedcat = new Redcat(mDataPath, mRDCFilePrefix, mMediaCount);
	mRedcat->offset(mOffset);

	int angle_count = argc - 5;
	//cerr << "Angles: " << angle_count << endl;
	Polypeptide pp;
	double angles[angle_count];
	for (int i = 5; i < argc; i++) angles[i-5] = atof(argv[i]) + 360.0;
	for (int i = 5; i < argc; i += 2) {
		pp.appendAminoAcid(atof(argv[i]), atof(argv[i+1]));
		//cerr << "phi: " << atof(argv[i]) << endl;
		//cerr << "psi: " << atof(argv[i+1]) << endl;
	}
	pp.appendAminoAcid(180, 180);
	int residueCount = pp.getResidueCount(); // NOTE: this includes the final dummy
	//cerr << mRedcat->getReducedRDCCount(0, residueCount) << " ";
	//cerr << mRedcat->getReducedRDCCount(1, residueCount) << endl;
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
	opts[0]=0.05;	// tau	\mu
	opts[1]=1E-20;		// esp1 ||J^T e||_int
	opts[2]=1E-20;		// eps2 ||Dp||_2
	opts[3]=1E-20;		// eps3 ||e||_2
	//opts[4]		// delta

	double rdcvalues[totalRDC];
	for (int i = 0; i < totalRDC; i++) rdcvalues[i] = 0.0;
	int ret = dlevmar_dif(rdcdiff, angles, rdcvalues, angle_count, totalRDC, 10000, opts, info, NULL, NULL, NULL);

	Polypeptide minpp;
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
