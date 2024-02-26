/***************************************************************************
 *   Copyright (C) 2013 by Casey Cole and Earron Twitty                    *
 *                                                                         *
 *   This cpp file implements a PDB parser for use in various              *
 *   parts of our project.                                                 *
 *                                                                         *
 ***************************************************************************/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>
#include <QMessageBox>
#include<QDebug>
#include "FloatArray.h"
#include "PDBParser.h"
#include "Matrix.h"

using namespace std;

/* Constructor
 *
 * Parameters : An ifstream and two ints as a start and end in respect to residues.
 *
 */
PDBParser :: PDBParser(ifstream& inFile, int start, int end, int opt, string selectAtoms)
{


        size = ((end-start)+1)*500;


    
    realSize=0;
    
    x = FloatArray(size);
    y = FloatArray(size);
    z = FloatArray(size);
    w = FloatArray(size);
    v = FloatArray(size);
    
    option = opt;
    store= new string *[size];

    for (int i=0;i<size;i++)
    {
        store[i]=new string[5];
    }
    for (int i=0;i<size;i++)
    {
        for(int j=0;j<5;j++)
        {
            store[i][j]="?";
        }
    }

    readFile(inFile, start, end, selectAtoms);


    if(option == 1 || option == 3)
    {
        //cout << "Translation enabled" << endl;
        translate();
    }
    
}

/* Reads the file using ths ifstream, finds the x, y, z coordinates in the file and store them into
 * their respective FloatArray objects.
 *
 * Parameters : An ifstream and two ints as a start and end in respect to residues.
 *
 */
void PDBParser :: readFile(ifstream& inFile, int start, int end, string selectAtoms)
{
    Atoms=selectAtoms;

        size = ((end-start)+1)*500;


    string line = "";
    string xs, ys, zs, ws, vs;
    float xxx, yyy, zzz, www, vvv;
    int count = 0;
    
    while (!inFile.eof() && count < size)
    {
        getline(inFile, line);
        //cout<<stringSize(line)<<endl;
        string other,res,atom,type,num,chainID,Element;

        if(line.substr(0,6)=="ENDMDL"||line.substr(0,3)=="TER")
        {
            return;
        }
         other = line.substr(0, 6);
        if(other == "ATOM  ")
        {
             res = line.substr(22, 6);
            resInt = atoi(res.c_str());

            atom = line.substr(12, 4);
            type = line.substr(17, 3);
            num = line.substr(23, 3);
            chainID = line.substr(21, 1);
            Element = "?";
            if(line.size()>=77)
            {
                Element = line.substr(77, 2);
            }

            if(count==0)
            {
                Start=atoi(num.c_str());
            }

            //cout<<chainID+" "+Element+"\n"<<endl;



            trim(atom);

            PDBFill(atom,type,num,chainID,Element);

          //  cout<<atom+" "+type+" "+num+"\n";


            if(resInt >= start && resInt <=end && selectAtoms == "bb" && (atom == "N " || atom == "C " || atom == "CA"))
            {
                xs = line.substr(30, 8);
                xxx = atof(xs.c_str());
                x(count, xxx);

                ys = line.substr(38, 8);
                yyy = atof(ys.c_str());
                y(count, yyy);

                zs = line.substr(46, 8);
                zzz = atof(zs.c_str());
                z(count, zzz);

                ws = line.substr(54, 6);
                www = atof(ws.c_str());
                w(count, www);

                vs = line.substr(60, 6);
                vvv = atof(vs.c_str());
                v(count, vvv);
                count++;
                realSize++;

            }
            else if(resInt >= start && resInt <= end && selectAtoms == "all")
            {
                xs = line.substr(30, 8);
                xxx = atof(xs.c_str());
                x(count, xxx);

                ys = line.substr(38, 8);
                yyy = atof(ys.c_str());
                y(count, yyy);

                zs = line.substr(46, 8);
                zzz = atof(zs.c_str());
                z(count, zzz);

                ws = line.substr(54, 6);
                www = atof(ws.c_str());
                w(count, www);

                vs = line.substr(60, 6);
                vvv = atof(vs.c_str());
                v(count, vvv);

                count++;
                realSize++;

            }
            else if(resInt >= start && resInt <= end && selectAtoms == "sc" && (atom != "N " && atom != "C " && atom != "CA"&&atom!="CA "))
            {

                xs = line.substr(30, 8);
                xxx = atof(xs.c_str());
                x(count, xxx);

                ys = line.substr(38, 8);
                yyy = atof(ys.c_str());
                y(count, yyy);

                zs = line.substr(46, 8);
                zzz = atof(zs.c_str());
                z(count, zzz);

                ws = line.substr(54, 6);
                www = atof(ws.c_str());
                w(count, www);

                vs = line.substr(60, 6);
                vvv = atof(vs.c_str());
                v(count, vvv);
                count++;
                realSize++;

                // cout << count << " " << atom << " " << type << " " << num<< " " << xs << " " << ys << " " << zs << endl;
            }

        }

    }

}


