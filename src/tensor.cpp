/**
 * tensor.cpp Copyright 2005
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

#include "tensor.h"
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

Tensor::Tensor(double Sxx, double Sxy, double Sxz, double Syy, double Syz) {
  S.Set_Mij(0, 0, Sxx);
  S.Set_Mij(0, 1, Sxy);
  S.Set_Mij(0, 2, Sxz);

  S.Set_Mij(1, 0, Sxy);
  S.Set_Mij(1, 1, Syy);
  S.Set_Mij(1, 2, Syz);

  S.Set_Mij(2, 0, Sxz);
  S.Set_Mij(2, 1, Syz);
  S.Set_Mij(2, 2, -(Sxx+Syy));
}

Tensor::Tensor(const Tensor &T1)
{

  S = T1.S;

}

Tensor::Tensor(const Matrix &M)
{
	if (M.GetRow() == 3 && M.GetCol() == 3)
		S=M;
	else if (M.GetRow() == 5 && M.GetCol() == 1) 
		SetElements(		// SetElements() receives in Sxx Sxy Sxz Syy Syz
			M.Get_Mij(0,0),	// this can be changed. SetElements is used ONLY here
			M.Get_Mij(2,0),
			M.Get_Mij(3,0),
			M.Get_Mij(1,0),
			M.Get_Mij(4,0)
		);
	else {
		cerr << "Error: Tensor::Tensor(Matrix) : constructing a Tensor from a non 3x3 and non 5x1 Matrix:" << endl << M << endl;
		exit(1);
	}

}


Tensor::~Tensor()
{
}

ostream & operator<< (ostream & s, const Tensor & rhs) {

    s << rhs.S;
    return s;
}

istream & operator>> (istream & s, Tensor & rhs) {

    s >> rhs.S;
    return s;
}

Matrix & Tensor::getMatrix()
{
  return S;
}

void Tensor::CorrectAngles(double &a, double &b, double &c)
{
  a = (a > 180 ? a - 360 : a);
  b = (b > 180 ? b - 360 : b);
  c = (c > 180 ? c - 360 : c);
}

void Tensor::SetElements(double Sxx, double Sxy, double Sxz, double Syy, double Syz)
{
  S.Set_Mij(0, 0, Sxx);
  S.Set_Mij(0, 1, Sxy);
  S.Set_Mij(0, 2, Sxz);

  S.Set_Mij(1, 0, Sxy);
  S.Set_Mij(1, 1, Syy);
  S.Set_Mij(1, 2, Syz);

  S.Set_Mij(2, 0, Sxz);
  S.Set_Mij(2, 1, Syz);
  S.Set_Mij(2, 2, -(Sxx+Syy));
}

void Tensor::ConvR2EA() // not used in REDCRAFT
{
  double a, b, c;
  Matrix DiagonalS(3,3,0), R(3,3,0), RT(3,3,0);

  S.Diag(DiagonalS, R);

  RT = R.Transpose();

  if(RT.Get_Mij(2, 2) == 1.0)
  {
    b = 0;
    if(RT.Get_Mij(0, 0) == 0)
    {
      a = PI/2;
      c = 0;
    }
    else
    {
      a = atan2(RT.Get_Mij(0, 1),RT.Get_Mij(0, 0));
      c = 0;
    }
  }
  else
  {
    b = acos(RT.Get_Mij(2, 2));
    a = atan2(RT.Get_Mij(2, 1),RT.Get_Mij(2, 0));
    c = atan2(RT.Get_Mij(1, 2),-1*RT.Get_Mij(0, 2));
  }

//  SetEA(a*180/PI, b*180/PI, c*180/PI);
}

//write a function that returns the rotation matrix and the diagonalized matrix
//call this one printDecompose then have another two that return diagonalS and R
//pass it a pointer to a matrix header is void Decompose(tensor s, matrix r)
void Tensor::Decompose(Matrix & DiagonalS, Matrix & R) {
	Matrix P(3,3,0);

	S.Diag(DiagonalS, R);


	double x=fabs(DiagonalS.Get_Mij(0,0));
	double y=fabs(DiagonalS.Get_Mij(1,1));
	double z=fabs(DiagonalS.Get_Mij(2,2));
	//cout << "Decompose: x y z: " << x << " " << y << " " << z << endl;

	if(x<y && x<z && y>z) // X Z Y => 0 2 1
	{
		//cout << 0 << endl;
		P.Set_Mij(0,0,1);
		P.Set_Mij(2,1,1);
		P.Set_Mij(1,2,1);
	}

	else if(y<x && y<z) // Y ? ?
	{
		if(x<z)	// Y X Z => 1 0 2
		{
			//cout << 1 << endl;
			P.Set_Mij(1,0,1);
			P.Set_Mij(0,1,1);
			P.Set_Mij(2,2,1);
		}

		else // Y Z X > 1 2 0
		{
			//cout << 2 << endl;
			P.Set_Mij(1,0,1);
			P.Set_Mij(2,1,1);
			P.Set_Mij(0,2,1);
		}
	}

	else if(z<x && z<y) // Z ? ?
	{
		if(x<y)	// Z X Y => 2 0 1
		{
			//cout << 3 << endl;
			P.Set_Mij(2,0,1);
			P.Set_Mij(0,1,1);
			P.Set_Mij(1,2,1);
		}

		else	// Z Y X => 2 1 0
		{
			//cout << 4 << endl;
			P.Set_Mij(2,0,1);
			P.Set_Mij(1,1,1);
			P.Set_Mij(0,2,1);
		}
	}

	else	// X Y Z => 0 1 2
	{
		//cout << 5 << endl;
		P.Set_Mij(0,0,1);
		P.Set_Mij(1,1,1);
		P.Set_Mij(2,2,1);
	}
	DiagonalS = P.Transpose()*DiagonalS*P;
	R = R*P;

	if (R.Det() < 0)
	{
		double r=0;
		for(int i =0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				r = R.Get_Mij(i,j);
				R.Set_Mij(i,j,-r);
			}
		}
	}
}

void Tensor::Rotate(double a, double b, double c)
{
  Matrix R(3,3,0);
  //double k=0;

  //if(int(a)==40 && int(b)==50 && int(c)==60){
	//  k=1;

  //}

  a = a * PI / 180;
  b = b * PI / 180;
  c = c * PI / 180;

  R.Set_Mij(0, 0, cosf (c) * cosf (b) * cosf (a) - sinf (c) * sinf (a));
  R.Set_Mij(0, 1, cosf (c) * cosf (b) * sinf (a) + sinf (c) * cosf (a));
  R.Set_Mij(0, 2, -cosf (c) * sinf (b));
  R.Set_Mij(1, 0, -sinf (c) * cosf (b) * cosf (a) - cosf (c) * sinf (a));
  R.Set_Mij(1, 1, -sinf (c) * cosf (b) * sinf (a) + cosf (c) * cosf (a));
  R.Set_Mij(1, 2, sinf (c) * sinf (b));
  R.Set_Mij(2, 0, cosf (a) * sinf (b));
  R.Set_Mij(2, 1, sinf (a) * sinf (b));
  R.Set_Mij(2, 2, cosf (b));

  //debug

/*  if(k==1){
	//cout<<"ROTATION MATRIX:" <<endl;
	  cout<<R.Get_Mij(0,0)<<"\t"<<R.Get_Mij(0,1)<<"\t"<<R.Get_Mij(0,2)<<endl;
	  cout<<R.Get_Mij(1,0)<<"\t"<<R.Get_Mij(1,1)<<"\t"<<R.Get_Mij(1,2)<<endl;
	  cout<<R.Get_Mij(2,0)<<"\t"<<R.Get_Mij(2,1)<<"\t"<<R.Get_Mij(2,2)<<endl;
	  cout<<"\nS BEFORE ROTATION:"<<endl;
	  	  for(int i=0;i<S.GetRow();i++){
	  	  		for(int j=0;j<S.GetCol();j++){
	  	  			cout<<S.M[i][j]<<"\t";
	  	  		}
	  	  		cout<<endl;

	  	  	}
	 }

*/
  S = R * S * R.Transpose();
  /*if(k==1){
	  cout<<"S MATRIX FROM R ROTATION"<<endl;
	  for(int i=0;i<S.GetRow();i++){
	  		for(int j=0;j<S.GetCol();j++){
	  			cout<<S.M[i][j]<<"\t";
	  		}
	  		cout<<endl;

	  	}
  }*/
}

