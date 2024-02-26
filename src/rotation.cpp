/**
 * rotation.cpp Copyright 2005
 * Dr. Homayoun Valafar
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
#include "rotation.h"

Rotation::Rotation (double x, double y, double z, double theta){
	Matrix(3, 3);

	double mag;
	
	mag = sqrt(x * x + y * y + z * z);
	/*
	if (mag < 1e-30) {
		makeIdentity();
		return;
	}
	*/

	x /= mag;
	y /= mag;
	z /= mag;

	if (isnan(x)) { // if mag == 0, but this way I don't have to compare a double to an int
		makeIdentity();
		return;
	}

	Set_Mij(0, 0, cos(theta) + (1 - cos(theta)) * x * x);
	Set_Mij(0, 1, (1 - cos(theta)) * x * y - sin(theta) * z);
	Set_Mij(0, 2, (1 - cos(theta)) * x * z + sin(theta) * y);
	Set_Mij(1, 0, (1 - cos(theta)) * y * x + sin(theta) * z);
	Set_Mij(1, 1, cos(theta) + (1 - cos(theta)) * y * y);
	Set_Mij(1, 2, (1 - cos(theta)) * y * z - sin(theta) * x);
	Set_Mij(2, 0, (1 - cos(theta)) * z * x - sin(theta) * y);
	Set_Mij(2, 1, (1 - cos(theta)) * z * y + sin(theta) * x);
	Set_Mij(2, 2, cos(theta) + (1 - cos(theta)) * z * z);
}

Rotation::Rotation (double a, double b, double c){
	Matrix(3, 3);

	Set_Mij(0, 0, cos (c) * cos (b) * cos (a) - sin (c) * sin (a));
	Set_Mij(0, 1, cos (c) * cos (b) * sin (a) + sin (c) * cos (a));
	Set_Mij(0, 2, -cos (c) * sin (b));
	Set_Mij(1, 0, -sin (c) * cos (b) * cos (a) - cos (c) * sin (a));
	Set_Mij(1, 1, -sin (c) * cos (b) * sin (a) + cos (c) * cos (a));
	Set_Mij(1, 2, sin (c) * sin (b));
	Set_Mij(2, 0, cos (a) * sin (b));
	Set_Mij(2, 1, sin (a) * sin (b));
	Set_Mij(2, 2, cos (b));
}

/*Rotation::Rotation (const Matrix &r) {
	Matrix(3,3);
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			M[i][j] = r.M[i][j];
}*/

Rotation::Rotation (const Matrix &r) : Matrix(r) {}

void Rotation::decompose(double &a, double &b, double &c){
	double alpha, beta, gamma;
	int flag;

	flag = findAngles(alpha, beta, gamma);

	if(flag == 1)
	{
		a = alpha;
		b = beta;
		c = gamma;
	} else	{
		Set_Mij(2, 0, -1 * Get_Mij(2, 0));
		Set_Mij(2, 1, -1 * Get_Mij(2, 1));
		Set_Mij(2, 2, -1 * Get_Mij(2, 2));

		flag = findAngles(alpha, beta, gamma);

		Set_Mij(2, 0, -1 * Get_Mij(2, 0));
		Set_Mij(2, 1, -1 * Get_Mij(2, 1));
		Set_Mij(2, 2, -1 * Get_Mij(2, 2));

		if(flag == 1)
		{
			a = alpha;
			b = beta;
			c = gamma;
		} else {
			a = -1;
			b = -1;
			c = -1;
		}
	}
}

double Rotation::compare(double a, double b, double c){
	double R[9], diff;
	int i;

	R[0] = cosf (c) * cosf (b) * cosf (a) - sinf (c) * sinf (a);
	R[1] = cosf (c) * cosf (b) * sinf (a) + sinf (c) * cosf (a);
	R[2] = -cosf (c) * sinf (b);
	R[3] = -sinf (c) * cosf (b) * cosf (a) - cosf (c) * sinf (a);
	R[4] = -sinf (c) * cosf (b) * sinf (a) + cosf (c) * cosf (a);
	R[5] = sinf (c) * sinf (b);
	R[6] = cosf (a) * sinf (b);
	R[7] = sinf (a) * sinf (b);
	R[8] = cosf (b);

	diff = 0;
	for(i =0; i < 9; i++) 
		diff += fabs(Get_Mij(i / 3, i % 3) - R[i]);

        return( diff );
}

int Rotation::findAngles(double &alpha, double &beta, double &gamma){
	double a, b, c, diff;
	double Solution[6][3];
	int flag, i;

        if(Get_Mij(2, 2) == 1.0) {
		b = 0;
		if(Get_Mij(0, 0) == 0) { 
			a = PI/2; 
			c = 0;
		} else {
                        a = atan(Get_Mij(0, 1)/Get_Mij(0, 0)); 
			c = 0;
		}
	} else {
	        b = acos(Get_Mij(2, 2));
	        a = atan(Get_Mij(2, 1)/Get_Mij(2, 0));
	        c = atan(-1*Get_Mij(1, 2)/Get_Mij(0, 2));
	}

	Solution[0][0]=a; Solution[0][1]=b; Solution[0][2]=c;
	Solution[1][0]=a+PI; Solution[1][1]=b; Solution[1][2]=c;
	Solution[2][0]=a; Solution[2][1]=2*PI-b; Solution[2][2]=c;
	Solution[3][0]=a; Solution[3][1]=b; Solution[3][2]=c+PI;
	Solution[4][0]=a+PI; Solution[4][1]=2*PI-b; Solution[4][2]=c;
	Solution[5][0]=a; Solution[5][1]=2*PI-b; Solution[5][2]=c+PI;

	flag = 0;
	for(i=0; i < 6; i++)
	{
		diff = compare(Solution[i][0], Solution[i][1], Solution[i][2]);
		if(diff < 0.1) 
		{
			flag = 1;
			alpha = Solution[i][0];
			beta  = Solution[i][1];
			gamma = Solution[i][2];
			return(flag);
		} 

	}
        return(flag);
}
