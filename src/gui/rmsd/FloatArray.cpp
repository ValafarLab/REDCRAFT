/***************************************************************************
 *   Copyright (C) 2013 by Casey Cole and Earron Twitty                    *
 *                                                                         *
 *   This cpp file implements an array of floats for use in various        *
 *   parts of our project.                                                 *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include <cstdlib>
#include "FloatArray.h"

using namespace std;

//Default constructor
FloatArray :: FloatArray()
{
	size = 10; 
	value = 0.0;
	initArray();
}

//constructor that takes in a size
FloatArray :: FloatArray(int big)
{
	if(big >= 0) size = big;
	value = 0.0;
	initArray();
}

//constructor that takes in a size and an initial value
FloatArray :: FloatArray(int big, float num)
{
	if(big >= 0) size = big;
	value = num;
	initArray();
}

//copy constructor
FloatArray :: FloatArray(const FloatArray& array2)
{
    size = array2.getSize();
    array = new float[size];
   
    for(int i = 0; i < size; i++)
    {
        array[i] = array2(i);
    }
}

//Accessors and Mutators
int FloatArray :: getSize() const
{
	return size;
}


//Creates the array and initializes the values at each point in the array. 
void FloatArray :: initArray()
{
	array = new float[size];
	for(int i = 0; i < size; i++)
	{
		array[i] = value;
	}
}

//Prints each element of the array. 
void FloatArray :: Print()
{
 	for(int i = 0; i < size; i++)
    	{
        	cout << array[i] << " ";
   	}
   	 cout << endl;
}

//Sets an element of an array (@ index) to a value (val) SET METHOD
void FloatArray :: operator()(int index, float val) const
{
    if(index >= getSize() || index < 0)
	{
		cout << "Index out of bounds, you lose!" << endl;
		exit(1);
	}
	array[index] = val;
}
	
//Gets a certain value in the array based on an index as long as it is not out of
//bounds or negative. GET METHOD
float FloatArray :: operator()(int index) const
{
   	if(index >= getSize() || index < 0)
	{
		cout << "Index out of bounds, you lose bro!" << endl;
		exit(1);
	}
	return array[index];
}

//copies the FloatArray on the rhs to the FloatArray on the lhs

FloatArray& FloatArray :: operator=(FloatArray array2)
{
    delete [] array;
    size = array2.getSize();
    value = 0.0;
    initArray();
    
	for(int i = 0; i < size; i++)
    {
        array[i] = array2(i);
    }
    return *this;
}

//performs a merge on the two objects involved, enables cascading by returning an FloatArray object. 
FloatArray FloatArray :: operator+(FloatArray& array2) const
{
    FloatArray temp(size + array2.getSize());
    for(int i = 0; i < size; i++)
    {
        temp(i, array[i]);
    }
    
    int num = 0;
    for(int i = size; i < size + array2.getSize(); i++)
    {
        temp(i, array2(num));
        num++;
    }
    
    return temp;
}

//performs a merge on the two objects involved, does not enable cascading. 
void FloatArray :: operator+=(FloatArray& array2)
{
    int tempSize = getSize();
    float* temp = new float[tempSize];
    for(int i = 0; i < size; i++)
    {
        temp[i] = array[i];
    }
    
    size += array2.getSize();
    delete [] array;
    initArray();
    for(int i = 0; i < tempSize; i++)
    {
        array[i] = temp[i];
    }
    
    int num = 0;
    for(int i = tempSize; i < size; i++)
    {
        array[i] = array2(num);
        num++;
    }
    
    delete [] temp;
    
}

//Tests two objects for equality. 
bool FloatArray :: operator==(FloatArray& array2)
{
    if(size != array2.getSize())
    {
        return false; 
    }
    else
    {
        for(int i = 0; i < size; i++)
        {
            if(array[i] != array2(i))
                return false;
        }
    }
    return true; 
}

//Tests two objects for inequality.
bool FloatArray :: operator!=(FloatArray& array2)
{
    if(size != array2.getSize())
    {
        return true;
    }
    else
    {
        for(int i = 0; i < size; i++)
        {
            if(array[i] != array2(i))
                return true;
        }
    }
    return false;
}

//Overloading pre-incrementation and adds one to each place in the array.
FloatArray& FloatArray :: operator++()
{
    for(int i = 0; i < size; i++)
    {
        array[i] += 1; 
    }
    return *this;
}


//Overloading the output stream to print my FloatArray object. 
ostream& operator<<(ostream& os, const FloatArray& array)
{
    for(int i = 0; i < array.getSize(); i++)
    {
        os << array(i) << " ";
    }
    return os;
}

//Overloading the input stream to put values into an instance of my FloatArray object. 
istream& operator>>(istream& os, const FloatArray& array)
{
    for(int i = 0; i < array.getSize(); i++)
    {
        int num = 0;
        os >> num;
        array(i, num);
    }
    return os;
}

//EXTERMINATE EXTERMINATE EXTERMINATE!
FloatArray :: ~FloatArray()
{
    delete [] array;
}

