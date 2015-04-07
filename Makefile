CXX = g++
CXXFLAGS = -g -Wall --std=c++0x

SRC = src
OBJ = obj
BIN = bin

all: recommender

recommender: $(OBJ)/recommender.o

$(OBJ)/recommender.o: $(SRC)/recommender.cpp $(SRC)/recommender.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/recommender.cpp -o $(OBJ)/recommender.o
	$(CXX) $(OBJ)/recommender.o -o $(BIN)/recommender

clean:
	rm -f $(OBJ)/* $(BIN)/*

.PHONY: all clean
