CXX = g++
CXXFLAGS = -g -Wall --std=c++0x

SRC = src
OBJ = obj
BIN = bin
GEN = gen

all: averages svd blender

test: all clean

averages: $(OBJ)/averages.o

$(OBJ)/averages.o: $(SRC)/averages.cpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/averages.cpp -o $(OBJ)/averages.o
	$(CXX) $(OBJ)/averages.o -o $(BIN)/averages

svd: $(OBJ)/svd.o

$(OBJ)/svd.o:  $(SRC)/svd.cpp $(SRC)/svd.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/svd.cpp -o $(OBJ)/svd.o
	$(CXX) $(OBJ)/svd.o -o $(BIN)/svd

blender: $(OBJ)/blender.o

$(OBJ)/blender.o:  $(SRC)/blender.cpp $(SRC)/blender.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/blender.cpp -o $(OBJ)/blender.o
	$(CXX) $(OBJ)/blender.o -o $(BIN)/blender

clean:
	rm -f $(OBJ)/* $(BIN)/*

.PHONY: all clean