void Tensor::Rotate(Rotation r) {
	S = r * S * r.Transpose();
}
Tensor & Tensor::operator= ( Tensor & rhs)
{
  S = rhs.S;

  return *this;
}
void Tensor::Print()
{
	cout<<"Tensor\n";
	for(int i=0;i<S.GetRow();i++){
		for(int j=0;j<S.GetCol();j++){
			cout<<S.M[i][j]<<"\t";
		}
		cout<<endl;

	}




}

double Tensor::otmDistance(const Tensor & t)
{
	// dxx*dxx+dyy*dyy+dxx*dyy+dxy*dxy+dxz*dxz+dyz*dyz
	double dxx = getXX()-t.getXX();
	double dyy = getYY()-t.getYY();
	double dzz = getZZ()-t.getZZ();
	double dxy = getXY()-t.getXY();
	double dxz = getXZ()-t.getXZ();
	double dyz = getYZ()-t.getYZ();

	// return sqrt(4.0/15.0*(dxx*dxx+dyy*dyy+dxx*dyy+dxy*dxy+dxz*dxz+dyz*dyz));

	return sqrt(
		dxx*dxx + 
		2 * dxy*dxy +
		2 * dxz*dxz +

		dyy*dyy +
		2 * dyz*dyz +

		dzz*dzz
		);
}

