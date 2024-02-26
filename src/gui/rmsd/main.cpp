
#include "gui.h"
#include <QApplication>
#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>


float calcDist(float x1, float x2, float y1, float y2, float z1, float z2)
{
    float dist = pow((x2-x1), 2) + pow((y2-y1), 2) + pow((z2-z1), 2);

    return dist;

}
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
int main(int argc, char *argv[])
{
    if(argc==1){
    QApplication a(argc, argv);
    Gui w;
    w.show();
    return a.exec();
    }
    else if ((argc>1&&argc<4)||argc>4){
        cout<<"Usage: "<<argv[0]<<" <start_residue> <stop_residue> <reference.pdb>"<<endl;
        exit(0);
    }
    //atof(xs.c_str())
    int starting=atoi(argv[1]);
    int ending=atoi(argv[2]);
    string file2=argv[3];
    const char *fileB=file2.c_str();
    ifstream inFile2;
    cout<<"Residue: Number of Structures: RDC RMSD: BackBone (N,C,CA) RMSD to "<<argv[3]<<endl;
    for (int i=starting;i<=ending;i++){
       const string file=to_string(i)+".pdb";
       string outFile=to_string(i)+".out";
       /* const char *fileA;
        fileA=file.c_str();*/
        string atoms="bb";
        ifstream  inFile;
        inFile.open(file);
        inFile2.open(fileB);
        if(!inFile.is_open()||!inFile2.is_open()){
            if(!inFile.is_open()){
                cout<<"unable to open "<<file<<endl;
            }
            if(!inFile2.is_open()){
                cout<<"unable to open "<<file2<<endl;
            }
            exit(0);
        }
        string cmd= "sed -n '/ATOM/s/ \\+/ /gp' "+file+" | cut -d' ' -f5 | head -n1";
        string cmd2= "sed -n '/ATOM/s/ \\+/ /gp' "+file+" | cut -d' ' -f5 | tail -n1";
        //cout<<cmd<<endl;
        string RealStart=exec(cmd.c_str());
        string RealEnd=exec(cmd2.c_str());
        int RealStartint=atoi(RealStart.c_str());
        int RealEndint=atoi(RealEnd.c_str());
        //cout<<RealStartint<<" "<<RealEndint<<" "<<realSize<<endl;
        cout<<i<<"  ";
        string command1= "head -1 "+outFile+" | awk '{printf $NF}'";
        string rdcRMSD=exec(command1.c_str());
        command1="wc "+outFile+" |sed -e 's/^[[:blank:]]*//' |cut -d' ' -f1";
        string wc=exec(command1.c_str());
        wc.erase(std::remove(wc.begin(), wc.end(), '\n'), wc.end());
        cout<<wc<<"  ";
        cout<<rdcRMSD<<"  ";
        PDBParser parser(inFile,RealStartint,RealEndint,3,atoms);
        PDBParser parser2(inFile2,RealStartint,RealEndint,3,atoms);
        int matrixSize=0;
        if(parser.getRealSize()<parser2.getRealSize())
        {
            matrixSize=parser.getRealSize();
            parser2.setRealSize(matrixSize);
        }
        else
        {
            matrixSize=parser2.getRealSize();
            parser.setRealSize(matrixSize);
        }
        Matrix matrix(matrixSize, 3, 0.0);
        Matrix matrix2(matrixSize, 3, 0.0);
        Matrix matrixA(matrixSize, 2, 0.0);
        Matrix matrixB(matrixSize, 2, 0.0);

       bool atomsInA =parser.fillMatrix(matrix,matrixA);
        parser.addFileName(file);
       bool atomsInB= parser2.fillMatrix(matrix2,matrixB);
        parser2.addFileName(argv[3]);
        Matrix * matPt;
        Matrix covarMatrix = matrix.Transpose()*matrix2;
        matPt = covarMatrix.pseudoInverse();
        Matrix matrix3(matrix.GetRow(), 3, 0.0);
        matrix3 = matrix2 * (*matPt);
        int numRows = matrix.GetRow();
        float x1, x2, y1, y2, z1, z2, dist(0.0), rmsd(0.0);
        for(int j = 0; j <numRows; j++)
        {
            x1 = matrix.Get_Mij(j, 0);
            y1 = matrix.Get_Mij(j, 1);
            z1 = matrix.Get_Mij(j, 2);

            x2 = matrix3.Get_Mij(j, 0);
            y2 = matrix3.Get_Mij(j, 1);
            z2 = matrix3.Get_Mij(j, 2);

            dist += calcDist(x1, x2, y1, y2, z1, z2);
           // cout<<dist<<endl;
        }
        dist /= numRows;
        rmsd = sqrt(dist);
        cout<<rmsd<<endl;
        inFile.close();
        inFile2.close();
    }


}
