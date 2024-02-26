/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "polypeptide.h"
#include "aminoacid.h"
#include "atom.h"
#include <vector>

int main(int argc, char **argv)
{
    double angles[20] = {-125, 150, -130, 160, -170, 170, -160, 120, -160, 130,140,150,-70,100,65,80,150,-150,-175,-145};
    //double *angle = new double[180, 180, 180, 180, 180, 180, 180, 180, 180, 180];

    Polypeptide word(angles, 20);
    vector<int> a;
    vector<int> b;
    vector<int> c;
    for(int i=0;i<20*6;i++)
    {
        a.push_back(i%6);
        b.push_back((20*6-i)%6);
    }
    for (int i=0;i<20;i++)
    {
        c.push_back((i%8)+4);
    }
    
      int numAtoms=0;
     int skip=0;
    for(int i=1;i<word.getResidueCount();i++)
    {
       numAtoms=c[i];
       vector<int> temp1;
       vector<int> temp2;
       for(int j=0;j<numAtoms;j++)
       {
           temp1.push_back(a[skip+j]);
           temp2.push_back(b[skip+j]);

       }
       for(int j=0;j<numAtoms;j++)
       {
           cout<<temp1[j]<<" "<<temp2[j]<<endl;
       }
       cout<<'\n'<<endl;
       skip=skip+numAtoms;  
       
    }
    
}

