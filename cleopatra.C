/***********************************************************************
 * 
 *  This is Cleopatra, a sucessor of Ptolemy
 *          only for (d,p), (d,p), (d,d), or (p,p)
 * 
 *  1) it read a simple infile.in from reaction_setting file
 *  2) use Ptolemy to calculate the the creation
 *  3) extract the cross sec distribution into txt and root file
 * 
 * -----------------------------------------------------
 *  This program will call the root library and compile in g++
 *  compilation:
 *  g++ cleopatra.C -o cleopatra 'root-config --cflags --glibs'
 *
 *------------------------------------------------------
 *  The reaction_setting file is simple like:
 *
 *  206Hg(d,p)207Hg(1s1/2 0.000) 10MeV/u AK
 *
 *  the first is similar to usual reaction setting, the word AK is a
 *     short name for Optical Potential, user can put as many line as
 *     they like, Cleopatra can create the suitable infile.in for Ptolomy
 *
 * ------------------------------------------------------
 *  created by Ryan (Tsz Leung) Tang, Nov-18, 2018
 *  email: goluckyryan@gmail.com
 * ********************************************************************/

//#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <vector>
//#include <TROOT.h>
//#include <TFile.h>
//#include <TString.h>
//#include "nuclear_mass.h" // for geting Z

using namespace std;

//golbal varibale for Optical Potential
double v, r0, a;
double vi, ri0, ai;
double vsi, rsi0, asi;
double vso, rso0, aso;
double vsoi, rsoi0, asoi, rc0;

bool AnCaiPotential(int A, int Z, double E){
  // d + A(Z)
  // E < 183 or 81.5 MeV/u
  // 12 < A < 238
  // http://dx.doi.org/10.1103/PhysRevC.73.054605
  
  if( !(12 <= A &&  A <= 238 ) ) return false;
  if( E > 183 ) return false;

  double A3 = pow(A, 1./3.);

  v  = 91.85 - 0.249*E + 0.000116*pow(E,2) + 0.642 * Z / A3;
  r0 = 1.152 - 0.00776 / A3;
  a  = 0.719 + 0.0126 * A3;

  vi  = 1.104 + 0.0622 * E;
  ri0 = 1.305 + 0.0997 / A3;
  ai  = 0.855 - 0.1 * A3;

  vsi  = 10.83 - 0.0306 * E;
  rsi0 = 1.334 + 0.152 / A3;
  asi  = 0.531 + 0.062 * A3;

  vso  = 3.557;
  rso0 = 0.972;
  aso  = 1.011;

  vsoi  = 0.0;
  rsoi0 = 0.0;
  asoi  = 0.0;

  rc0 = 1.303;

  return true;
}

bool CalPotential(string potName, int A, int Z, double E){
  bool okFlag = false;

  AnCaiPotential(A, Z, E);
  
  return okFlag;
}

vector<string> SplitStr(string tempLine, string splitter, int shift = 0){

  vector<string> output;

  size_t pos;
  do{
    pos = tempLine.find(splitter, 1); // fine space
    if( pos == 1 ){ //check if it is space again
      tempLine = tempLine.substr(pos+1);
      continue;
    }

    string secStr;
    if( pos == string::npos ){
      secStr = tempLine;
    }else{
      secStr = tempLine.substr(0, pos+shift);
      tempLine = tempLine.substr(pos+shift);
    }

    //check if secStr is begin with space
    if( secStr.substr(0, 1) == " "){
      secStr = secStr.substr(1);
    }

    output.push_back(secStr);
    //printf(" |%s---\n", secStr.c_str());
    
  }while(pos != string::npos );

  return output;
}


int main (int argc, char *argv[]) {
   
  printf("=================================================================\n");
  printf("=====  Cleopatra, Ptolemy for (d,p),(p,d), (p,p) and (d,d)  =====\n");
  printf("=================================================================\n");

  if(argc != 2) { 
    printf("Usage: ./ExtractXSec input_file\n");
    exit(0); 
  }else{
    printf("From file : %s \n", argv[1]);
  }

  //================= read infile. extract the reactions
  ifstream file_in;
  file_in.open(argv[1], ios::in);

  if( !file_in ){
    printf(" cannot read file. \n");
    return 0 ; 
  }

  //extract information
  while( file_in.good() ) {
    string tempLine;
    getline(file_in, tempLine );

    if( tempLine.substr(0, 1) == "#" ) continue;
    if( tempLine.size() < 5 ) continue;

    printf(" %s", tempLine.c_str());

    //split line using space
    vector<string> str0 = SplitStr(tempLine, " ");

    //printf("size :%d \n", (int) sTempLine.size() );

    vector<string> str1 = SplitStr(str0[0], "(");
    vector<string> str2 = SplitStr(str1[1], ")", 1);

    if( !(str2[0] == "(d,d)" || str2[0] =="(p,p)" || str2[0] == "(p,d)" || str2[0] == "(d,p)") ){
      printf(" <==  ignored. \n");
      continue; 
    }
    printf("\n");
    
    printf("target nucleus : %s \n", str1[0].c_str());
    printf("      reaction : %s \n", str2[0].c_str());
    printf("        remain : %s \n", str2[1].c_str());
    printf("        energy : %s \n", str0[1].c_str());
    printf("     Potential : %s \n", str0[2].c_str());
    
    
  }
  

  file_in.close();

  //================= create ptolemy infile
  CalPotential("AK", 206, 82, 14.8);
  
  string saveFileName = argv[1];
  saveFileName += ".Xsec.txt";

  //printf(" save file : %s \n", saveFileName.c_str()); 
  //ofsteam file_out;



  //file_out.open(

  //================= run ptolemy


  //================= extract the Xsec and save as txt and root


  //================= Call root to plot the d.s.c.


  //================= load the experimental Xsec file in txt or in root, then fit


} 
