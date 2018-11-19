#!/usr/bin/python

import sys

if sys.version_info[0] >= 3:
   raise Exception("Python 2 is require to run the PyROOT")

import argparse
import math

def is_number(string):
   try:
      float(string)
      return True
   except ValueError:
      return False

#================================

parser = argparse.ArgumentParser()
parser.add_argument("fileName", type=str, help="input file name")
args = parser.parse_args()
fileName = args.fileName

print("=========== Extract X-sec from : ", fileName)

searchAngle = "0INPUT... anglemin"

saveFileName = fileName + ".Xsec.txt"

print(" saving X-sec into : ", saveFileName)

fopen = open(fileName, "r");

startExtract = 0 # 0 = false, 1 = for (d,p), 2 = for (d,d)

saveFileFirstLine = []
saveFileFirstLine.append("angle") 
dataMatrix = []
angle = []
Ex = []
totalXsec = []
totalXsec.append("Xsec_total") #from 0 to 180 degree theta_CM

count = 0
reactionNum = 0
lineNum = 0
Size = 0

print("===============================")

for line in fopen.readlines():
   line = line.strip()
   lineNum += 1
   
   posStart = line.find("$============================================")
   posEx = line.find("Ex=")
   if posEx >= 0 :
      Ex.append(float(line[posEx+3: posEx+8]))
   if posStart > 0: 
      startMsg = line[posStart + 46:]
      saveFileFirstLine.append(startMsg)
      count = 0   
      reactionNum += 1
      print("%2d" % reactionNum, startMsg)
   
   posReaction = line.find("REACTION:")
   if posReaction > 0:
      reaction = line[posReaction + 9:]
      #print("%2d" % reactionNum, reaction)
      
   posAngleMin = line.find("anglemin=")
   posAngleMax = line.find("anglemax=")
   posAngleStep = line.find("anglestep=") 
   if posAngleMin > 0:
      angleMin = int(line[posAngleMin+9:posAngleMax])
      angleMax = int(line[posAngleMax+9:posAngleStep])
      angleStep = float(line[posAngleStep+10:])
      Size = int((angleMax - angleMin)/angleStep)
      tempData = []
      #if reactionNum == 1: print(" getting angle setting : ", angleMin , angleMax, angleStep)
      
   if startExtract == 0 and line == "0    ANGLE          SIGMA/             SIGMA              RUTHERFORD         % PER    % PER" :
      startExtract = 2
      #print("-------- start extraction for (d,d).")

   if startExtract == 2 and line == "0  C.M.       iT11       T20       T21       T22" :
      startExtract = 0
      #print("-------- end of extraction for (d,d).")

   if startExtract == 2 :
      #print(" %s " %(line))
      if is_number(line[:7]) == True :
         dataStr = line[:40]
         data = dataStr.split()
         #print(data)
         if count <= Size and len(data) == 4 :
             if reactionNum == 1: angle.append(float(data[0]))
             tempData.append(float(data[2])) # 2 is sigma/Ruth, 3 is sigma
             count += 1
             if count == Size : dataMatrix.append(tempData)
 

   if startExtract == 0 and line == "0  C.M.  REACTION     REACTION   LOW L  HIGH L   % FROM" :
      startExtract = 1
      #print("---------------start extraction.")
      
   if startExtract == 1 and line == "0 JP  JT  LX MX      TOTAL      PERCENT     (VALUES FOR M > 0 NOT DOUBLED.)":
      startExtract = 0
      #print("---------------end of extraction.")
      
   if startExtract == 1 :
      #print(" %s " %(line) )
      #get angle and Xsec
      if is_number(line[:5]) == True :  
         dataStr = line[:19]
         data = dataStr.split()
         if count <= Size and len(data) == 2:
            if reactionNum == 1: angle.append(float(data[0]))
            tempData.append(float(data[1]))
            #print( count , Size)
            count += 1
            if count == Size : dataMatrix.append(tempData)
   
   posTotalXsec = line.find("0TOTAL:")
   if( posTotalXsec >= 0 ):
      totalXsec.append(float(line[posTotalXsec + 7 : posTotalXsec + 30]))

   posTotalXsec = line.find("0TOTAL REACTION CROSS SECTION =")
   if( posTotalXsec >= 0 ):
      totalXsec.append(float(line[posTotalXsec + 31 : posTotalXsec +  45]))
               
fopen.close()

#====== print data statistics

print("==============================")
print(" number of lines read  : ", lineNum)
print(" getting angle setting : ", angleMin , angleMax, angleStep)
print(" number of Angles      : ", Size, "(", count, ")" )
print(" number of reactions   : ", reactionNum)

if (count == 0) : 
   print("=========== no data extracted. exit.")
   exit();

#====== save dataMatrix
fsave = open(saveFileName, "w");

for x in saveFileFirstLine :
   fsave.write("%17s\t" % x)

print("==== saved : ", saveFileName)
   
fsave.write("\n")

for i in range(0, Size):
   fsave.write("%18.2f" % angle[i])
   for j in range(0, reactionNum):
      fsave.write("%18.6f" % dataMatrix[j][i])
   fsave.write("\n")

fsave.close()

#====== save into TGraph into root
print("===== save into TGraph in to root");

from ROOT import gROOT, gStyle, TGraph, TFile, TObjArray
gROOT.Reset();

rootFileName = fileName + ".root"
f0 = TFile(rootFileName, "RECREATE")

gList = TObjArray()

for j in range(0, reactionNum):
   gr = TGraph()
   gr.Clear()
   gr.SetName(saveFileFirstLine[j+1])
   for i in range(0, Size):
      gr.SetPoint(i, angle[i], dataMatrix[j][i])
   name = "gr" + str(j)
   gList->Add(gr);
   
gList->Write("gList", TObject::kSingleKey);
f0.Write()

#========= Calculate total Xsec
print("======================")

Xsec = []
deg2rad = 3.141592654 / 180.
dTheta = angleStep * deg2rad
phi = 6.283185307

for j in range(0, reactionNum):
   xsecTemp = 0 
   for i in range(0, Size):
      xsecTemp += dataMatrix[j][i] * math.sin( (angle[i]) * deg2rad ) * dTheta * phi
   Xsec.append(xsecTemp)
   print("%30s : %8.5f mb | From 0-180 deg : %8.5f mb " % (saveFileFirstLine[j+1], xsecTemp, totalXsec[j+1]) )
   
#if ( len(Xsec) == 3 ) : 
#   print("====================== average :")
#   for i in range(0, len(Xsec)/3) :
#      xs1 = (Xsec[3*i] + Xsec[3*i+1] + Xsec[3*i+1])/3.
#      print("%s : %f "% ( saveFileFirstLine[3*i+1][0:8], xs1))
