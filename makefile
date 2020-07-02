all: PR.o DV.o
    g++ PR.o -o PoisonedReverse
    g++ DV.o -o DistanceVector

DV.o: main.cpp
    g++ -c main.cpp

PR.o: PR.cpp
    g++ -c PR.cpp

clean:
    rm *.o DistanceVector PoisonedReverse