CC=g++

all: cleopatra

cleopatra: cleopatra.C Isotope.h constant.h potentials.h
	$(CC) cleopatra.C -o cleopatra `root-config --cflags --glibs`