/* Calculates the centroid of the x,y,z coordinates by averaging them. 
 *
 * Return : A vector of three floats <CentroidX, CentroidY, CentroidZ>
 *
 */
vector<float> PDBParser :: calcCentroid()
{
    vector<float> temp;
    float xxx(0.0), yyy(0.0), zzz(0.0);
    int n = realSize;
    
    for(int i = 0; i < n; i++)
    {
        xxx+= x(i);
        yyy+= y(i);
        zzz+= z(i);
    }
    xxx /= n;
    temp.push_back(xxx);
    yyy /= n;
    temp.push_back(yyy);
    zzz /= n;
    temp.push_back(zzz);

    return temp;
}

/* Translates each point in the system with respect to the centroid. 
 *
 */
void PDBParser :: translate()
{
    vector<float> centroid = calcCentroid();
    
    float centX = centroid[0];
    float centY = centroid[1];
    float centZ = centroid[2];
    
    for(int i = 0; i < size; i++)
    {
        x(i, (x(i) -centX));
        y(i, (y(i) -centY));
        z(i, (z(i) -centZ));
    }
}

/* Fills the matrix that is sent in with the (x, y, z) coordinates being stored in our
 * three FloatArrays
 *
 * Parameters : A reference to a Matrix (chuckNorris (yea i went there))
 *
 */
bool PDBParser :: fillMatrix(Matrix& chuckNorris, Matrix& b)
{
    if(x(0)==y(0)&&y(0)==z(0))
    {

        return false;
    }
    else if(isnan(x(0))||isnan(y(0))||isnan(z(0)))
    {
        return false;
    }
    for(int i = 0; i < realSize; i++)
    {
       // qDebug()<<QString::number(x(i))+QString::number(y(i))+QString::number(z(i));
        chuckNorris.Set_Mij(i, 0, x(i));
        chuckNorris.Set_Mij(i, 1, y(i));
        chuckNorris.Set_Mij(i, 2, z(i));
        b.Set_Mij(i,0,w(i));
        b.Set_Mij(i,1,v(i));

        
        
    }
    return true;
}



/* Private method to trim white space off of a string. 
 *
 * Parameters : Reference to a string to be snipped.
 *
 */
void PDBParser :: trim(string &s)
{
    for(int i = 0; i < s.length(); i++)
    {
        if(s.at(i) == ' ')
        {
            s.erase(i, 1);
        }
    }
}

/*Getter for size (actually returns the size of the FloatArrays...) 
 *
 */
int PDBParser :: getSize()
{
    return size;
}

