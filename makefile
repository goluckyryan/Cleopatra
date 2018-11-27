CC=g++

all: Cleopatra InFileCreator ExtractXSec

Cleopatra: Cleopatra.C Isotope.h constant.h potentials.h InFileCreator.h
	$(CC) Cleopatra.C -o Cleopatra `root-config --cflags --glibs`

InFileCreator: InFileCreator.C InFileCreator.h Isotope.h constant.h potentials.h
	$(CC) InFileCreator.C -o InFileCreator `root-config --cflags --glibs`

ExtractXSec: ExtractXSec.C
	$(CC) ExtractXSec.C -o ExtractXSec `root-config --cflags --glibs`
