CC=g++

all: Cleopatra InFileCreator ExtractXSec PlotTGraphTObjectArray

Cleopatra: Cleopatra.C Isotope.h constant.h potentials.h InFileCreator.h ExtractXSec.h PlotTGraphTObjectArray.h
	$(CC) Cleopatra.C -o Cleopatra `root-config --cflags --glibs`

InFileCreator: InFileCreator.C InFileCreator.h Isotope.h constant.h potentials.h
	$(CC) InFileCreator.C -o InFileCreator `root-config --cflags --glibs`

ExtractXSec: ExtractXSec.C ExtractXSec.h
	$(CC) ExtractXSec.C -o ExtractXSec `root-config --cflags --glibs`

PlotTGraphTObjectArray: PlotTGraphTObjectArray.C PlotTGraphTObjectArray.h
	$(CC) PlotTGraphTObjectArray.C -o PlotTGraphTObjectArray `root-config --cflags --glibs`
