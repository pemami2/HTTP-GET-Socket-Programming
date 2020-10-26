all: main

main: main.cpp
	clang++ -std=c++11 -o main main.cpp 
 
  