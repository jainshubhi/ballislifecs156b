CXX = g++
CXXFLAGS = -g -Wall --std=c++0x

SRC = src
OBJ = obj
BIN = bin
GEN = gen

all: averages svd knn rbm2 blender

test: all clean

averages: $(OBJ)/averages.o

$(OBJ)/averages.o: $(SRC)/averages.cpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/averages.cpp -o $(OBJ)/averages.o
	$(CXX) $(OBJ)/averages.o -o $(BIN)/averages

svd: $(OBJ)/svd.o

$(OBJ)/svd.o:  $(SRC)/svd.cpp $(SRC)/svd.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/svd.cpp -o $(OBJ)/svd.o
	$(CXX) $(OBJ)/svd.o -o $(BIN)/svd

knn: $(OBJ)/knn.o

$(OBJ)/knn.o:  $(SRC)/knn.cpp $(SRC)/knn.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/knn.cpp -o $(OBJ)/knn.o
	$(CXX) $(OBJ)/knn.o -o $(BIN)/knn

rbm2: $(OBJ)/rbm2.o

$(OBJ)/rbm2.o:  $(SRC)/rbm2.cpp $(SRC)/rbm2.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/constants.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/rbm2.cpp -o $(OBJ)/rbm2.o
	$(CXX) $(OBJ)/rbm2.o -o $(BIN)/rbm2

blender: $(OBJ)/blender.o

$(OBJ)/blender.o:  $(SRC)/blender.cpp $(SRC)/blender.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/constants.hpp $(SRC)/linear_regression.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/blender.cpp -o $(OBJ)/blender.o
	$(CXX) $(OBJ)/blender.o -o $(BIN)/blender

clean:
	rm -f $(OBJ)/* $(BIN)/*

.PHONY: all clean
