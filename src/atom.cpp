/**
 * atom.cpp Copyright 2005, 2009
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

#include <cstdlib>
#include "atom.h"

Atom::Atom(double x, double y, double z){
	setCoords(x, y, z);
}

Atom::Atom(const Atom& old_atom){
	setCoords(old_atom.coords[0], old_atom.coords[1], old_atom.coords[2]);
}

double& Atom::operator[](const unsigned int index){
	if(index > 2){
		std::cerr << "\"" << index << "\" is an invalid index into an Atom." << std::endl;
		exit(1);
	}

	return coords[index];
}

double& Atom::getCoordinate(const unsigned int index){
    if(index > 2){
		std::cerr << "\"" << index << "\" is an invalid index into an Atom." << std::endl;
		exit(1);
	}
    return coords[index];
}

void Atom::setCoords (double x, double y, double z){
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}

void Atom::translate (double x, double y, double z){
	coords[0] += x;
	coords[1] += y;
	coords[2] += z;
}

void Atom::rotate(Rotation r){
	//if (r.isIdentity()) return;
	double nx, ny, nz;
	nx = r.Get_Mij(0, 0) * coords[0] + r.Get_Mij(0, 1) * coords[1] + r.Get_Mij(0, 2) * coords[2];
	ny = r.Get_Mij(1, 0) * coords[0] + r.Get_Mij(1, 1) * coords[1] + r.Get_Mij(1, 2) * coords[2];
	nz = r.Get_Mij(2, 0) * coords[0] + r.Get_Mij(2, 1) * coords[1] + r.Get_Mij(2, 2) * coords[2];

	coords[0] = nx;
	coords[1] = ny;
	coords[2] = nz;
}

Atom Atom::cross (Atom a){
	double x, y, z;
	
	x = coords[1] * a.coords[2] - coords[2] * a.coords[1];
	y = -coords[0] * a.coords[2] + coords[2] * a.coords[0];
	z = coords[0] * a.coords[1] - coords[1] * a.coords[0];

	return Atom(x, y, z);
}

double Atom::dot (Atom a){
	return coords[0] * a.coords[0] + coords[1] * a.coords[1] + coords[2] * a.coords[2];
}

double Atom::mag(){
	return sqrt(pow(coords[0], 2) + pow(coords[1], 2) + pow(coords[2], 2));
}

void Atom::normalize(){
	double magnitude = mag();
	for(int i = 0; i < 3; i++) coords[i] /= magnitude;
}

void Atom::print (){
	cout << "[" << coords[0] << ", " << coords[1] << ", " << coords[2] << "]" << endl;
}
