CXX = g++
CXXFLAGS = -g -Wall --std=c++0x

SRC = src
OBJ = obj
BIN = bin

all: svd temporal blender recommender

test: all clean

blender: $(OBJ)/blender.o

$(OBJ)/blender.o:  $(SRC)/blender.cpp $(SRC)/blender.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/DataPoint.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/blender.cpp -o $(OBJ)/blender.o
	$(CXX) $(OBJ)/blender.o -o $(BIN)/blender

svd: $(OBJ)/svd.o

$(OBJ)/svd.o:  $(SRC)/svd.cpp $(SRC)/svd.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/DataPoint.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/svd.cpp -o $(OBJ)/svd.o
	$(CXX) $(OBJ)/svd.o -o $(BIN)/svd

temporal: $(OBJ)/temporal.o

$(OBJ)/temporal.o:  $(SRC)/temporal.cpp $(SRC)/temporal.hpp $(SRC)/DataReader.hpp $(SRC)/compute.cpp $(SRC)/DataPoint.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/temporal.cpp -o $(OBJ)/temporal.o
	$(CXX) $(OBJ)/temporal.o -o $(BIN)/temporal

recommender: $(OBJ)/recommender.o

$(OBJ)/recommender.o: $(SRC)/recommender.cpp $(SRC)/recommender.hpp $(SRC)/compute.cpp $(SRC)/DataPoint.hpp
	$(CXX) -c $(CXXFLAGS) $(SRC)/recommender.cpp -o $(OBJ)/recommender.o
	$(CXX) $(OBJ)/recommender.o -o $(BIN)/recommender

clean:
	rm -f $(OBJ)/* $(BIN)/*

.PHONY: all clean
