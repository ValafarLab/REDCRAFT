/***************************************************************************
 *   Copyright (C) 2013 by Casey Cole and Earron Twitty                    *
 *                                                                         *
 *   This header file maintains an array of floats for use in various      *
 *   parts of our project.                                                 *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
using namespace std;
class FloatArray
{
    friend ostream& operator<<(ostream& os, const FloatArray& array);
    friend istream& operator>>(istream& os, const FloatArray& array);
    
private:
	int size;
	float value;
	float* array;
    void initArray();
    
public:
    //constructors.
	FloatArray();
	FloatArray(int big);
	FloatArray(int big, float num);
    FloatArray(const FloatArray& array2);
    virtual ~FloatArray();
    
    //Print/Getters and Setters.
	void Print();
    void operator()(int index, float val) const;
    float operator()(int index) const;
	int getSize() const;
    
    //operator overloads. 
    FloatArray& operator=(FloatArray array2);
    FloatArray operator+(FloatArray& array2) const;
    void operator+=(FloatArray& array2);
    bool operator==(FloatArray& array2);
    bool operator!=(FloatArray& array2);
    FloatArray& operator++();
    FloatArray& operator++(int dummy); 

};




