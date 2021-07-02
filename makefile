all: PoisonedReverse.cpp DistanceVector.cpp
    g++ -o PoisonedReverse PoisonedReverse.cpp
    g++ -o DistanceVector DistanceVector.cpp

clean:
    rm DistanceVector PoisonedReverse
