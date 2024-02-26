/*
 * 
 * */
using namespace std;
class Point
{
	private:
	float x,y,z;
	public:
	Point(float,float,float);
	float getX();
	float getY();
	float getZ();
	void show();
};
class Vector
{
	private :
	float x,y,z,magitude;
	public:
	Vector();
	Vector(Point,Point);
	float getX();
	float getY();
	float getZ();
	float getMag();
	void Cross(Vector,Vector);
	float Dot(Vector);
	void UnitVec();
	void Print();
	//float findAngle(Vector, Vector );
	
	
	
};
