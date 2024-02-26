#include "gui.h"
#include "ui_gui.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>


Gui::Gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gui)
{
    ui->setupUi(this);
    QIntValidator* validator = new QIntValidator(this);
    ui->R1Start->setValidator(validator);
    ui->R1End->setValidator(validator);
    ui->R2Start->setValidator(validator);
    ui->R2End->setValidator(validator);


}

Gui::~Gui()
{
    delete ui;
}

void Gui::on_actionOpen_triggered()
{

    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("pdb(*.pdb)");
    QString file= dialog.getOpenFileName(0,"select PDB file",QString(),"PDB (*.pdb)");
    if(files.size()<2)
    {
        if(files.size()==0)
        {
            QStringList temp ;
            temp.append(file);
            files.append(file);
            modelFile1=new QStringListModel(this);
            modelFile1->setStringList(temp);
            ui->fileOneListView->setModel(modelFile1);
        }
        else if(files.size()==1)
        {
            QStringList temp ;
            temp.append(file);
            files.append(file);
            modelFile2=new QStringListModel(this);
            modelFile2->setStringList(temp);
            ui->fileTwoListView->setModel(modelFile2);
        }
    }

}
void Gui::addToList(QString data)
{
    lists<< data;
}
void Gui::Runback()
{
    modelDisplay = new QStringListModel(this);
    modelDisplay->setStringList(lists);
    ui->Display->setModel(modelDisplay);
}
void Gui::addToList2(QString data)
{
    lists2<< data;
}
void Gui::Runback2()
{
    modelDisplay = new QStringListModel(this);
    modelDisplay->setStringList(lists2);
    ui->Display->setModel(modelDisplay);
}
QString Gui::setReportFlag()
{
    ui->flag->activateWindow();
    QString reportFlag =ui->flag->currentText();
    return reportFlag;
}

void Gui::on_pushButton_2_clicked()
{
    QString empty="\0";
    QStringList Nil;
    Nil.append(empty);
    modelFile1=new QStringListModel(this);
    modelFile1->setStringList(Nil);
    ui->fileOneListView->setModel(modelFile1);
    modelFile2=new QStringListModel(this);
    modelFile2->setStringList(Nil);
    ui->fileTwoListView->setModel(modelFile2);
    files.clear();

}
void Gui::on_File1Show_clicked()
{
    if(ui->flag->currentText()!="Dihedral Angles in Degrees"&&ui->flag->currentText()!="Dihedral Angles in Radians")
    {
        return;
    }
    if(lists.size()==0)
    {
        QMessageBox::critical(this,"Error","Please Calculate the RMSD First");
        return;
    }
    QString empty="\0";
    QStringList Nil;
    Nil.append(empty);
    modelDisplay = new QStringListModel(this);
    modelDisplay->setStringList(Nil);
    ui->Display->setModel(modelDisplay);

    Runback();
}

void Gui::on_File2Show_clicked()
{
    if(ui->flag->currentText()!="Dihedral Angles in Degrees"&&ui->flag->currentText()!="Dihedral Angles in Radians")
    {
        return;
    }
    if(lists.size()==0)
    {
        QMessageBox::critical(this,"Error","Please Calculate the RMSD First");
        return;
    }
    QString empty="\0";
    QStringList Nil;
    Nil.append(empty);
    modelDisplay = new QStringListModel(this);
    modelDisplay->setStringList(Nil);
    ui->Display->setModel(modelDisplay);

    Runback2();
}


