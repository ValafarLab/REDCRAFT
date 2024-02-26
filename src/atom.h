#ifndef ATOM_H
#define ATOM_H

#include <iostream>
#include <string>
#include "rotation.h"

using namespace std;

class Atom {

public:
	static void printVersion(){ cout << "Atom.....v1.0\n"; }

	/* constructors */
	Atom(double x=0, double y=0, double z=0);
	Atom(const Atom& old_atom);

	double& operator[](const unsigned int index);
        double& getCoordinate(const unsigned int index);

	void setCoords (double x, double y, double z);
	void translate (double x, double y, double z);
	void rotate (Rotation r);
	Atom cross (Atom a);
	double dot (Atom a);
	double mag ();
	void normalize();

	void print();

protected:

private:
	double coords[3];

};

#endif //ATOM_H

