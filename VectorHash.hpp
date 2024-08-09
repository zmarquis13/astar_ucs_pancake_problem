/*
* VectorHash.hpp - Hash function for pancake order vectors
* Author: Zeno Marquis
* HW2: Informed Search
* 2/26/2024
*/

#include <string>
#include <vector>

//hash operation definition
struct VectorHash {
    size_t operator()(const std::vector<int>& vec) const {

        //relies on the fact that all pancake sizes are 1-digit numbers,
        //concatenates them into one number (ex. 9876543210 -> 9,876,543,210)
        std::string ordering = "";
        int vector_size = vec.size();

        for (int i = 0; i < vector_size; i++){
            ordering += std::to_string(vec.at(i));
        }

        return stol(ordering);
    }
};