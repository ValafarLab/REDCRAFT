/**
 * stage2.cpp Copyright 2010
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

#include "redcraft.h"
#include "rude_config/config.h"
#include <cstdio>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#ifdef _WIN32
#include <ctime>
#endif

using namespace std;

static string svn_id() {
	return "$Id: stage2.cpp 415 2012-10-04 18:38:24Z browndd3 $";
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> split(const string &s, char delim = ' ') {
	vector<string> elems;
	return split(s, delim, elems);
}

/**
 * our main function for running redcraft.
 * */
int main(int argc, char *argv[]) {

#ifdef _OPENMP
    omp_set_num_threads(omp_get_num_procs());
#endif
	string config_filename = "./redcraft.conf"; //Default for RedCraft construct if no other conf file is specified
	int c;
	bool contFrom(false);
	const char* contNum;

	static struct option long_options[] = {
		{"create-new", no_argument, 0, 'n'},
		{"config=", required_argument, 0, 'c'},
		{"continue-from=", required_argument, 0, 'f'},
		{"version", no_argument, 0, 'v'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};//end struct option

	while(1) {
		int option_index = 0;

		c = getopt_long(argc, argv, "nc:f:vh", long_options, &option_index);
		if (c == -1) {break;}

		switch (c)
		{
			case 'n':
			{

				/**
				 * if the user wants to create a new configuration file with default values,
	 			 * then we'll go ahead and create a new one for them and exit
	 			**/
				cout << "Creating redcraft.conf ..." << endl;
				ofstream newconf("./redcraft.conf");
				newconf << "# To learn more about this configuration file please visit:" << endl;
                                newconf <<"# https://redcraft.readthedocs.io/en/latest/usage/usage.html" <<endl;
				/*newconf << "# http://ifestos.cse.sc.edu/wiki/html/index.php/Redcraft" << endl << endl;*/

				newconf << "[Run_Settings]" << endl;
				newconf << "Run_Type=new" << endl;
				newconf << "Start_Residue=1" << endl;
				newconf << "Stop_Residue=25" << endl;
				newconf << "Media_Count=2" << endl;
				newconf << "Data_Path=\".\"" << endl;
				newconf << "RDC_File_Prefix=nefRDC" << endl;
                                newconf << "RDC_File_Format=NEF" << endl;
                                newconf << "Default_Search_Depth=1000" << endl;
				newconf << "LJ_Threshold=50.0" << endl;
                                newconf << "[/Run_Settings]" << endl << endl;

				newconf << "[Depth_Search_Settings]" << endl;
				newconf << "#Residue_18=5000" << endl;
				newconf << "[/Depth_Search_Settings]" << endl << endl;

				newconf << "[Decimation_Settings]" << endl;
				newconf << "#Cluster_Sensitivity=1" << endl;
				newconf << "#Score_Threshold=1" << endl;
				newconf << "#Decimation_Ranges=3, 4-6, 9" << endl;
				newconf << "#Cluster_Count=100" << endl; 
                                newconf << "#Maximum_Number_of_Additional_Structures=100" << endl;
				newconf << "[/Decimation_Settings]" << endl << endl;

				newconf << "[OTEstimation]" << endl;
				newconf << "# syntax for OrderTensorEstimation is S?=Sxx Syy Sxy Sxz Syz" << endl;
				newconf << "#S1=Sxx Syy 0 0 0" << endl;
				newconf << "#S2=Sxx2 Syy2 Sxy2 Sxz2 Syz2" << endl;
				newconf << "#Tolerance=1.0" << endl;
				newconf << "#Weight=1.0" << endl;
				newconf << "#Estimation_Range=5-25,40,42" << endl;
				newconf << "#Dmax" << endl;
				newconf << "[/OTEstimation]" << endl << endl;

				newconf << "[Refinement]" << endl;
				newconf << "#script1=unconstrained.prl" << endl;
				newconf << "[/Refinement]" << endl<<endl;
                                
                                newconf << "[Sequence]" << endl;
				newconf << "#Chain=" << endl;
				newconf << "[/Sequence]" << endl;

				newconf.close();
				exit(0);
			}//end case 'n'

			case 'c': //if the user wants to specify a configuration file other than ./redcraft.conf
			{
				cout << "stage2 :: Using " << optarg << endl;
				config_filename = optarg;
				break;
			}//end case 'c'

			case 'f':
			{
				contFrom = true;
				contNum = optarg;
				break;
			}//end case 'f'

			case 'v':
			{		
				cout << "stage2: " << split(svn_id())[2] << endl;
				cout << "Redcraft: " << split(Redcraft::svn_id())[2] << endl;
				cout << "Redcat: " << split(Redcat::svn_id())[2] << endl;
				cout << "Matrix: " << split(Matrix::svn_id())[2] << endl;
				cout << "Polypeptide: " << split(Polypeptide::svn_id())[2] << endl;
				cout << "AminoAcid: " << split(AminoAcid::svn_id())[2] << endl;
				exit(0);
			}//end case 'v'

			case 'h':
			{
				cout << "Usage: stage2 [OPTION...] "<< endl;
				cout << "Run Stage-II of REDCRAFT" << endl;
				cout << "\n";
				cout << "	-h, --help			Display this help and exit." << endl;
				cout << "	-n, --create-new		Create a new configuration file with default values." << endl;
				cout << "	-c, --config=FILE		Specify a specific configuration file." << endl;
				cout << "	-f, --continue-from=NUMBER	Continue from a specific residue." << endl;
				cout << "	-v, --version			Display version information and exit." << endl;
				exit(0);
			}//end case 'h'

			default:
			{
				cout << "Use -h or --help for a list of available arguments." << endl;
				exit(0);
			}//end default
		}// end switch (c)
	}//end while
	

	/**
	 * here we launch into error checking our inputs. we don't want the run to
	 * die mid way through when we could have easily corrected it beforehand
	 **/

	// instead of spitting out multiple warnings create error_log (maybe not)

	//first we'll check to see if the config file even exists
	ifstream fileCheck(config_filename.c_str());
	if (!fileCheck) {
		cerr << "Error: could not find the configuration file " << config_filename << endl;
		cerr << "       rerun with --create-new to create a default config file or --config=<filename.conf> to specify a config file." << endl;
		exit(0);
	}
	fileCheck.close(); //close the file before rude_config wants it

				
	//try to open up our config file.
	rude::Config config;
	if (!config.load(config_filename.c_str())) {
		cerr << "Error: could not load the configuration file " << config_filename << endl;
		cerr << "       this is likely an error with the format of " << config_filename << endl;
		cerr << "       please ensure the configuration file is complete, or run " << argv[0] << " again with --create-new" << endl;
		exit(0);
	}

	//try to get to our Run_Settings. this is the only mission critical section,
	//so we need to check for it only. the other sections are optional and their
	//absence means very little
	if (!config.setSection("Run_Settings")) {
		cerr << "Error: could not load the section 'Run_Settings' in default.conf" << endl;
		cerr << "       please make sure the section exists and its label is spelled correctly" << endl;
		cerr << "       you can also run " << argv[0] << " again with --create-new to create a new configuration file" << endl;
		exit(0);
	}

	/**
	 * the following are checks on the configuration file to make sure all of
	 * the inputs make sense. this should drastically reduce problems with runs
	 **/

	//is our run type valid? correct values are 'new' and 'continue' only
	string runType = config.getStringValue("Run_Type");
	if (runType.compare("new") && runType.compare("continue")) {
		cerr << "Error: invalid run type '" << runType << "' specified." << endl;
		cerr << "       valid values are 'new' and 'continue' only" << endl;
		exit(0);
	}

	//do we have enough media?
	int media = config.getIntValue("Media_Count");
	if (media <= 0) {
		cerr << "Error: cannot run with 0 or fewer media" << endl;
		exit(0);
	}

	//is our default search depth greater than 0?
	int defaultDepth = config.getIntValue("Default_Search_Depth");
	if (defaultDepth <= 0) {
		cerr << "Error: the default search depth must be greater than 0" << endl;
		exit(0);
	}

	/**
	 * now we want to check to make sure all of our relevant angle files are
	 * correct (for now, just make sure there are no duplicates. this is a brute
	 * force algorithm written quickly to get the functionality in redcraft.
	 **/

	//we'll need to grab the data path for our reference. in case it doesn't
	//exist in the file, we'll assume it's "." and warn the user
	string dataPath = config.getStringValue("Data_Path");
	string rdcPrefix = config.getStringValue("RDC_File_Prefix");
        string rdcFormat = config.getStringValue("RDC_File_Format");
	if (dataPath.size() == 0) {
		cerr << "Warning: Data_Path is empty or missing from default.conf" << endl;
		cerr << "         assuming a value of '.' to continue the run." << endl;
		dataPath = ".";
	}
	
	
	
	/**
	* Error checking for the decimation settings. If Decimation_Ranges is enabled then
	* the user has to define Cluster_Count or Cluster_Sensitivity but not both.
	**/
	
	if(config.setSection("Decimation_Settings"))
	{
		if(config.exists("Decimation_Ranges"))
		{
		
			if(config.exists("Cluster_Count") && config.exists("Cluster_Sensitivity")) //Both
			{
				cerr << "Error: You cannot define both a target Cluster_Count and a target Cluster_Sensitivity. " << 
				"Please only input one of the two." << endl;
				exit(1);
			}
			if(!config.exists("Cluster_Count") && !config.exists("Cluster_Sensitivity")) //Neither
			{
				cerr << "Error: Decimation Ranges is enabled but neither target Cluster_Count or Cluster_Sensitivity exists." << endl;
				exit(1);
			}
			if(config.exists("Cluster_Count")) //Make sure cluster count is >=1.
			{
				if(config.getIntValue("Cluster_Count") < 1)
				{
					cerr << "Error: Cluster Count must be a positive integer greater than 0." << endl;
					exit(1);
				}
			}
			if(config.exists("Cluster_Sensitivity")) //Make sure cluster sensitivity is > 0.
			{
				if(config.getDoubleValue("Cluster_Sensitivity") <= 0.0)
				{
					cerr << "Error: Cluster Sensitivity must be greater than 0.0" << endl;
					exit(1);
				}
			}
		}
	}
	
	
	config.setSection("Run_Settings"); //Set the section back to run settings after changing it above
	
	
	

	//this offset is used to advance from startResidue to stopResidue. if we're
	//running in reverse, then we'll be decrementing our index. otherwise we'll
	//be incrementing like usual.
	int startResidue = config.getIntValue("Start_Residue");
	int stopResidue = config.getIntValue("Stop_Residue");
	int offset = (startResidue > stopResidue) ? -1 : 1;

	//this is our line buffer for input
	char inLine[512];

	//run over each of our residues...
	for (int i = startResidue; i != stopResidue + offset; i += offset) {
		//these vectors will represent the values we've read so far from the
		//angle file. phiList will contain each unique phi value in our angle
		//file. psiList is a vector of vectors of doubles. each primary index in
		//psiList represents the corresponding phiList angle, with each
		//secondary index representing a unique psi value coupled with the phi.
		//a useful way of envisioning this is as follows:
		// phiList  psiList  ->     1   2   3   4   5
		// 1 50     vector<double>   60  -10 90  -40 170
		// 2 -70    vector<double>   50  70
		// 3 120    vector<double>   120 80  -40
		// 4 160    vector<double>   -10
		//represents the angles:
		//50 60, 50 -10, 50 90, 50 -40, 50 170, -70 50, -70 70, etc.
		vector< vector<double> > psiList;
		vector<double> phiList;

		//this will tell us if we need to rewrite the angle file. i'm not going
		//to implement this anymore. instead, we'll just warn users for now. we
		//might come up with a prettier implementation later on.
		bool rewriteFile = false;

		//we'll get the name of our current angle file that we're harvesting
		//input from ready. then we'll go ahead and open it.
		stringstream infileName;
		infileName << dataPath << "/" << i << ".angles";
		ifstream in(infileName.str().c_str());

		if (!in.is_open()) {
			cerr << "Error: missing file " << infileName.str() << endl;
			exit(1);
		}

		//get our variables ready to hold our phi/psi values as we read them.
		//there is no need for any trash variables as we ignore everything after
		//the first two floating point numbers. line is an integer that tells us
		//what line we're currently reading. this is for helping remove
		//duplicate lines by hand.
		float phi, psi, trash; // has to be a float and not a double for sscanf
		int line = 0;

		//while we still have more input to read...
		while (!in.eof()) {
			//we're on a new line, so let's increment
			line++;

			//grab the whole line, parse out our first two doubles, and ignore
			//the rest.
			in.getline(inLine, 512);
			sscanf(inLine, "%e %e", &phi, &psi);

			//for some reason this was reading an extra (nonexistent) line of angles.
			//so we just make a check to see if we should
			//break early.
			if (in.eof())
				break;

			//if our phi or psi is 180, make it -180 for our purposes.
			phi = (phi == 180) ? -180 : phi;
			psi = (psi == 180) ? -180 : psi;

			//this is where it gets fun and a little downsy
			//this variable's can't be explained without seeing their use.
			bool addNewLine = true;
			bool addNewPsi = true;

			//for each value in the phiList...
			for (int j = 0; j < phiList.size(); j++) {
				//check to see if the phi value we read in is equal
				if (phiList[j] == phi) {
					//if it is, we'll be depositing the psi value in the corres-
					//ponding psiList. but we need to make sure that the psi
					//doesn't already exist there. so for each psi value in the
					//psiList...
					for (int k = 0; k < psiList[j].size(); k++) {
						//see if we have a match.
						if (psiList[j][k] == psi) {
							//if we do, then we've found a duplicate. warn the user
							cerr << "Warning: duplicate angle detected in " << infileName.str() << " on line " << line << endl;

							//this is where we'd say we need to rewrite the file
							//we're not doing this currently.
							rewriteFile = true;

							//we won't be adding a new psi to this vector since
							//a duplicate psi was already found
							addNewPsi = false;
						}
					}

					//now that we're done scanning for duplicate angles, let's
					//see if we need to add the psi to our psiList.
					if (addNewPsi) {
						psiList[j].push_back(psi);
					}

					//at this point, we know the phi we just read exists in
					//phiList. since it exists already, we *don't* need to add
					//a new phi to the list.
					addNewLine = false;
				}
			}

			//now, let's see if we need to add a new line to the lists...
			if (addNewLine) {
				//if we do, we'll push back the new phi onto the phi list
				phiList.push_back(phi);

				//and we'll create a new vector with our psi value to push back
				//onto our psi list
				vector<double> temp (0,0);
				temp.push_back(psi);
				psiList.push_back(temp);
			}
		}

		//at this point we've finished checking for duplicate angles. if we had
		//found any, rewriteFile would be true. however, we don't want redcraft
		//to do this because everything but the phi/psi values would be lost.
		//if(rewriteFile){
		//    cout << "Warning: rewriting " << infileName.str() << " to eliminate duplicate angles" << endl;
		//    ofstream out(infileName.str().c_str());
		//    for(int j = 0; j < phiList.size(); j++){
		//        for(int k = 0; k < psiList[j].size(); k++){
		//            out << phiList[j] << " " << psiList[j][k] << " 0.0" << endl;
		//        }
		//    }
		//}
	}

	//create our redcraft object
	Redcraft rc(config_filename.c_str());
	if (contFrom) {
		int continue_from = atoi(contNum);
		cout << "stage2 :: overriding config specification." << endl;
		cout << "Run_Type=continue" << endl << "Start_Residue=" << continue_from << endl;
		rc.set_continue(continue_from);
	}


	//start a timer to measure how long redcraft takes to run. then run redcraft
	long unsigned int startTime = time(0);
	rc.run();
	long unsigned int totalTime = time(0) - startTime;
	unsigned int minutes = 0, hours = 0, seconds = 0;
	if (totalTime > 60) {
		minutes = (int) totalTime / 60;
		seconds = totalTime - (minutes * 60);
	} else seconds = totalTime;
	if (minutes > 60) {
		hours = minutes / 60;
		minutes -= hours * 60;
	}
	cout << "redcraft ran in " << hours << ":" << minutes << ":" << seconds << endl;
	//returned all clear!
	return 0;
}