void Gui::on_calcRMSD_clicked()
{

    if(files.size()<2)
    {
        QMessageBox::critical(this,"Error","Please select 2 PDB files");
        return;
    }
    if(ui->R1Start->text()==""||ui->R1End->text()==""||ui->R2Start->text()==""||ui->R2End->text()=="")
    {
        QMessageBox::critical(this,"Error","Please enter a valid range");
        return;
    }
    else if (ui->R1Start->text().toInt()<=0||ui->R1End->text().toInt()<=0||ui->R2Start->text().toInt()<=0||ui->R2End->text().toInt()<=0)
    {
        QMessageBox::critical(this,"Error","Please only use values greater than 0");
        return;
    }
    if((ui->flag->currentText()=="RMSD per Residue"||ui->flag->currentText()=="Dihedral Angles in Radians"||ui->flag->currentText()=="Dihedral Angles in Degrees")
            &&ui->Atoms->currentText()!="BackBone")
    {
        QMessageBox::critical(this,"Error","RMSD per Atoms is the only report avaliable when the BackBone atoms are not selected");
        return;
    }
    ifstream  inFile2;
    ifstream inFile;
    const  std::string file = files.at(0).toStdString();
    const char *fileA=file.c_str();

    std::string file2 = files.at(1).toStdString();
    const char *fileB=file2.c_str();

    inFile.open(fileA);
    inFile2.open(fileB);
    int startP1=ui->R1Start->text().toInt(),startP2=ui->R2Start->text().toInt(),endP1=ui->R1End->text().toInt(),endP2=ui->R2End->text().toInt();

    if(endP1<startP1||endP2<startP2)
    {
        QMessageBox::critical(this,"Error","Please enter a valid range");
        return;

    }
    if(endP1-startP1!=endP2-startP2)
    {
        QMessageBox::critical(this,"Error","Ranges must be equal in size");
        return;
    }
    if (!inFile || !inFile2)
    {
        QMessageBox::critical(this,"Error","Unable to open Files");
        return;
    }
    lists.clear();
    lists2.clear();
    Runback();
    string selectAtoms;
    if(ui->Atoms->currentText()=="BackBone")
    {
        selectAtoms="bb";
    }
    else if (ui->Atoms->currentText()=="Sidechain")
    {
        selectAtoms="sc";
    }
    else if (ui->Atoms->currentText()=="All")
    {
        selectAtoms="all";
    }
    int option=0;
    if(ui->operation->currentText()=="Translation")
    {
        option=1;
    }
    else if(ui->operation->currentText()=="Rotation")
    {
        option=2;
    }
    else if(ui->operation->currentText()=="Translation and Rotation")
    {
        option=3;
    }
    string reportflag;

    if (ui->flag->currentText()=="No Printout")
    {
        reportflag="\0";
    }
    else if (ui->flag->currentText()=="RMSD per Residue")
    {
        reportflag="-r";
    }
    else if (ui->flag->currentText()=="RMSD per Atom")
    {
        reportflag="-a";
    }
    else if(ui->flag->currentText()=="Dihedral Angles in Radians")
    {
        reportflag="-d";
    }
    else if (ui->flag->currentText()=="Dihedral Angles in Degrees")
    {
        reportflag="-D";
    }

    PDBParser parser(inFile, startP1, endP1, option, selectAtoms), parser2(inFile2, startP2, endP2, option, selectAtoms);
    bool toStop=Evaluate(parser,parser2);
    bool stop=Evaluate(parser,parser2,endP1,endP2,startP1,startP2);

    if(toStop==true)
    {
        return;
    }
    if(stop==true)
    {
        return;
    }

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
    parser.addFileName(fileA);
   bool atomsInB= parser2.fillMatrix(matrix2,matrixB);
    parser2.addFileName(fileB);

    if(atomsInA==false)
    {
        QString e="There are none of the selected atoms in "+files.at(0);
        QMessageBox::critical(this,"Error",e);
        return;
    }
    else if(atomsInB==false)
    {
        QString r="There are none of the selected atoms in "+files.at(1);
        QMessageBox::critical(this,"Error",r);
        return;
    }

    float rmsd;

    if(option == 2 || option == 3)
    {
        //cout << "Rotation enabled" << endl;
        Matrix * matPt;
        Matrix covarMatrix = matrix.Transpose()*matrix2;
        //covarMatrix.Print();
        matPt = covarMatrix.pseudoInverse();

       // matrix.Print();
       // matrix2.Print();
        //u =  matrix2*(*matPt);
        //u.Print();
        Matrix matrix3(matrix.GetRow(), 3, 0.0);
        //matrix3 = matrix * u;
        matrix3 = matrix2 * (*matPt);
        if(reportflag=="-d"||reportflag=="-D")
        {

            Angles(matrix,parser,reportflag);
            Angles2(matrix2,parser2,reportflag);
            Runback();
        }

        rmsd = calcRMSD(matrix, matrix3, reportflag);
        if(ui->printBox->currentText()=="Yes")
        {
            parser.formatWrite(matrix,selectAtoms,matrixA,endP1,startP1);
            parser2.formatWrite(matrix3,selectAtoms,matrixB,endP2,startP2);
        }
        else{
            parser.checkRes(matrix,selectAtoms,matrixA,endP1,startP1);
            parser2.checkRes(matrix3,selectAtoms,matrixB,endP2,startP2);
        }

    }
    else
    {
        //matrix.Print();
        //matrix2.Print();
        if(reportflag=="-d"||reportflag=="-D")
        {
            Angles(matrix,parser,reportflag);
            Angles2(matrix2,parser2,reportflag);
            Runback();
        }
        rmsd = calcRMSD(matrix, matrix2, reportflag);

        if(ui->printBox->currentText()=="Yes")
        {
            parser.formatWrite(matrix,selectAtoms,matrixA,endP1,startP1);
            parser2.formatWrite(matrix2,selectAtoms,matrixB,endP2,startP2);
        }
       else{
            parser.checkRes(matrix,selectAtoms,matrixA,endP1,startP1);
            parser2.checkRes(matrix2,selectAtoms,matrixB,endP2,startP2);

        }
    }


    // cout << "The rmsd is : " << rmsd << endl;
    QStringList temp ;
    temp.append(QString::number(rmsd));
    modelRMSD=new QStringListModel(this);
    modelRMSD->setStringList(temp);
    ui->RMSD->setModel(modelRMSD);
    inFile.close();
    inFile2.close();



}
bool Gui::Evaluate(PDBParser parser, PDBParser parser2,int endP1,int endP2,int startP1,int startP2)
{
    if(endP1<=parser.getResInt()&&endP2<=parser2.getResInt())
    {
        return false;
    }
/*    if((parser.getResInt()<endP1)&&(parser2.getResInt()<endP2))
    {
        if(parser.getResInt()<parser2.getResInt())
        {
            QString a="Residue out of bounds. Use ";
            QString b=QString::number(parser.getResInt());
            QString c = " residues or less";
            QString d=a+b+c;
            QMessageBox::critical(this,"Error",d);
            return true;
        }
        else
        {
            QString a="Residue out of bounds. Use ";
            QString b=QString::number(parser2.getResInt());
            QString c = " residues or less";
            QString d=a+b+c;
            QMessageBox::critical(this,"Error",d);
            return true;
        }

    }*/
   bool first, second;
    if (parser.getResInt()<endP1)
    {
        QString a="Residue out of bounds. Use ";
        QString b=QString::number(parser.getResInt());
        QString c = " residues or less for the first residue range";
        QString d=a+b+c;
        QMessageBox::critical(this,"Error",d);
        first= true;
    }
    if (parser2.getResInt()<endP2)
    {
        QString a="Residue out of bounds. Use ";
        QString b=QString::number(parser2.getResInt());
        QString c = " residues or less for the second residue range";
        QString d=a+b+c;
        QMessageBox::critical(this,"Error",d);
        second =true;
    }
    if(second||first)
        return true;
    return false;

}
bool Gui::Evaluate(PDBParser A, PDBParser B){
    if(ui->R1Start->text().toInt()>=A.getStart()&&ui->R2Start->text().toInt()>=B.getStart())
    {
        return false;
    }
    bool first,second;
    if(ui->R1Start->text().toInt()<A.getStart())
    {
        QString a="Residue out of bounds. Use ";
        QString b=QString::number(A.getStart());
        QString c = " residues or more for the first residue range";
        QString d=a+b+c;
        QMessageBox::critical(this,"Error",d);
        first=true;
    }
    if(ui->R2Start->text().toInt()<B.getStart())
    {
        QString a="Residue out of bounds. Use ";
        QString b=QString::number(B.getStart());
        QString c = " residues or more for the second residue range";
        QString d=a+b+c;
        QMessageBox::critical(this,"Error",d);
        second= true;
    }
    if(second||first)
        return true;
    return false;
}

