#!/usr/bin/python
import math
import argparse
from ROOT import gROOT, gStyle, TCanvas, TH1F, TF1, TFile, TTree, TString, TGraph

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

#===============
tree.Draw("det")
