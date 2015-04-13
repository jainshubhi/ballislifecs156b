CXX = g++
CXXFLAGS = -g -Wall --std=c++0x

SRC = src
OBJ = obj
BIN = bin

all: recommender

test: all clean

recommender: $(OBJ)/recommender.o

$(OBJ)/recommender.o: $(SRC)/recommender.cpp $(SRC)/recommender.hpp $(SRC)/compute.cpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/recommender.cpp -o $(OBJ)/recommender.o
	$(CXX) $(OBJ)/recommender.o -o $(BIN)/recommender

clean:
	rm -f $(OBJ)/* $(BIN)/*

.PHONY: all clean