float Gui::calcDist(float x1, float x2, float y1, float y2, float z1, float z2)
{
    float dist = pow((x2-x1), 2) + pow((y2-y1), 2) + pow((z2-z1), 2);

    return dist;

}
float Gui::calcRMSD(Matrix one, Matrix two, string reportFlag)
{
    QString info;
    if(reportFlag=="-r")
    {
        info="Residue: RMSD per Residue: Current RMSD Total";
        addToList(info);
    }
    else if (reportFlag=="-a")
    {
        info="Atom: RMSD per Atom: Current RMSD Total";
        addToList(info);
    }
    int numRows = one.GetRow();
    //cout << "This is num of rows : " <<numRows << endl;
    float x1, x2, y1, y2, z1, z2, dist(0.0), rmsd(0.0), distRes(0.0);

    for(int i = 0; i <=numRows; i++)
    {
        if(i % 3 == 0 && reportFlag=="-r" && i != 0)
        {
            float currRMSD = sqrt(dist / i);
            //cout << "Residue " << i/4 << ": Current RMSD Total: " <<currRMSD << " RMSD For This Residue: " << sqrt(distRes/i) << endl;
            QString a=QString::number(i/3)+": "+QString::number(sqrt(distRes/i))+": "+QString::number(currRMSD);
            distRes = 0.0;
            addToList(a);
        }
        else if(reportFlag=="-a" &&i!=0)
        {
            float currRMSD = sqrt(dist / i);
            // cout << "Atom " << (i) << ": Current RMSD Total: " <<currRMSD << " RMSD For This Atom: " << sqrt(distRes/i) << endl;
            QString a=QString::number(i)+": "+QString::number(sqrt(distRes/i))+": "+QString::number(currRMSD);
            distRes = 0.0;
            addToList(a);
        }
        if(i==numRows)
        {
            break;
        }
        x1 = one.Get_Mij(i, 0);
        y1 = one.Get_Mij(i, 1);
        z1 = one.Get_Mij(i, 2);

        x2 = two.Get_Mij(i, 0);
        y2 = two.Get_Mij(i, 1);
        z2 = two.Get_Mij(i, 2);

        dist += calcDist(x1, x2, y1, y2, z1, z2);
        distRes += calcDist(x1, x2, y1, y2, z1, z2);
    }
    dist /= numRows;
    rmsd = sqrt(dist);
    Runback();
    return rmsd;

}
void Gui::Angles(Matrix A, PDBParser D,string report)
{

    if(A.NumRows()<4)
    {
        QString crt="There are not enough atoms present to calculate any angles";
        QMessageBox::critical(this,"Error",crt);
        return;
    }
    int res=ui->R1Start->text().toInt()-1;
    float phi=0,psi=0,omega=0;
    float oldPhi=0, oldPsi=0,oldO=0;
    QString a="Residue:Chain:Phi Angle:Psi Angle:Omega Angle";
    addToList(a);
    for (int i =0; i<A.NumRows()-3;i++)
    {
        oldPhi=phi;
        oldPsi=psi;
        oldO=omega;
        Point p1(A.Get_Mij(i,0),A.Get_Mij(i,1),A.Get_Mij(i,2));
        Point p2(A.Get_Mij(i+1,0),A.Get_Mij(i+1,1),A.Get_Mij(i+1,2));
        Point p3(A.Get_Mij(i+2,0),A.Get_Mij(i+2,1),A.Get_Mij(i+2,2));
        Point p4(A.Get_Mij(i+3,0),A.Get_Mij(i+3,1),A.Get_Mij(i+3,2));
        /*p1.show();
        p2.show();
        p3.show();
        p4.show();*/
        //cout<<D.getAtom(i)<<endl;
        Vector V1(p1,p2);
        Vector V2(p2,p3);
        Vector V3(p3,p4);
        V1.UnitVec();
        V2.UnitVec();
        V3.UnitVec();
        Vector C1;
        Vector C2;
        C1.Cross(V1,V2);
        //C1.Print();
        C2.Cross(V2,V3);
        //C2.Print();
        float angle;
        Vector S;
        S.Cross(C1,C2);
        float a=S.Dot(V2);
        float b=C1.Dot(C2);
        angle=atan2(a,b);
        if(report=="-D")
            angle=atan2(a,b)*(180/M_PI);
        if((D.getAtom(i+1)=="N "&&D.getAtom(i+2)=="CA"))
        {
            phi=angle;
        }
        else if((D.getAtom(i+1)=="CA"&&D.getAtom(i+2)=="C "))
        {
            psi=angle;

        }
        else if(D.getAtom(i+1)=="C "&&D.getAtom(i+2)=="N ")
        {
            omega=angle;
        }
        if(i==1)
        {
            res++;
            const QString data =QString::number(res)+": "+QString(D.getChain(i).c_str())+": "+QString::number(phi)+
                    ": "+QString::number(psi)+": "+QString::number(omega);
            addToList(data);

            // qDebug()<<data;

        }
        /*else if (i==A.NumRows()-2)
        {
            res++;
            if(oldPhi==phi)
            {
                phi=0;
            }
            if (oldPsi==psi)
            {
                psi=0;
            }

            const QString data =QString::number(res)+": "+QString(D.getChain(i).c_str())+": "+QString::number(phi)+
                    ": "+QString::number(psi)+": "+QString::number(omega);
            addToList(data);
              qDebug()<<QString::number(i);
            // qDebug()<<data;
        }*/
        else if(i%3==1&&i>0)
        {
                      // cout<<"i that becomes b: "<<i<<endl;
            res++;
            const QString data =QString::number(res)+": "+QString(D.getChain(i).c_str())+": "+QString::number(phi)+
                    ": "+QString::number(psi)+": "+QString::number(omega);
            addToList(data);


        }
    }
    if(oldPhi==phi)
    {
        phi=0;
    }
    if (oldPsi==psi)
    {
        psi=0;
    }
    if(oldO==omega)
    {
        omega=0;
    }

   if(res!=D.getResInt())
    {
        res++;
        //        cout<<res<<": "<<D.getChain(A.NumRows()-1)<<": "<<phi<<
        //              ": "<<psi<<": "<<omega<<endl;

        const QString data =QString::number(res)+": "+QString(D.getChain(A.NumRows()-1).c_str())+": "+QString::number(phi)+
                ": "+QString::number(psi)+": "+QString::number(omega);
        addToList(data);
    }
}
void Gui::Angles2(Matrix A, PDBParser D,string report)
{

    if(A.NumRows()<4)
    {

        return;
    }
    int res=ui->R2Start->text().toInt()-1;
    float phi=0,psi=0,omega=0;
    float oldPhi=0, oldPsi=0,oldO=0;
    QString a="Residue:Chain:Phi Angle:Psi Angle:Omega Angle";
    addToList2(a);
    for (int i =0; i<A.NumRows()-3;i++)
    {
        oldPhi=phi;
        oldPsi=psi;
        oldO=omega;
        Point p1(A.Get_Mij(i,0),A.Get_Mij(i,1),A.Get_Mij(i,2));
        Point p2(A.Get_Mij(i+1,0),A.Get_Mij(i+1,1),A.Get_Mij(i+1,2));
        Point p3(A.Get_Mij(i+2,0),A.Get_Mij(i+2,1),A.Get_Mij(i+2,2));
        Point p4(A.Get_Mij(i+3,0),A.Get_Mij(i+3,1),A.Get_Mij(i+3,2));
        /*p1.show();
        p2.show();
        p3.show();
        p4.show();*/
        //cout<<D.getAtom(i)<<endl;
        Vector V1(p1,p2);
        Vector V2(p2,p3);
        Vector V3(p3,p4);
        V1.UnitVec();
        V2.UnitVec();
        V3.UnitVec();
        Vector C1;
        Vector C2;
        C1.Cross(V1,V2);
        //C1.Print();
        C2.Cross(V2,V3);
        //C2.Print();
        float angle;
        Vector S;
        S.Cross(C1,C2);
        float a=S.Dot(V2);
        float b=C1.Dot(C2);
        angle=atan2(a,b);
        if(report=="-D")
            angle=atan2(a,b)*(180/M_PI);
        if(D.getAtom(i+1)=="N "&&D.getAtom(i+2)=="CA")
        {
            phi=angle;
        }
        else if(D.getAtom(i+1)=="CA"&&D.getAtom(i+2)=="C ")
        {
            psi=angle;
        }
        else if(D.getAtom(i+1)=="C "&&D.getAtom(i+2)=="N ")
        {
            omega=angle;
        }
        if(i==1)
        {
            res++;
            const QString data =QString::number(res)+": "+QString(D.getChain(i).c_str())+": "+QString::number(phi)+
                    ": "+QString::number(psi)+": "+QString::number(omega);
            addToList2(data);
            // qDebug()<<data;

        }
        /*else if (i==A.NumRows()-4)
        {
            res++;
            if(oldPhi==phi)
            {
                phi=0;
            }
            if (oldPsi==psi)
            {
                psi=0;
            }

            const QString data =QString::number(res)+": "+QString(D.getChain(i).c_str())+": "+QString::number(phi)+
                    ": "+QString::number(psi)+": "+QString::number(omega);
            addToList2(data);
            // qDebug()<<data;
        }*/
        else if(i%3==1&&i>0)
        {
          //  cout<<"i that becomes b: "<<i<<endl;
            res++;
            const QString data =QString::number(res)+": "+QString(D.getChain(i).c_str())+": "+QString::number(phi)+
                    ": "+QString::number(psi)+": "+QString::number(omega);
            addToList2(data);


        }

    }
    if(oldPhi==phi)
    {
        phi=0;
    }
    if (oldPsi==psi)
    {
        psi=0;
    }
    if(oldO==omega)
    {
        omega=0;
    }
    if(res!=D.getResInt())
    {
        res++;
        //        cout<<res<<": "<<D.getChain(A.NumRows()-1)<<": "<<phi<<
        //              ": "<<psi<<": "<<omega<<endl;

        const QString data =QString::number(res)+": "+QString(D.getChain(A.NumRows()-1).c_str())+": "+QString::number(phi)+
                ": "+QString::number(psi)+": "+QString::number(omega);
        addToList2(data);
    }
}



