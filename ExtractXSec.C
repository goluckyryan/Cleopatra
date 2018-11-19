#include <fstream>
#include <stdlib.h>
#include <cmath>

//TODO, use root

int main (int argc, char *argv[]) {
   
   printf("=================================================================\n");
   printf("========== Extract Cross-Section From Ptolemy out file ==========\n");
   printf("=================================================================\n");
   
   if(argc != 2) { 
      printf("Usage: ./ExtractXSec input_file\n");
      //printf("Usage: root "ExtracXSec(fileName)"\n");
      exit(0); 
   }else{
      printf("From file : %s \n", argv[1]);
   }

   ifstream file_in;
   file_in.open(argv[1], iso::in);

   string saveFileName = argv[1];

   saveFileName += ".Xsec.txt"

   //ofsteam file_out;

   
   
   //file_out.open(





   file_in.close();
} 
