##Builds full executable for the program
Pancakes: main.cpp Node.hpp VectorHash.hpp
	g++ -std=c++20 -Wall -Wextra main.cpp -o Pancakes 