void Gui::on_Atoms_currentIndexChanged(int index)
{
    if(ui->Atoms->currentText()!="BackBone")
    {
        qobject_cast< QStandardItemModel * >( ui->flag->model() )->item( 1 )->setEnabled( false );
        qobject_cast< QStandardItemModel * >( ui->flag->model() )->item( 3 )->setEnabled( false );
        qobject_cast< QStandardItemModel * >( ui->flag->model() )->item( 4 )->setEnabled( false );
    }
    else
    {
        qobject_cast< QStandardItemModel * >( ui->flag->model() )->item( 1 )->setEnabled( true );
        qobject_cast< QStandardItemModel * >( ui->flag->model() )->item( 3 )->setEnabled( true );
        qobject_cast< QStandardItemModel * >( ui->flag->model() )->item( 4 )->setEnabled( true );
    }
}

void Gui::on_actionHelp_triggered()
{
    QMessageBox::about(this,"Welcome","1. Go to the file tab and select \"Open\". Select your first PDB file. Then go back to \"Open\" and select your second file. The order of the files can matter when the rotating option is used\n "
                                      "2. Select the atoms you want\n 3. Enter the residue ranges in both PDB's \n 4. Select what operation will be done to the files\n "
                                      "5. Select the report, which displays more detailed information in the right window\n6. Select if the modified PDB files is saved. Note it will be saved in "
                                      "the same folder the original files is from \n7. Calculate RMSD \nNote that any line that is not formatted correctly will be skipped making some results inaccurate");
    return;
}

