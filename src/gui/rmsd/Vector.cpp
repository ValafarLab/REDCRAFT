/*
 * 
 * */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>
#include "Matrix.h"
#include "Vector.h"

using namespace std;
Point::Point(float a, float b, float c)
{
		x=a;
		y=b;
		z=c;
}
float Point::getX()
{
	return x;
}
float Point::getY()
{
	return y;
}
float Point::getZ()
{
	return z;
}
void Point::show()
{
	cout<<"["<<x<<","<<y<<","<<z<<"]"<<endl;
}
Vector::Vector()
{
	x=0;
	y=0;
	z=0;
	magitude=0;
}
Vector::Vector(Point a, Point b)
{
	x=(b.getX()-a.getX());
	y=(b.getY()-a.getY());
	z=(b.getZ()-a.getZ());
	magitude=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	
	
}
float Vector::getX()
{
	return x;
}
float Vector::getY()
{
	return y;
}
float Vector::getZ()
{
	return z;
}
float Vector::getMag()
{
	return magitude;
}
void Vector::Cross(Vector A, Vector B)
{
	x=A.getY()*B.getZ()-A.getZ()*B.getY();
	y=A.getZ()*B.getX()-A.getX()*B.getZ();
	z=A.getX()*B.getY()-A.getY()*B.getX();
	magitude=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}
float Vector::Dot(Vector B)
{
	return x*B.getX()+y*B.getY()+z*B.getZ();
}
void Vector::Print()
{
	cout<<x<<" "<<y<<" "<<z<<" "<<magitude<<endl;
}
void Vector::UnitVec()
{
	x=x/magitude;
	y=y/magitude;
	z=z/magitude;
	magitude=magitude/magitude;
}

