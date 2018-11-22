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
 *  g++ cleopatra.C -o cleopatra `root-config --cflags --glibs`
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

#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <TROOT.h>
#include <TFile.h>
#include <TString.h>
#include "Isotope.h" // for geting Z
#include "potentials.h"

using namespace std;

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

  //================= read infile. extract the reactions, write pptolemy infile for each reaction
  ifstream file_in;
  file_in.open(argv[1], ios::in);

  if( !file_in ){
    printf(" cannot read file. \n");
    return 0 ; 
  }
  
  
  string ptolemyInFileName = argv[1];
  ptolemyInFileName += ".in";
  printf(" save to infile : %s \n", ptolemyInFileName.c_str()); 
  FILE * file_out;
  file_out = fopen (ptolemyInFileName.c_str(), "w+");

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
    
    string orbital = str0[1];
    string Ex = str0[2];
    string reactionEnergy = str0[3];
    string potential = str0[4];
    
    string isoA = str1[0];
    string isoB = str2[1];
    string reactionType = str2[0];
    
    
    printf("  target nucleus : %s \n", isoA.c_str());
    printf("        reaction : %s \n", reactionType.c_str());
    printf("          remain : %s \n", isoB.c_str());
    printf(" reaction energy : %s \n", reactionEnergy.c_str());
    printf("       Potential : %s \n", potential.c_str());
    printf("         orbital : %s \n", orbital.c_str());
    printf("        Ex [MeV] : %s \n", Ex.c_str());
    
    Isotope isotopeA(str1[0]);
    Isotope isotopeB(str2[1]);
    
    printf("A: %d, Z: %d, mass: %f MeV/c2 \n", isotopeA.A, isotopeA.Z, isotopeA.Mass);
    printf("A: %d, Z: %d, mass: %f MeV/c2 \n", isotopeB.A, isotopeB.Z, isotopeB.Mass);
    
    //write ptolmey infile
    fprintf(file_out, "$============================================ Ex=%s(%s)%s\n", Ex.c_str(), orbital.c_str(), potential.c_str());
    fprintf(file_out, "reset\n");
    fprintf(file_out, "REACTION: %s%s%s() ELAB=\n", isoA.c_str(), reactionType.c_str(), isoB.c_str());
    fprintf(file_out, "PARAMETERSET dpsb r0target \n");
    fprintf(file_out, "$lstep=1 lmin=0 lmax=30 maxlextrap=0 asymptopia=50 \n");
    fprintf(file_out, "\n");
    fprintf(file_out, "PROJECTILE \n");
    fprintf(file_out, "wavefunction av18 \n");
    fprintf(file_out, "r0=1 a=0.5 l=0 \n");
    fprintf(file_out, ";\n");
    fprintf(file_out, "TARGET\n");
    //fprintf(file_out, "JBIGA=0\n");
    fprintf(file_out, "nodes=1 l=4 jp=9/2 $node is n-1 \n");
    fprintf(file_out, "r0=1.25 a=.65 \n");
    fprintf(file_out, "vso=6 rso0=1.10 aso=.65 \n");
    fprintf(file_out, "rc0=1.3 \n");
    fprintf(file_out, ";\n");
    fprintf(file_out, "INCOMING\n");
    
    CallPotential("A", 206, 82, 14.8);
    
    fprintf(file_out, ";\n");
    fprintf(file_out, "OUTGOING\n");
    
    CallPotential("K", 206, 82, 14.8);
    
    fprintf(file_out, ";\n");
    fprintf(file_out, "anglemin=0 anglemax=50 anglestep=0.5\n");
    fprintf(file_out, ";\n");
    
    
  }
  
  fprintf(file_out, "end $================================== end of input\n");
  file_in.close();
  fclose(file_out);

  //================= create ptolemy infile






  
  //================= run ptolemy


  //================= extract the Xsec and save as txt and root


  //================= Call root to plot the d.s.c.


  //================= load the experimental Xsec file in txt or in root, then fit


} 