void Gui::on_actionAbout_triggered()
{
    QMessageBox::about(this,"Credits","Gui written by Caleb Parks");
}

void Gui::on_actionExport_Angles_triggered()
{
    if(lists.size()==0||lists2.size()==0){
        QMessageBox::critical(this,"Error","Please Calculate RMSD with the 'Report' set to either 'Dihedral Angles in Degrees' or 'Dihedral Angles in Radians'");
        return;
    }
    std::string newFile;
    std::string newFile2;
    try{
        std::string file1String=files[0].toStdString();
        int i = file1String.find_last_of('/');
        if (i != string::npos)
            file1String = file1String.substr(i+1);
        //cout<<file1String<<endl;
         newFile=file1String.substr(0,file1String.find_last_of('.'))+".csv";
        //cout<<newFile<<endl;
         std::string file2String=files[1].toStdString();
         int j = file2String.find_last_of('/');
         if (j != string::npos)
             file2String = file2String.substr(j+1);
         //cout<<file2String<<endl;
         newFile2=file2String.substr(0,file2String.find_last_of('.'))+".csv";
         //cout<<newFile2<<endl;
        ifstream in1(newFile.c_str());
        ifstream in2(newFile2.c_str());
        bool stop=false;
        if(in1.good()||in2.good()){
           switch( QMessageBox::warning(this, "Warning", "One or both of these files already exists. Override These files?", QMessageBox::Yes |
                                        QMessageBox::No)){
           case QMessageBox::Yes:
                   stop=false;
                   break;
           case QMessageBox::No:
                   stop=true;
                   break;
           default:
                  stop=true;
                  break;
           }
        }
        if(stop){
            return;
        }
        ofstream out1(newFile.c_str());
        ofstream out2(newFile2.c_str());

        for(int a =0;a<lists.size();a++){
           // qDebug()<<lists[a];
            QString test = lists[a];
            std::string stdTest =test.toStdString();
            replace(stdTest.begin(),stdTest.end(),':',',');
            out1<<stdTest<<endl;
           // cout<<stdTest<<endl;
        }
        for(int a =0;a<lists2.size();a++){
           // qDebug()<<lists[a];
            QString test = lists2[a];
            std::string stdTest =test.toStdString();
            replace(stdTest.begin(),stdTest.end(),':',',');
            out2<<stdTest<<endl;
            //cout<<stdTest<<endl;
        }
    }
    catch (const std::exception &e) {
            QMessageBox::warning(this, "Error", e.what());
            return;
        }
    QMessageBox done;
    std::string finished="Successfully created "+newFile+" and "+newFile2;
    done.setText(QString::fromStdString(finished));
    done.exec();
}
