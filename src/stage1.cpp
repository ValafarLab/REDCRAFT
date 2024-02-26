#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include "polypeptide.h"
#include "angles.h"


using namespace std;


/************************************************************************
This program reads RDC files, creates a polypeptide for each amino acid in those files. It then uses redcat to compute the
rmsd for each pair of angles in the RDC files.
 ***************************************************************************/


int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <Prefix> [--custom-ramangles] {[non-gly space 1|2|all] [gly space 0|non gly 1|all] [cutoff Hz]}" << endl;
        return 1;
    }



    //Variable Declarations
    string RDCPrefix(argv[1]); //Store the RDC file prefix name, expecting "RDC"

    Angles angles;
    angles.setRdcPrefix(RDCPrefix);

    bool skip = false;
    double cutoff = 0;

    //Count how many number of RDC.1, RDC.2, RDC.3, etc files are in the directory
    cout << "RDC File Prefix " << RDCPrefix << endl;
    int numOfMedia = 1; //Start this at one as RDC.1 is the first file
    bool more = true;
    while (more) {
        more = false;
        string filename = RDCPrefix + "." + to_string(numOfMedia);
        ifstream infile(filename.c_str());
        if (infile.good()) {
            more = true;
            ++numOfMedia;
        }
    }
    --numOfMedia; //Need to decrement as we started the count at 1.
    if (!numOfMedia) {
        cout << "No alignment media found!'" << endl << "Please check your specified prefix: " << RDCPrefix << endl;
        return 1;
    }

    cout << "Alignment Media found: " << numOfMedia << endl;
    angles.setNumOfMedia(numOfMedia);

    string RDC_file = string(RDCPrefix) + string(".1");

    angles.readAAFromFile(RDC_file, true);

    // user did not enter anything
    if (argc == 2) {
        cout << "Ramachandran Level 1 (more strict)" << endl;
        angles.setRamSpace(RAM1);
        cout << "Ramachandran for GLY" << endl;
        angles.setRamSpaceGly(RAMG);
        skip = true;
        cutoff = 0;
    }

    if (argc > 2)
    {
        if (string("--ignore-custom-ramangles") == argv[2]) {
            angles.setCustomRamspace(false);
            angles.setRamSpace(RAM_ALL);
        }
        else
        {
            angles.setCustomRamspace(true);
            angles.readRAMFromFiles();
            cout << "Non-Gly: ";
            if (string("all") == argv[2]) //if the user enters all
            {
                cout << "All Space" << endl;
                angles.setRamSpace(RAM_ALL);
            } else if (std::stoi(argv[2]) == 2) //if the user enters 2
            {
                cout << "Ramachandran Level 2 (less strict)" << endl;
                angles.setRamSpace(RAM2);
            } else //if the users enters anything else(expecting 1)
            {
                cout << "Ramachandran Level 1 (more strict)" << endl;
                angles.setRamSpace(RAM1);
            }
        }          
    }

    // Generate default Ramachandran spaces and hold them in memory
    angles.generateSpaces();

    if (argc > 3) {
        cout << "Gly: ";
        if (string("all") == argv[3]) //if the user enters all
        {
            cout << "All Space" << endl;
            angles.setRamSpaceGly(RAM_ALL);
        } else if (std::stoi(argv[3]) == 1) //if the user enters 1
        {
            cout << "Ramachandran Level 1 (more strict) for GLY" << endl;
            angles.setRamSpaceGly(RAM1);
        } else//if the user enters anything else(expecting 0)
        {
            cout << "Ramachandran for GLY" << endl;
            angles.setRamSpaceGly(RAMG);
        }
    }

    if (argc > 4) {
        string arg3(argv[4]);
        if (arg3 != "skip" && arg3 != "none")
        {
            if (!arg3.empty() && std::all_of(arg3.begin(), arg3.end(), [] (char c) { return std::isdigit(c) || c == '.';}))
                cutoff = stof(arg3);
            else {
                cerr << "Warning, given cutoff " << arg3 << " not parsable as decimal." << endl;
                cutoff = 0;
            }
            cout << "RDC Fitness cutoff: " << angles.getCutoff() << endl;
        }
    }
    angles.setSkip(skip);
    angles.setCutoff(cutoff);


    vector<vector<anglePair>> angleList;
    vector<vector<double>> rmsdList;
    angles.generateAngles(angleList, rmsdList, true);
    if (angleList.empty()) {
        cout << "Warning! All angles exceeded " << cutoff << "Hz. 0 angles in this file!" << std::endl;
    }
    Angles::writeAngles(angleList, rmsdList, true);
}

