/**
 * pdbgen.cpp Copyright 2019
 * Dr. Homayoun Valafar, Julian Rachele
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

#include "polypeptide.h"
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
using namespace std;

void usage() {
	cout << "PDBGen: Generates PDB from phi/psis and residues. Can output all .out pdbs in bulk." << endl;
	cout << "Options:"
		 "\n\t-p path (no trailing /)"
		 "\n\t-o .out file"
         "\n\t\t-l integer limit of how many pdb files to generate"
         "\n\t-P phi/psi string,\n\t\trequires -n file name (without extension)"
         "\n\t-r RDC file"
		 "\n\t-a amino acid file"
         "\n\t-s Single character amino acids" << endl;
	exit(EXIT_FAILURE);
}

void generatePDB(vector<vector<string>> angles, vector<string> residues, const string &output_path) {
	int depth = (int) (residues.size() >= angles.size() ? angles.size() : residues.size());
	Polypeptide pp;
	for (int i = 0; i < depth; i++) {
		pp.appendAminoAcid2(stod(angles[i][0]), stod(angles[i][1]), residues[i]);
	}
	pp.appendAminoAcid2();
	pp.writePDB(output_path, 1);
	cout << "Wrote polypeptide to " << output_path << endl;
}

int main(int argc, char** argv) {
	bool PHIPSI_STRING = false; // if true, phi psis will be read from a string and not .out files
	int AMINO_FILE_TYPE = 0; // 0 - from RDC 1 - from file containing 3-letter residues 2 - single characters
	string path, out_name, res_name, file_name;
	int limit = 0;
	int c;
	while((c = getopt(argc, argv, "p:o:P:n:r:a:s:l:")) != -1)
		switch (c)
		{
			case 'p':
				path = optarg;
				break;
			case 'o':
				out_name = optarg;
				break;
			case 'P':
				PHIPSI_STRING = true;
				out_name = optarg;
				break;
			case 'n':
				file_name = optarg;
				break;
			case 'r':
				AMINO_FILE_TYPE = 0; // reading from RDC file
				res_name = optarg;
				break;
			case 'a':
				AMINO_FILE_TYPE = 1;
				res_name = optarg;
				break;
			case 's':
				AMINO_FILE_TYPE = 2;
				res_name = optarg;
				break;
			case 'l':
				limit = stoi(optarg);
				break;
			default:
				usage();
		}

	if (path.empty() || out_name.empty() || res_name.empty() || (PHIPSI_STRING && file_name.empty())) {
		usage();
	}
	vector<string> residues;
	string line;

	if (AMINO_FILE_TYPE < 2) {
		// We are reading from a file

		string res_path = path + "/" + res_name;
		ifstream res_file(res_path);

		if (!res_file.is_open()) {
			cerr << "Unable to open file " << res_path << endl;
			exit(EXIT_FAILURE);
		}

		// Read residues first since they won't change
		while (getline(res_file, line)) {
			istringstream stream(line);
			vector<string> l;
			for (std::string s; stream >> s;) {
				l.push_back(s);
			}
			residues.push_back(l[0]);
		}

		if (AMINO_FILE_TYPE == 0) {
			// Parse only residues from RDC
			vector<string> new_res;
			for (int i = 0; i < residues.size(); i+=7) {
				new_res.push_back(residues[i]);
			}
			residues = new_res;
		}

	} else {
		// Read single amino acids
		map<char, string> m;
		vector<string> triples = {"ALA", "CYS", "ASP", "GLU", "PHE", "GLY", "HIS", "ILE", "LYS", "LEU", "MET", "ASN", "PRO", "GLN", "ARG", "SER", "THR", "VAL", "TRP", "TYR", "***", "END"};
		vector<char> singles = {'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'y', '*', '*'};
		for (int i = 0; i < singles.size(); i++) {
			m[singles[i]]=triples[i];
		}
		for (auto residue : res_name) {
			residues.push_back(m[tolower(residue)]);
		}
	}


	if (PHIPSI_STRING) {
		istringstream stream(out_name);
		vector<vector<string>> angles;
		for (string phi, psi; stream >> phi, stream >> psi; ) {
			vector<string> angle;
			angle.push_back(phi);
			angle.push_back(psi);
			angles.push_back(angle);
		}
		generatePDB(angles, residues, path + "/" + file_name + ".pdb");
	} else {
		string out_path = path + "/" + out_name;
		string output_path = path+"/pdb_" + out_name;
		system(("mkdir " + output_path).c_str());
		system(("rm " + output_path + "/*").c_str());

		ifstream out_file(out_path);
		if (!out_file.is_open()) {
			cerr << "Unable to open file " << out_path << endl;
			exit(EXIT_FAILURE);
		}

		// Read and generate a PDB for every line in the .out
		int lm = 0;
		while (getline(out_file, line)) {
			// Because getline() works idiomatically with while{} and does not return a bool
			// We are unable to use the && operator and must use an if...break instead.
			if (limit != 0 && lm >= limit) break;
			istringstream stream(line);
			vector<string> l;
			for (std::string s; stream >> s;) {
				l.push_back(s);
			}
			string f_name = l.back();
			l.pop_back();
			vector<vector<string>> angles;
			for (int i = 0; i < l.size(); i+=2) {
				vector<string> pair;
				pair.push_back(l[i]);
				pair.push_back(l[i+1]);
				angles.push_back(pair);
			}

			generatePDB(angles, residues, output_path + "/" + f_name + ".pdb");
			lm++;
		}
	}


}