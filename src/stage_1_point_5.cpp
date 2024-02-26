/**
 * stage_1_point_5.cpp Copyright 2010
 * Dr. Homayoun Valafar, Zach Swearingen
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

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <fstream>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "redcat.h"
#include "polypeptide.h"
#include "aminoacid.h"

using namespace std;

template <class T>
inline string toString(const T &t) {
	stringstream ss;
	ss << t;
	return ss.str();
};

const double kgSmallNumber = 0.00001;
inline bool fcomp(double &f1, double &f2){
    return abs(f1 - f2) < kgSmallNumber;
}

const int kgCountThreshold = 100;
const double kgScoreThreshold = 0.8;

struct TorsionAngle{
    double phi;
    double psi;
    double rmsd;
};

int main(int argc, char *argv[]){
    ifstream curr, next;
    int start, stop, current;

    if(argc == 1){
        start = stop = -1;
    }else if(argc == 3){
        start = atoi(argv[1]);
        stop = atoi(argv[2]);

        if(start < 1 || stop < start){
            cerr << "Error: invalid range (" << start << "-" << stop <<") specified" << endl;
            exit(1);
        }

        if(start == stop){
            cerr << "Error: " << argv[0] << " requires at least two .angles files to run" << endl;
            exit(1);
        }
    }else{
        cerr << "Usage: " << argv[0] << " [<start_residue> <stop_residue>]" << endl;
        exit(1);
    }

    if(start == -1){
        cout << "Starting .angles file undefined, finding first .angles file..." << endl;
        start = 0;
        
        while(!curr.is_open()){
            start++;
            curr.open(toString(start).append(".angles").c_str());
        }

        curr.close();

        cout << "Angle file found at " << start << ".angles" << endl;

        int temp = current = start;

        temp++;
        next.open(toString(temp).append(".angles").c_str());

        if(!next.is_open()){
            cerr << "Error: not enough .angles files to operate. " << argv[0] << " requires at least two .angles files" << endl;
            exit(1);
        }

        cout << "Finding the last .angles file in this range" << endl;

        while(next.is_open()){
            next.close();
            temp++;
            next.open(toString(temp).append(".angles").c_str());
        }

        stop = temp - 1;

        cout << "Final .angles file found at " << stop << ".angles" << endl;

        next.close();
    }else{
        cout << "Range specified, checking for range completeness" << endl;
        current = start - 1;

        do{
            current++;
            curr.open(toString(current).append(".angles").c_str());

            if(!curr.is_open()){
                cerr << "Error: missing angle file (" << current << ".angles) in range." << endl;
                exit(1);
            }

            curr.close();
        }while(current <= stop);

        cout << "Valid range indicated" << endl;

        current = start;
    }

    char buffer[256];
    vector<TorsionAngle> candidates;
    vector<TorsionAngle> nextCandidates;
    TorsionAngle *currCand, *nextCand;
    int readStatus;

    Redcat redcat(".", "RDC", 2);

    cout << "Running stage 1.5 from " << start << ".angles to " << stop << ".angles" << endl;

    while(current < stop){
        cout << "Beginning refinement of " << current << ".angles..." << endl;

        curr.open(toString(current).append(".angles").c_str());
        next.open(toString(current + 1).append(".angles").c_str());

        redcat.offset(current - 1);

        double phi, psi, trash;
        while(!curr.eof()){
            curr.getline(buffer, 256);

            TorsionAngle temp;
            readStatus = sscanf(buffer, "%le %le %le", &phi, &psi, &trash);
            temp.phi = phi;
            temp.psi = psi;
            temp.rmsd = 0.0;
            
            if(readStatus == 3){
                candidates.push_back(temp);
            }
        }

        while(!next.eof()){
            next.getline(buffer, 256);

            TorsionAngle temp;
            readStatus = sscanf(buffer, "%le %le %*e", &(temp.phi), &(temp.psi));
            temp.rmsd = 0.0;

            if(readStatus == 2)
                nextCandidates.push_back(temp);
            //else
            //    delete temp;
        }

        Polypeptide *pp = new Polypeptide();

        pp->appendAminoAcid();
        pp->appendAminoAcid();

        cout << "Beginning scoring" << endl;

        int totalStrucs = candidates.size() * nextCandidates.size();
        int strucsCalcd = 0;

        for(int i = 0; i < candidates.size(); i++){
            currCand = &candidates[i];
            pp->setAngles(0, currCand->phi, currCand->psi);

            for(int j = 0; j < nextCandidates.size(); j++){
                nextCand = &nextCandidates[j];
                pp->setAngles(1, nextCand->phi, nextCand->psi);

                currCand->rmsd += redcat.calculateRMSD(*pp);

                strucsCalcd++;
                cout << "Scoring structure " << strucsCalcd << "/" << totalStrucs << "                  \r";
            }
            currCand->rmsd /= nextCandidates.size();

            if(currCand->rmsd > kgScoreThreshold){
                candidates.erase(candidates.begin() + i);
            }
        }
        cout << "Scoring finished, sorting...                                           " << endl;

        delete pp;

        int top = 0;
        int size = candidates.size();
        double topRmsd = candidates[0].rmsd;

        if(size > kgCountThreshold)
            size = kgCountThreshold;

        curr.close();
        next.close();

        ofstream out(toString(current).append(".angles").c_str());

        for(int i = 0; i < size; i++){
            topRmsd = candidates[0].rmsd;
            top = 0;
            
            for(int j = 0; j < candidates.size(); j++){
                if(candidates[j].rmsd <= topRmsd){
                    top = j;
                    topRmsd = candidates[j].rmsd;
                }
            }

            out << candidates[top].phi << " " << candidates[top].psi << " " << candidates[top].rmsd << endl;
            candidates.erase(candidates.begin() + top);
        }

        out.close();

        cout << "Finished writing " << current << ".angles                    " << endl << endl;

        /*for(int i = 0; i < candidates.size(); i++){
            delete candidates[i];
        }
        cout << "Deleted torsion angles from " << current << ".angles" << endl;

        for(int i = 0; i < nextCandidates.size(); i++){
            delete nextCandidates[i];
        }
        cout << "Deleted torsion angles from " << current + 1 << ".angles" << endl;*/

        candidates.clear();
        nextCandidates.clear();

        current++;
    }
}
