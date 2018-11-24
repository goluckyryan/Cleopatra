CC=g++

all: Cleopatra InFileCreator

Cleopatra: Cleopatra.C Isotope.h constant.h potentials.h
	$(CC) Cleopatra.C -o Cleopatra `root-config --cflags --glibs`
	
InFileCreator: InFileCreator.C Isotope.h constant.h potentials.h
	$(CC) InFileCreator.C -o InFileCreator `root-config --cflags --glibs`
