#ifndef ROTATION_H
#define ROTATION_H
#include <string>
#include <math.h>
#include "matrix.h"

#define PI 3.14159265

class Rotation : public Matrix {

public:
	static void printVersion(){ cout << "Rotation.....v1.0\n"; }

	Rotation (double x, double y, double z, double theta);
	Rotation (double a, double b, double c);
	Rotation (const Matrix &r); 
	void decompose(double &a, double &b, double &c);

protected:

private:
	int findAngles(double &alpha, double &beta, double &gamma);
	double compare(double a, double b, double c);

};
#endif //ROTATION_H