void PDBParser::PDBFill(string atom, string type, string num, string chain, string element)
{
    for (int i=0;i<size;i++)
    {
        if(store[i][0]=="?")
        {
            store[i][0]=atom;
            store[i][1]=type;
            store[i][2]=num;
            store[i][3]=chain;
            store[i][4]=element;
            break;

        }
    }
   /* for (int i=0;i<size;i++)
    {
       cout<<"i: "<<i<<" "<<store[i][0]<< " "<<store[i][1]<< " "<<store[i][2]<< " "<<store[i][3]<< " "<<store[i][4]<<endl;
    }*/
}
void PDBParser::formatWrite(Matrix protein, string selectAtoms, Matrix protein2, int stop, int start)
{
    /*for (int i=0;i<size;i++)
    {
        if(store[i][0]!="?")
        {
            cout<<i;
            cout<<" "<<store[i][0]<<" "<<store[i][1]<<" "<<store[i][2]<<" "<<store[i][3]<<" "<<store[i][4]<<endl;
        }



    }*/
    int length=0;
    int count =1;
    int lastInt=0;
    int i=0;

    int a=0;//The a is for the matrix. We want to skip entries of the store[][] without skipping entries of our matrix
    ofstream myfile;
    string printFileName=fileName;
    int u=printFileName.size();
    printFileName.erase(u-4,4);
    printFileName.append("_");
    printFileName.append(Atoms);
    printFileName.append(".pdb");


    //string newFile=Atoms+"_"+fileName;
    myfile.open(printFileName.c_str());

    while(a<protein.GetRow())
    {
        if(atof(store[i][2].c_str())<start)
        {
            i++;
            continue;
        }


        if(atof(store[i][2].c_str())>stop)
        {
            return;
        }
        if(selectAtoms == "bb" && store[i][0] != "N " && store[i][0]!= "C " && store[i][0] != "CA")
        {
            i++;
            lastInt++;
            continue;
        }
        else if(selectAtoms == "sc" && (store[i][0] == "N " ||store[i][0]== "C " || store[i][0] == "CA"))
        {
            i++;
            lastInt++;
            continue;
        }
        if(store[i][0]=="?")
        {
            break;
        }
        if(a==protein.GetRow())
        {
            break;
        }


        length=stringSize(store[i][0]);
        if (count<10)
        {
            myfile<<"ATOM      ";

        }
        else if (count>=10&&count<100)
        {
            myfile<<"ATOM     ";
        }
        else if (count>=100&&count<1000)
        {
            myfile<<"ATOM    ";
        }
        else if (count>=1000)
        {
            myfile<<"ATOM   ";
        }

        myfile<<count;
        myfile<<"  ";
        //cout<<store[i][2]<<endl;
        if(length==1)
        {
            myfile<<store[i][0]+"   "+store[i][1]+" "
                    +""+store[i][3]+" "+store[i][2];
        }
        else if(length==2)
        {
            myfile<<store[i][0]+"  "+store[i][1]+" "
                    +""+store[i][3]+" "+store[i][2];
        }
        else if(length==3)
        {
            myfile<<store[i][0]+" "+store[i][1]+" "
                    +""+store[i][3]+" "+store[i][2];
        }
        else if(length==4)
        {
            myfile<<store[i][0]+""+store[i][1]+" "
                    +""+store[i][3]+" "+store[i][2];
        }
        myfile<<"    ";

        /**
             * This is where the matrix formating happends
             *
             *
             *
             *
             *
             */
        if(protein.Get_Mij(a,0)>=10)
        {
            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,0);
            myfile<<"   ";
        }
        else if(protein.Get_Mij(a,0)<0&&protein.Get_Mij(a,0)<=-10)
        {

            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,0);
            myfile<<"  ";
        }
        else if(protein.Get_Mij(a,0)<0)
        {

            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,0);
            myfile<<"   ";
        }

        else if(protein.Get_Mij(a,0)<0)
        {

            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,0);
            myfile<<"   ";
        }
        else
        {
            if(protein.Get_Mij(a,0)<1&&protein.Get_Mij(a,0)>=0)
            {
                myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,0);
                myfile<<"    ";

            }
            else
            {
                myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,0);
                myfile<<"    ";

            }
        }

        if(protein.Get_Mij(a,1)>=10)
        {
            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,1);
            myfile<<"   ";
        }
        else if(protein.Get_Mij(a,1)<0&&protein.Get_Mij(a,1)<=-10)
        {

            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,1);
            myfile<<"  ";
        }
        else if(protein.Get_Mij(a,1)<0)
        {

            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,1);
            myfile<<"   ";
        }
        else
        {
            if(protein.Get_Mij(a,0)<1&&protein.Get_Mij(a,1)>=0)
            {
                myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,1);
                myfile<<"    ";

            }
            else
            {
                myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,1);
                myfile<<"    ";

            }
        }

        if(protein.Get_Mij(a,2)<0&&protein.Get_Mij(a,2)>-10)
        {
            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,2);
            myfile<<"  ";
        }
        else if(protein.Get_Mij(a,2)<=-10)
        {
            myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,2);
            myfile<<" ";
        }

        else
        {
            if(protein.Get_Mij(a,2)<10&&protein.Get_Mij(a,2)>=0)
            {
                myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,2);
                myfile<<"   ";
            }
            else
            {
                myfile<<std::fixed<<std::setprecision(3)<<protein.Get_Mij(a,2);
                myfile<<"  ";
            }
        }

        if(protein2.Get_Mij(a,0)<0&&protein2.Get_Mij(a,0)>-10)
        {
            myfile<<std::fixed<<std::setprecision(2)<<protein2.Get_Mij(a,0);
            myfile<<"  ";
        }
        else if(protein2.Get_Mij(a,0)<=-10)
        {
            myfile<<std::fixed<<std::setprecision(2)<<protein2.Get_Mij(a,0);
            myfile<<" ";
        }

        else
        {
            if(protein2.Get_Mij(a,0)<10&&protein2.Get_Mij(a,0)>=0)
            {
                myfile<<std::fixed<<std::setprecision(2)<<protein2.Get_Mij(a,0);
                myfile<<"  ";
            }
            else
            {
                myfile<<std::fixed<<std::setprecision(2)<<protein2.Get_Mij(a,0);
                myfile<<"  ";
            }
        }

        if(protein2.Get_Mij(a,1)>10)
        {
            myfile<<std::fixed<<std::setprecision(2)<<protein2.Get_Mij(a,1);
            myfile<<" ";
        }
        else
        {
            myfile<<std::fixed<<std::setprecision(2)<<protein2.Get_Mij(a,1);
            myfile<<"  ";
        }
        if(store[i][4]!="?")
        {
            myfile<<"     "+store[i][4];
        }
        myfile<<"\n";
        //cout<<"Here"<<i<<" "<<a<<" "<<protein.Get_Mij(a,0)<<" "<<protein.Get_Mij(a,1)<<" "<<protein.Get_Mij(a,2)<<endl;

        a++;
        count++;
        lastInt++;
        i++;

    }

    if(atof(store[lastInt-1+bypass(start)][2].c_str())<stop&&selectAtoms!="bb")
    {
        QString a="Unable to compare all "+QString::number(stop-start)+" residues in" +fileName.c_str()+". Computing"+
                " rmsd for "+store[lastInt-1+bypass(start)][2].c_str()+" residues compared to "+QString::number(stop-start)+" residues";
        QMessageBox::warning(0,"Warning",a);
        //				cout<<"Unable to compare all "<<stop-start+1<<" residues in "
        //				<<fileName<<". Computing rmsd for "<< store[lastInt-1][2]<<" residues compared to "<<
        //                stop-start+1<<" residues"<<endl;
    }
    myfile<<"TER\nEND\n";


}
void PDBParser::addFileName(string q)
{
    fileName=q;

}

