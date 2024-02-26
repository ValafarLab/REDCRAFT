/**
 * cmin.cpp Copyright 2009
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "redcat.h"
#include "lm/lm.h"

using namespace std;

string mDataPath;
string mRDCFilePrefix;
vector<string> mResidueNames;
void fillResidueNames();
string getResidueNameAt(int);
Redcat *mRedcat;
int *indecies;
double *defaultAngles;	//TODO: this variable isn't needed if delta(data(del)) stores them, or delta can be smaller (2m instead of 3m)
int m;

void rdcdiff(double *p, double *x, int ms, int n, void *data) {
	double * del = (double *) data;
	// del is now an array size m*3, where 0-m is original p, m+1 through 2*m is min phi/psi, and 2*m+1 through 3*m is max phi/psi
	// p contains phi/psi (modified by levmar)
	// x contains error in RDC


	// Create structure with appropriate angles
	// should be "m" of them
	Polypeptide pp;
	int resnum = 1;
	double phi,psi;
	int il = 0; // array `indecies` is sorted, il will keep track of which one we are waiting for as "index location"
	for(int i = 0; i < m; i += 2) {
		if (indecies[il] == i) {
			if (p[il] < del[m+i]) {
				p[il] = del[m+i];

			}
			if (p[il] > del[2*m+i]) {
				p[il] = del[2*m+i];
			}
			phi = p[il];
			il++;
		} else {
			phi = defaultAngles[i];
		}
		if (indecies[il] == (i + 1)) {
			if (p[il] < del[m+i+1]) p[il] = del[m+i+1];
			if (p[il] > del[2*m+i+1]) p[il] = del[2*m+i+1];
			psi = p[il];
			il++;
		} else {
			psi = defaultAngles[i+1];
		}
		
		pp.appendAminoAcid(phi, psi, getResidueNameAt(resnum++));

	}
	pp.appendAminoAcid(180,180, getResidueNameAt(resnum));

	// Calculate RDCs
	// shold be "n" of them
	// populate x (size n) with difference between RDCs

	/*int start = 0;
	for (int i = 0; i < mRedcat->getMedia(); i++) {
		mRedcat->getRDCDiff(pp, i, x, start);
	}
	*/
	//x[0] = mRedcat->calculateRMSD(pp);
	int start = 0;
	for (int i = 0; i < mRedcat->getMedia(); i++) {
		mRedcat->getRDCDiff(pp, i, x, start); // TODO: check this function for correctness and leaks
		//cerr << "start: " << start << endl;
	}

}

