/***************************************************************************
 *   Copyright (C) 2005-2011 by Dr. Homayoun Valafar (homayoun@cse.sc.edu) *
 *   Ryan Yandle, Arjang Fahim
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILI.MTY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include "Matrix.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define SMALL_NUMBER 0.000001

Matrix::Matrix(int row_size, int col_size, double Init_value)
        : row ( row_size ), col ( col_size ) {
    M  = new double *[row];

    for(int i = 0; i < row; i++) {
        M[i] = new double [col];
    }

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            M[i][j] = Init_value;

}

Matrix::Matrix(const Matrix & m1)
        : row(m1.row), col(m1.col) {
    M  = new double *[row];

    for(int i = 0; i < row; i++) {
        M[i] = new double [col];
    }

    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++) {
            M[i][j] = m1.M[i][j];
        }
}


Matrix::~Matrix() {
    for(int i = 0; i < row; i++) {
        delete [] M[i];
    }

    delete [] M;
}

ostream & operator<< (ostream & s, const Matrix & rhs) {

    //s.setf(ios::scientific);
    //s.precision(10);
    for (int i = 0; i < rhs.row; i++) {
        for (int j = 0; j < rhs.col; j++)
            s << rhs.M[i][j] << " ";
        s << endl;
    }
    return s;
}

istream & operator>> (istream & s, Matrix & rhs) {

    for (int i = 0; i < rhs.row; i++) {
        for (int j = 0; j < rhs.col; j++)
            s >> rhs.M[i][j];
    }
    return s;
}

Matrix Matrix::operator+ (const Matrix & rhs) {
    //Matrix *temp_ptr = new Matrix(*this);
    Matrix temp(*this);

    if ((row != rhs.row) || (col != rhs.col)) {
        cout << "Miss-matched dimensions in + operator.\n";
        exit(1);
    }

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            temp.M[i][j] = M[i][j] + rhs.M[i][j];

    return temp;
}

const Matrix & Matrix::operator= (const Matrix & rhs) {

    if ((row != rhs.row) || (col != rhs.col)) {
        cout << "Miss-matched dimensions in = operator.\n";
        exit(1);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++)
            M[i][j] = rhs.M[i][j];
    }
    return *this;
}

Matrix Matrix::operator* (const Matrix & rhs) {
    double s;
    //Matrix *temp_ptr = new Matrix(row, rhs.col);

    Matrix temp(row, rhs.col);

    if (col != rhs.row) {
        cout << "Miss-matched dimensions in * operator.\n";
        exit(1);
    }

    for (int i = 0; i < row; i++)
        for (int j = 0; j < rhs.col; j++) {
            s = 0.0;
            for (int k = 0; k < col; k++)
                s += M[i][k] * rhs.M[k][j];   //s.m[i][j] = 0 at initiation
            temp.M[i][j] = s;
        }

    return temp;
}

bool Matrix::operator== (const Matrix & rhs) {

    if ((row != rhs.row) || (col != rhs.col)) {
        cout << "Miss-matched dimensions in == operator.\n";
        exit(1);
    }

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if (M[i][j] != rhs.M[i][j])
                return (false);
    return (true);
}

bool Matrix::operator !=(const Matrix & rhs) {
    return (!(*this == rhs));
}

Matrix Matrix::Transpose() {
    //Matrix *temp;
    //temp = new Matrix(col,row,0);

    Matrix temp(col, row, 0);

    for (int i = 0; i < temp.row; i++)
        for (int j = 0; j < temp.col; j++)
            temp.M[i][j] = M[j][i];

    return temp;
}

Matrix * Matrix::pseudoInverse() {
	Matrix *u = new Matrix(GetRow(), GetCol());
	Matrix *sig = new Matrix(GetCol(), GetCol());
	Matrix *v = new  Matrix(GetCol(), GetCol());

	SVD(*u, *sig, *v);
	*sig = sig->Transpose();

	for(int i = 0; i < sig->GetRow(); i++){
		for(int j = 0; j < sig->GetCol(); j++){
			/*if(sig->M[i][j] > SMALL_NUMBER)
				sig->M[i][j] = 1 / sig->M[i][j];
			else sig->M[i][j] = 0;*/
		}
	}
	/*u->Print();
	sig->Print();
	v->Print();*/
	Matrix vuTrans = *v*u->Transpose();
	//vuTrans.Print();
	float detA = vuTrans.Get_Mij(0,0)*((vuTrans.Get_Mij(1,1)*vuTrans.Get_Mij(2,2))-(vuTrans.Get_Mij(1,2)*vuTrans.Get_Mij(2,1)));
	float detB = vuTrans.Get_Mij(0,1)*((vuTrans.Get_Mij(1,0)*vuTrans.Get_Mij(2,2))-(vuTrans.Get_Mij(1,2)*vuTrans.Get_Mij(2,0)));
	float detC = vuTrans.Get_Mij(0,2)*((vuTrans.Get_Mij(1,0)*vuTrans.Get_Mij(2,1))-(vuTrans.Get_Mij(1,1)*vuTrans.Get_Mij(2,0)));
	float det= detA-detB+detC;
	
	Matrix * pseudoInverse = new Matrix(GetCol(), GetRow());
	if(det>0)
	{
	*pseudoInverse = *v *u->Transpose();
	}
	else
	{
		Matrix d =(3,3);
		d.Set_Mij(0,0,1);
		d.Set_Mij(0,1,0);
		d.Set_Mij(0,2,0);
		d.Set_Mij(1,0,0);
		d.Set_Mij(1,1,1);
		d.Set_Mij(1,2,0);
		d.Set_Mij(2,0,0);
		d.Set_Mij(2,1,0);
		d.Set_Mij(2,2,-1);
		*pseudoInverse = *v*d*u->Transpose();
	}
		
	
	/*pseudoInverse->Print();
	(pseudoInverse->Transpose()*(*pseudoInverse)).Print();*/

	delete v;
	delete sig;
	delete u;

	return pseudoInverse;
}

