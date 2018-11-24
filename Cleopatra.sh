#!/bin/bash


########################################################################
# 
#  This is Cleopatra.sh, a scripted version for Cleopatra
#
#  Using bash script provide flexibility that user can add difference
#      compoenents during the calculation
#
#  A full package includes fellowing:
#    1) create a in-file for ptolemy
#    2) run ptolemy from that in-file and output an out-file
#    3) extract cross-section distribution from the out-file
#                 save as txt or root TGraph format
#    4) call ROOT to draw the TGraph
#    5) load possible experimental Xsec and fit with Ptolemy calulation
#
#  User can easily select/comment-out different component 
#      to suit their needs
#-------------------------------------------------------
#  created by Ryan (Tsz Leung) Tang, Nov-18, 2018
#  email: goluckyryan@gmail.com
########################################################################

#for User, please Modify the path for thisroot.h
source /Applications/root/bin/thisroot.sh

if [ $# -eq 0 ] ; then
   echo "$./Cleopatra in-file"
  exit 1
fi;

loadfile=$1
infile=$1".in"
outfile=$1".out"

echo "#################################################################"
echo "##   &&&& &&    &&&&  &&&&  &&&&   &&&&  &&&&&& &&&&&   &&&&   ##"
echo "##  &&    &&    &    &&  && && && &&  &&   &&   && &&  &&  &&  ##"
echo "##  &&    &&    &&&& &&  && &&&&  &&&&&&   &&   &&&&&  &&&&&&  ##"
echo "##  &&    &&    &    &&  && &&    &&  &&   &&   && &   &&  &&  ##"
echo "##   &&&& &&&&& &&&&  &&&&  &&    &&  &&   &&   &&  &  &&  &&  ##"
echo "#################################################################"
echo "#####  Cleopatra, Ptolemy for (d,p),(p,d), (p,p) and (d,d)  #####"
echo "#################################################################"
echo "infile ----> "${loadfile}
echo "Ptolemy  infile ----> "${infile}
echo "Ptolemy outfile ----> "${outfile}

./InFileCreator ${loadfile}

echo "================================================================="
echo "===================   Ptolemy Calcualtion   ====================="
echo "================================================================="
./ptolemy <${infile}> ${outfile}

echo "================================================================="
echo "===================   Extract Xsec   ====================="
echo "================================================================="
./ExtractXsec.py ${outfile}

