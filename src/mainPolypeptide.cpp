#include <cstdlib>
#include "polypeptide.h"
#include <eigen/SVD>
#include <string>

int main(int argc, char *argv[]) {
    double angles[20] = {-125, 150, -130, 160, -170, 170, -160, 120, -160, 130,140,150,-70,100,65,80,150,-150,-175,-145};
    //double *angle = new double[180, 180, 180, 180, 180, 180, 180, 180, 180, 180];

    Polypeptide word(angles, 20);
   // word.print();
    for(int i=0;i<10;i++)
    {
    word.appendAminoAcid(150+i,150-i,"CPR",180);
    cout<<word.getAminoAcid(10+i)->getType()<<endl;
    }
    string yo ="Caleb Parks7896";
    cout<<atof(yo.c_str())<<endl;
    
    //word.writePDB("testPDB.pdb",1);
   /* double Omega=15;
    for (int i=0;i<word.getResidueCount();i++)
    {
        Omega+=10;
        word.setAngles(i,word.getAminoAcid(i)->getPhi(),word.getAminoAcid(i)->getPsi(),Omega);
    }
    word.updateAtoms(0);
    cout<<"\n";
    word.print();
    word.writePDB("test0PDB.pdb",1);
    
    
   Polypeptide bruh;
    for (int i = 0; i < 10; i += 2) {
        bruh.appendAminoAcid(double(angle[i]), double(angle[i + 1]));
    }
    word.print();
    cout << "\n";
    bruh.print();
    Polypeptide test;
    cout << "\n";
    test.print();
    test.appendAminoAcid(-170, 150);
    cout << "\n";
    test.print();
    test.appendAminoAcid(-170, 160);
    cout << "\n";
    test.print();
    word.writePDB("testPDB", 1);
    //-170 150 -170 160 -170 170 -160 120 -160 130
*/
    
}
