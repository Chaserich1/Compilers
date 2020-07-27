#Chase Richards
#CMPSCI 4280 P0
#Filename: makefile

CXX = g++
CXXFLAGS = -std=c++0x -Wall -g

P0: main.o tree.o
	$(CXX) $(CXXFLAGS) -o P0 main.o tree.o
	
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

tree.o: tree.cpp tree.h node.h
	$(CXX) $(CXXFLAGS) -c tree.cpp
	
clean:
	rm *.o *.inorder *.preorder *.postorder keyboard.txt P0