int main(int argc, char *argv[]) {
	if(argc < 6){
		cout << "usage: " << argv[0] << " <data path> <rdc file prefix> <media> <offset> <angle constrain file>" << endl;
		cout << "the constrain file should be in format: RES phi psi minphi minpsi maxphi maxpsi" << endl;
		exit(1);
	}
	register int i;
	mDataPath = argv[1];
	mRDCFilePrefix = argv[2];
	fillResidueNames();

	// read the angle file
	ifstream a(argv[5]);
	if (!a.is_open()) {
		cerr << "Could not open the angle constrain file " << argv[5] << "." << endl;
		exit(0);
	}

	m = -1;
	string sTemp;
	while (!a.eof()) {
		getline(a,sTemp);
		m++;
	}
	m *= 2;
	double p[m];
	double delta[3*m]; // instead of [m][3] we just store them back to back
			  // 3 because 1 is phi/psi, 2 is min phi/psi, and 3 is max phi/psi

	mRedcat = new Redcat(argv[1], argv[2], atoi(argv[3]));
	mRedcat->offset(atoi(argv[4]));

	a.clear(); a.seekg(0, ios::beg);

	bool firstRes = true;
	int nonzero = 0;
	for(i = 0; i < m; i+=2) {
		// ALA PHI PSI minPHI minPSI maxPHI maxPSI Comments
		a >> sTemp;
		if (firstRes) {
			firstRes = false;
			if (sTemp.compare(getResidueNameAt(mRedcat->offset()+1)) && sTemp.compare("UNK")) { // if the first residue doesn't match and it's not UNK
				cerr << "Warning: Residue name in the angle constrain file and the RDC file do not match! (" << sTemp << ") (" << getResidueNameAt(mRedcat->offset()+1) << "). If unknown -- change to UNK" << endl;
			}
		}
		a >> p[i];
		if (fabs(p[i]) < 1e-5) { p[i] = 360; }
		delta[i] = p[i]; // phi. 360 is added to avoid numerical 0 -- levmar doesn't handle it
		
		a >> p[i+1];
		if (fabs(p[i+1]) < 1e-5) { p[i+1]= 360; }
		delta[i+1] = p[i+1]; // psi

		a >> delta[m+i]; // min phi
		a >> delta[m+i+1]; // min psi

		a >> delta[2*m+i]; // max phi
		a >> delta[2*m+i+1]; // max psi
		if ( fabs(delta[m+i] - delta[2*m+i]) > 1e-3) nonzero++;
		if ( fabs(delta[m+i+1] - delta[2*m+i+1]) > 1e-3) nonzero++;

		a.ignore(256,'\n');
	}

	// at the moment p is populated completely, now we need to compress it so where only Max - Min != 0 remain

	if (nonzero == 0) { cerr << "Error: Did not find any variables" << endl; exit(1); }
	int pids[nonzero];
	double rp[nonzero]; // real "p", compressed
	indecies[nonzero];
	indecies = pids; // global pointer to these ids
	defaultAngles = p;
	int tempid = 0;
	for (i = 0; i < m; i++) { // delta[] is of size 2m, phi,psi, then dPhi, dPsi
		if (fabs(delta[m+i] - delta[2*m+i]) < 1e-3) continue;
		pids[tempid] = i;
		rp[tempid] = p[i];
		tempid++;
	}



	int n = 0;
	for (i = 0; i < atoi(argv[3]); i++) {
		n += mRedcat->getReducedRDCCount(i, m/2);
	}

	//n = 1; // dirty hack -- instead of individual rdc values we use RMSD from redcat

	int ret;
	double x[n];
	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];

	opts[0]=LM_INIT_MU; opts[1]=1E-3; opts[2]=1E-20; opts[3]=1E-20;

	for(i=0; i<n; i++) x[i]=0.0;
	ret=dlevmar_dif(rdcdiff, rp, x, nonzero, n, 1000, opts, info, NULL, NULL, delta);

	int il = 0; // index location
	for (i=0; i<m; ++i) { // print rp[] if it's a variable, and p[] if constraint is 0
		if (indecies[il] == i) {
			while (rp[il] > 180) rp[il] -= 360;
			while (rp[il] <-180) rp[il] += 360;
			printf("%.5g ", rp[il]);
			il++;
		} else {
			while (p[i] > 180) p[i] -= 360;
			while (p[i] <-180) p[i] += 360;
			printf("%g ", p[i]);
		}
	}
	cout << endl;

/*	printf("\n\nMinimization info:\n");
	for(i=0; i<LM_INFO_SZ; ++i)
		printf("%g ", info[i]);
*/
	delete mRedcat;
	return 0;
}

void fillResidueNames(){
	stringstream inName;
	inName << mDataPath << "/" << mRDCFilePrefix << ".1";
	ifstream in(inName.str().c_str());
	if(!in.is_open()){
		cerr << "could not read from the rdc file " << inName.str() << " in fillResidueNames()" << endl;
	}

	mResidueNames.clear();

	string sTrash;
	while(!in.eof()){
		in >> sTrash;
		mResidueNames.push_back(sTrash);
		for(int i = 0; i < 7; i++){
			in.ignore(256,'\n');
		}
	}
}

string getResidueNameAt(int index){
	index--;
	if(index < 0 || index >= mResidueNames.size()){
		return "ALA";
	}
	return mResidueNames[index];
}