/***********************************************************
void Tensor::Switch(int first, int second)
{
  double temp;

  temp = S->Get_Mij(first, first);
  S->Set_Mij(first, first, S->Get_Mij(second, second));
  S->Set_Mij(second, second, temp);

  for(int i=0; i < 3; i++)
  {
    temp = -1 * R->Get_Mij(i,first);
    R->Set_Mij(i,first, R->Get_Mij(i,second));
    R->Set_Mij(i, second, temp);
  }
  Sxx = S->Get_Mij(0, 0);
  Syy = S->Get_Mij(1, 1);
  Szz = S->Get_Mij(2, 2);
}

void Tensor::CorrectRH()
{
  double a, b, c, dot;

  a = R->Get_Mij(1,0) * R->Get_Mij(2,1) - R->Get_Mij(2,0) * R->Get_Mij(1,1);
  b = -(R->Get_Mij(0,0) * R->Get_Mij(2,1) - R->Get_Mij(0,1) * R->Get_Mij(2,0));
  c = R->Get_Mij(0,0) * R->Get_Mij(1,1) - R->Get_Mij(0,1) * R->Get_Mij(1,0);

  dot = a * R->Get_Mij(0,2) + b * R->Get_Mij(1,2) + c * R->Get_Mij(2,2);

  if(acos(dot) > PI/2)
  {
    cout << "Left handed transfer detected...Correcting\n";
    R->Set_Mij(0, 0, -1 * R->Get_Mij(0,0));
    R->Set_Mij(1, 0, -1 * R->Get_Mij(1,0));
    R->Set_Mij(2, 0, -1 * R->Get_Mij(2,0));
  }
}
*****************************************************************************/
vector<Tensor*> canonicalize_tensors(vector<Tensor*> inp) {
	int M = inp.size();
	vector<Tensor*> incanon;

	Matrix * rot = NULL; // Rotation from first alignment + second canon

	for (int m = 0; m < M; m++) {
		Tensor * fixed;
		if (m == 0) {
			// first alignment medium, get R
			Matrix S, R;
			inp[m]->Decompose(S, R);
			rot = new Matrix(R);
			fixed = new Tensor(S);
		} else {

			if (m == 1) {
				// second alignment medium, get canon matrix
				Matrix * canon = (
					rot->Transpose() *
					inp[m]->getMatrix() *
					*rot).canon();
				rot->operator =(*rot * *canon);
				delete canon;
			}

			fixed = new Tensor(
				rot->Transpose() *
				inp[m]->getMatrix() * 
				*rot);
		}
		//cout << "before: " << *inp[m] << endl;
		//cout << "after: " << *fixed << endl;
		incanon.push_back(fixed);
	}
	delete rot;
	return incanon;
}

double multi_tensor_distance(vector<Tensor*> A, vector<Tensor*> B) {
	if (A.size() != B.size()) {
		cerr << "Error: multi_tensor_distance(A,B) sizes do not match!" << endl;
		exit(EXIT_FAILURE);
	}
	vector<Tensor*> cA = canonicalize_tensors(A);
	vector<Tensor*> cB = canonicalize_tensors(B);
	double distance = 0;
	for (int i = 0; i < cA.size(); i++) {
		distance += cA[i]->otmDistance(*(cB[i]));
		delete cA[i];
		delete cB[i];
	}
	return distance;
}
