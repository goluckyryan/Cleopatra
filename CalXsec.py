#!/usr/bin/python
import math
import argparse
from ROOT import gROOT, gStyle, TCanvas, TH1F, TF1, TFile, TTree, TString, TGraph


def is_number(string):
   try:
      float(string)
      return True
   except ValueError:
      return False

#================================
parser = argparse.ArgumentParser()
parser.add_argument("fileName", type=str, help="input file name")
parser.add_argument("ExID", type=int, help="id of Ex")
args = parser.parse_args()
fileName = args.fileName
ExID = args.ExID

print("=========== Extract X-sec from : ", fileName)

searchAngle = "0INPUT... anglemin"

saveFileName = fileName + ".Xsec.txt"

print(" saving X-sec into : ", saveFileName)

fopen = open(fileName, "r");

startExtract = False

saveFileFirstLine = []
saveFileFirstLine.append("angle") 
dataMatrix = []
angle = []
Ex = []

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
 
   if startExtract == False and line == "0  C.M.  REACTION     REACTION   LOW L  HIGH L   % FROM" :
      startExtract = True
      #print("---------------start extraction.")
      
   if startExtract -- True and line == "0 JP  JT  LX MX      TOTAL      PERCENT     (VALUES FOR M > 0 NOT DOUBLED.)":
      startExtract = False
      #print("---------------end of extraction.")
      
   if startExtract == True :
      #print(" %s " %(line) )
      #get angle and Xsec
      if is_number(line[:5]) == True :  
         dataStr = line[:19]
         data = dataStr.split()
         if count <= Size and len(data) == 2:
            if reactionNum == 1:
               angle.append(float(data[0]))
            tempData.append(float(data[1]))
            #print( count , Size)
            count += 1
            if count == Size : dataMatrix.append(tempData)
         
               
fopen.close()

#====== print data statistics

print("==============================")
print(" number of lines read  : ", lineNum)
print(" getting angle setting : ", angleMin , angleMax, angleStep)
print(" number of Angles      : ", Size )
print(" number of reactions   : ", reactionNum)


######################################################
 
gROOT.Reset()

#============= root file input
rootfile="test.root";
treeName="tree"
f0 = TFile (rootfile, "read") 
tree = f0.Get(treeName)
print("=====> /// %15s //// is loaded. Total #Entry: %10d " % (rootfile,  tree.GetEntries()))

#============= Canvas
Div = [1,1]  #x,y
size = [800,600] #x,y

cScript = TCanvas("cScript", "cScript", 0, 0, size[0]*Div[0], size[1]*Div[1])
cScript.Divide(Div[0],Div[1])

for i in range(1, Div[0]*Div[1]+1) :
   cScript.cd(i).SetGrid()
cScript.cd(1)

gStyle.SetOptStat(1111111)
gStyle.SetStatY(0.99)
gStyle.SetStatX(0.99)
gStyle.SetStatW(0.2)
gStyle.SetStatH(0.1)
   

#ExID = 0

#============= create TGraph for theoritical cross section for ExID
gr = TGraph()

for i in range(0, len(angle)):
   gr.SetPoint(i, angle[i], dataMatrix[ExID][i])

gMax = gr.GetYaxis().GetXmax()

#============= get the acceptance from root simulation
nBin = 200
print(" ============================ Ex : %f " % Ex[ExID])
gate_e = "tag == 2 && TMath::Abs(Ex-%f) < 0.02 && thetaCM > 9 " % Ex[ExID]
h = TH1F("h", "h", nBin, 0, 50)
expression = "thetaCM >> h"
   
acceptAngle = []
for i in range(0,6) :
   gate = "&& detID == %d " % i
   tree.Draw(expression, gate_e + gate)
   
   if h.GetEntries() == 0 :
      print(" no data for detID = %d " % i)
      continue
   
   angleTemp = []
   for j in range(1, nBin) :
      a = h.GetBinContent(j)
      b = h.GetBinContent(j+1)

      if a == 0 and b > 0 :
         angleTemp.append(h.GetBinLowEdge(j+1))
         #print(" boundary : %10.5f" % h.GetBinLowEdge(j+1) );

      if a > 0 and b == 0 :
         angleTemp.append(h.GetBinLowEdge(j+1))
         #print(" boundary : %10.5f" % h.GetBinLowEdge(j+1) );
         
   if len(angleTemp) > 2 :
      del angleTemp[0]
      del angleTemp[0]
   
   #print( angleTemp )
   acceptAngle.append(angleTemp)
      
print(acceptAngle)
  
'''  
hMax = h.GetMaximum()
h.Scale(gMax/hMax)
gr.Draw("AC")
h.Draw("same")
cScript.Update()
'''
      
dCos = []
DegToRad = 0.017453292
for j in range(0, len(acceptAngle)):
   delta = acceptAngle[j][1] - acceptAngle[j][0]
   mean = (acceptAngle[j][1] + acceptAngle[j][0])/2
   dCos.append(math.sin(mean*DegToRad)*(delta*DegToRad))
   print("%2d | %10.5f - %10.5f = %10.5f | %10.5f, %10.5f "%(j, acceptAngle[j][1], acceptAngle[j][0], delta, mean, 1./dCos[j]) )


######################################################
#========= calculate intergated X-sec
print("=============================")

dphi = 4* 2 * math.atan2(4.5, 11.5)

#=== integrate X-sec
Xsec = 0.0
for i in range(0, Size) :
   for m in range(0, len(acceptAngle)):
      #check = 0
      #if acceptAngle[m][0] < angle[i] and angle[i] < acceptAngle[m][1] : check = 1
      #print("%d, %d, %5.2f, %5.2f, %5.2f, %10.4f, %d, %10.4f" % (i,  m,  acceptAngle[m][0], angle[i], acceptAngle[m][1], dataMatrix[ExID][i], check , Xsec))
      
      if acceptAngle[m][0] < angle[i] and angle[i] < acceptAngle[m][1] : 
         Xsec += dataMatrix[ExID][i] * math.sin( angle[i] * DegToRad) * angleStep * DegToRad * dphi
         
         
print("%s, total Xsec : %10.4f ub" % (saveFileFirstLine[ExID+1] , Xsec *1000 ))


## wait for input to keep the GUI (which lives on a ROOT event dispatcher) alive
if __name__ == '__main__':
   rep = ''
   while not rep in [ 'q', 'Q' ]:
      rep = raw_input( 'enter "q" to quit: ' )
      if 1 < len(rep):
         rep = rep[0]