int PDBParser::stringSize(string a)
{
    int length=a.size();

    return length;
}
int PDBParser::getResInt()
{
    return resInt;
}
int PDBParser::getRealSize()
{
    return realSize;
}
void PDBParser::setRealSize(int b)
{
    realSize=b;
}
string PDBParser::getAtom(int b)
{
    int a=1;
    while(a<size)
    {
        if(store[a][0]=="?")
        {
            cout<<store[a][0]<<endl;
            return "\0";
        }
        if(b==0)
        {

            return store[a-1][0];

        }
        if(store[a][0] != "N " && store[a][0]!= "C " && store[a][0] != "CA" )
        {

            a++;
            continue;
        }
        else
        {
            a++;
            b--;
        }
    }
}
string PDBParser::getChain(int b)
{
    int a=bypass(getStart());
   // cout<<"start: "<<getStart()<<endl;
   // cout<< "b "<< b<<endl;
    while(a<size)
    {
        if(store[a][1]=="?")
        {
            //cout<<store[a][1]<<endl;
            return "ALA";
        }
        if(b==0)
        {
           // cout<<"a:" <<a<<endl;
            return store[a-1][1];

        }
        if(store[a][0] != "N " && store[a][0]!= "C " && store[a][0] != "CA" && store[a][0]!="CA ")
        {
            //cout<<"here a: "<<a<<" b: "<<b<<endl;
            a++;
            continue;
        }
        else
        {
            a++;
            b--;
        }
    }
}
void PDBParser::checkRes(Matrix protein, string selectAtoms, Matrix protein2, int stop, int start)
{

    int count =1;
    int lastInt=0;
    int i=0;

    int a=0;//The a is for the matrix. We want to skip entries of the store[][] without skipping entries of our matrix

    while(a<protein.GetRow())
    {
        if(store[i][0]=="?")
        {

            break;
        }
        if(a==protein.GetRow())
        {

            break;
        }
        if(atof(store[i][2].c_str())<start)
        {
            i++;
            continue;
        }


        if(atof(store[i][2].c_str())>stop)
        {

            return;
        }
        if(selectAtoms == "bb" && store[i][0] != "N " && store[i][0]!= "C " && store[i][0] != "CA")
        {
            i++;
            lastInt++;
            continue;
        }
        else if(selectAtoms == "sc" && (store[i][0] == "N " ||store[i][0]== "C " || store[i][0] == "CA"))
        {
            i++;
            lastInt++;
            continue;
        }

        a++;
        count++;
        lastInt++;
        i++;
    }
    if(lastInt-1+bypass(start)<size)
    {
        if(atof(store[lastInt-1+bypass(start)][2].c_str())<stop&&selectAtoms!="bb")
        {
            QString a="Unable to compare all "+QString::number(stop-start)+" residues in" +fileName.c_str()+". Computing"+
                    " rmsd for "+QString::number(atof(store[lastInt-1+bypass(start)][2].c_str())-start)+" residues compared to "+QString::number(stop-start)+" residues";
            QMessageBox::warning(0,"Warning",a);
            //				cout<<"Unable to compare all "<<stop-start+1<<" residues in "
            //				<<fileName<<". Computing rmsd for "<< store[lastInt-1][2]<<" residues compared to "<<
            //                stop-start+1<<" residues"<<endl;
        }
    }
}
int PDBParser::getStart(){
    return Start;
}
int PDBParser::bypass(int q)//pass in residue returns number of atoms before that residue
{
  for(int i=0;i<size;i++)
  {
      if(atof(store[i][2].c_str())==q)
      {
          return i;
      }
  }
}