void Matrix::SVD(Matrix & LI, Matrix & DI, Matrix & RI) {
    int flag, i, its, j, jj, k, l = 0, nm;
    double anorm, c, f, g, h, s, scale, x, y, z, *rv1;

    rv1 = (double *) malloc((col + 1) * sizeof(double));
    if (rv1 == NULL) {
        cout << "Error while allocating rv1.\n";
        exit(1);
    }

    LI = *this;

    g = scale = anorm = 0.0;
    for (i = 1; i <= col; i++) {
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i <= row) {
            for (k = i; k <= row; k++)
                scale += fabs(LI.M[k-1][i-1]);

            if (scale) {
                for (k = i; k <= row; k++) {
                    LI.M[k-1][i-1] /= scale;
                    s += LI.M[k-1][i-1] * LI.M[k-1][i-1];
                }
                f = LI.M[i-1][i-1];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                LI.M[i-1][i-1] = f - g;
                for (j = l; j <= col; j++) {
                    for (s = 0.0, k = i; k <= row; k++)
                        s += LI.M[k-1][i-1] * LI.M[k-1][j-1];
                    f = s / h;
                    for (k = i; k <= row; k++)
                        LI.M[k-1][j-1] += f * LI.M[k-1][i-1];
                }
                for (k = i; k <= row; k++)
                    LI.M[k-1][i-1] *= scale;
            }
        }
        DI.M[i-1][i-1] = scale * g;
        g = s = scale = 0.0;
        if (i <= row && i != col) {
            for (k = l; k <= col; k++)
                scale += fabs(LI.M[i-1][k-1]);
            if (scale) {
                for (k = l; k <= col; k++) {
                    LI.M[i-1][k-1] /= scale;
                    s += LI.M[i-1][k-1] * LI.M[i-1][k-1];
                }
                f = LI.M[i-1][l-1];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                LI.M[i-1][l-1] = f - g;
                for (k = l; k <= col; k++)
                    rv1[k] = LI.M[i-1][k-1] / h;
                for (j = l; j <= row; j++) {
                    for (s = 0.0, k = l; k <= col; k++)
                        s += LI.M[j-1][k-1] * LI.M[i-1][k-1];
                    for (k = l; k <= col; k++)
                        LI.M[j-1][k-1] += s * rv1[k];
                }
                for (k = l; k <= col; k++)
                    LI.M[i-1][k-1] *= scale;
            }
        }
        anorm = MAX(anorm, (fabs(DI.M[i-1][i-1]) + fabs(rv1[i])));
    }

    for (i = col; i >= 1; i--) {
        if (i < col) {
            if (g) {
                for (j = l; j <= col; j++)
                    RI.M[j-1][i-1] = (LI.M[i-1][j-1] / LI.M[i-1][l-1]) / g;
                for (j = l; j <= col; j++) {
                    for (s = 0.0, k = l; k <= col; k++)
                        s += LI.M[i-1][k-1] * RI.M[k-1][j-1];
                    for (k = l; k <= col; k++)
                        RI.M[k-1][j-1] += s * RI.M[k-1][i-1];
                }
            }
            for (j = l; j <= col; j++)
                RI.M[i-1][j-1] = RI.M[j-1][i-1] = 0.0;
        }
        RI.M[i-1][i-1] = 1.0;
        g = rv1[i];
        l = i;
    }

    for (i = MIN(row, col); i >= 1; i--) {
        l = i + 1;
        g = DI.M[i-1][i-1];
        for (j = l; j <= col; j++)
            LI.M[i-1][j-1] = 0.0;
        if (g) {
            g = 1.0 / g;
            for (j = l; j <= col; j++) {
                for (s = 0.0, k = l; k <= row; k++)
                    s += LI.M[k-1][i-1] * LI.M[k-1][j-1];
                f = (s / LI.M[i-1][i-1]) * g;
                for (k = i; k <= row; k++)
                    LI.M[k-1][j-1] += f * LI.M[k-1][i-1];
            }
            for (j = i; j <= row; j++)
                LI.M[j-1][i-1] *= g;
        } else
            for (j = i; j <= row; j++)
                LI.M[j-1][i-1] = 0.0;
        ++LI.M[i-1][i-1];
    }

    for (k = col; k >= 1; k--) {
        for (its = 1; its <= 30; its++) {
            flag = 1;
            for (l = k; l >= 1; l--) {
                nm = l - 1;
                if ((double ) (fabs(rv1[l]) + anorm) == anorm) {
                    flag = 0;
                    break;
                }
                if ((double) (fabs(DI.M[nm-1][nm-1]) + anorm) == anorm)
                    break;
            }
            if (flag) {
                c = 0.0;
                s = 1.0;
                for (i = l; i <= k; i++) {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if ((double) (fabs(f) + anorm) == anorm)
                        break;
                    g = DI.M[i-1][i-1];
                    h = PYTH(f, g);
                    DI.M[i-1][i-1] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;
                    for (j = 1; j <= row; j++) {
                        y = LI.M[j-1][nm-1];
                        z = LI.M[j-1][i-1];
                        LI.M[j-1][nm-1] = y * c + z * s;
                        LI.M[j-1][i-1] = z * c - y * s;
                    }
                }
            }
            z = DI.M[k-1][k-1];
            if (l == k) {
                if (z < 0.0) {
                    DI.M[k-1][k-1] = -z;
                    for (j = 1; j <= col; j++)
                        RI.M[j-1][k-1] = -RI.M[j-1][k-1];
                }
                break;
            }
            if (its == 30) {
                cout << "no convergence in 30 svdcmp iterations";
                exit(1);
            }
            x = DI.M[l-1][l-1];
            nm = k - 1;
            y = DI.M[nm-1][nm-1];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = PYTH(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
            c = s = 1.0;
            for (j = l; j <= nm; j++) {
                i = j + 1;
                g = rv1[i];
                y = DI.M[i-1][i-1];
                h = s * g;
                g = c * g;
                z = PYTH(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for (jj = 1; jj <= col; jj++) {
                    x = RI.M[jj-1][j-1];
                    z = RI.M[jj-1][i-1];
                    RI.M[jj-1][j-1] = x * c + z * s;
                    RI.M[jj-1][i-1] = z * c - x * s;
                }
                z = PYTH(f, h);
                DI.M[j-1][j-1] = z;
                if (z) {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = c * y - s * g;
                for (jj = 1; jj <= row; jj++) {
                    y = LI.M[jj-1][j-1];
                    z = LI.M[jj-1][i-1];
                    LI.M[jj-1][j-1] = y * c + z * s;
                    LI.M[jj-1][i-1] = z * c - y * s;
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            DI.M[k-1][k-1] = x;
        }

    }
    free(rv1);
}

int Matrix::Diag(Matrix &d, Matrix &v) {
    int j, iq, ip, i, nrot, n;
    double tresh, theta, tau, t, sm, s, h, g, c, *b, *z;
    double a[row + 1][col + 1];

    if(row != col) {
        cout << "Error: Matrix is not square\n";
        return -1;
    }

    for(i=0; i < row; i++)
        for(j=0; j < i; j++)
            if(M[i][j] != M[j][i]) {
                cout << "Error: Matrix not symmetric\n";
                return -1;
            }

    n = row;
    for(i=0; i < n; i++)
        for(j=0; j < n; j++)
            a[i+1][j+1] = M[i][j];

    b = (double *) malloc((n + 1) * sizeof(double));
    if (b == NULL) {
        printf("Error allocating b.\n");
        exit(1);
    }

    z = (double *) malloc((n + 1) * sizeof(double));
    if (z == NULL) {
        printf("Error allocating z.\n");
        exit(1);
    }

    for (ip = 1; ip <= n; ip++) {
        for (iq = 1; iq <= n; iq++)
            v.M[ip-1][iq-1] = 0.0;
        v.M[ip-1][ip-1] = 1.0;
    }

    for (ip = 1; ip <= n; ip++) {
        b[ip] = d.M[ip-1][ip-1] = a[ip][ip];
        z[ip] = 0.0;
    }

    nrot = 0;
    for (i = 1; i <= 50; i++) {
        sm = 0.0;
        for (ip = 1; ip <= n - 1; ip++) {
            for (iq = ip + 1; iq <= n; iq++)
                sm += fabs(a[ip][iq]);
        }
        if (sm == 0.0) {
            free(z);
            free(b);
            return nrot;
        }
        if (i < 4)
            tresh = 0.2 * sm / (n * n);

        else
            tresh = 0.0;
        for (ip = 1; ip <= n - 1; ip++) {
            for (iq = ip + 1; iq <= n; iq++) {
                g = 100.0 * fabs(a[ip][iq]);
                if (i > 4
                        && (double) (fabs(d.M[ip-1][ip-1]) + g) == (double) fabs(d.M[ip-1][ip-1])
                        && (double) (fabs(d.M[iq-1][iq-1]) + g) == (double) fabs(d.M[iq-1][iq-1]))
                    a[ip][iq] = 0.0;

                else if (fabs(a[ip][iq]) > tresh) {
                    h = d.M[iq-1][iq-1] - d.M[ip-1][ip-1];
                    if ((double) (fabs(h) + g) == (double) fabs(h))
                        t = (a[ip][iq]) / h;
                    else {
                        theta = 0.5 * h / (a[ip][iq]);
                        t =
                            1.0 / (fabs(theta) +
                                   sqrt(1.0 + theta * theta));
                        if (theta < 0.0)
                            t = -t;
                    }
                    c = 1.0 / sqrt(1 + t * t);
                    s = t * c;
                    tau = s / (1.0 + c);
                    h = t * a[ip][iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d.M[ip-1][ip-1] -= h;
                    d.M[iq-1][iq-1] += h;
                    a[ip][iq] = 0.0;
                    for (j = 1; j <= ip - 1; j++) {
                        ROTATE(a, j, ip, j, iq);
                    }
                    for (j = ip + 1; j <= iq - 1; j++) {
                        ROTATE(a, ip, j, j, iq);
                    }
                    for (j = iq + 1; j <= n; j++) {
                        ROTATE(a, ip, j, iq, j);
                    }
                    for (j = 1; j <= n; j++) {
                        ROTATE(v.M, j-1, ip-1, j-1, iq-1);
                    }
                    ++nrot;
                }
            }
        }
        for (ip = 1; ip <= n; ip++) {
            b[ip] += z[ip];
            d.M[ip-1][ip-1] = b[ip];
            z[ip] = 0.0;
        }
    }
    printf("Too many iterations in routine Diag without convergence\n");
    exit(1);
}

bool Matrix::isSquare() {
    if(row != col)
        return false;
    else
        return true;
}

bool Matrix::isSym() {
    if(!(this->isSquare()))
        return false;

    for(int i=0; i < row; i++)
        for(int j=0; j < col; j++)
            if(M[i][j] != M[j][i])
                return false;

    return true;
}

double Matrix::Trace() {
    double trace = 0;

    if(!(this->isSquare())) {
        cout << "Error: Matrix is not square\n";
        return -1;
    }

    for(int i=0; i < row; i++)
        trace += M[i][i];

    return trace;
}

void Matrix::AppendRows(Matrix &N){
	if(col != N.col){ cout << "Error: Array dimensions are incompatible\n"; return; }
	double **oldM = M;
	M = new double *[row + N.row];

	for(int i = 0; i < row; i++) M[i] = oldM[i];
	delete [] oldM;

	for(int i = row; i < row + N.row; i++) M[i] = N.M[i - row];
	delete [] N.M;
	N.M = NULL;
	row += N.row;
	N.row = 0;
}

void Matrix::Copy(Matrix &N){
	for(int i = 0; i < MIN(row, N.row); i++){
		for(int j = 0; j < MIN(col, N.col); j++){
			M[i][j] = N.M[i][j];
		}
	}
}

void Matrix::Print(){
	cout << "(" << row << ", " << col << ")\n";
	for(int i = 0; i < row; i++){
		cout << "[ ";
		for(int j = 0; j < col; j++){
			cout << M[i][j] << " ";
		}
		cout << "]\n";
	}
	
}
Matrix* Matrix::Kabsch(Matrix matrix2)
{
	Matrix* U;
	Matrix covar=this->Transpose()*matrix2;
	//covar.Print();
	Matrix *u = new Matrix(GetRow(), GetCol());
	Matrix *sig = new Matrix(GetCol(), GetCol());
	Matrix *v = new  Matrix(GetCol(), GetCol());

	SVD(*u, *sig, *v);
	
	
	return U;
}

