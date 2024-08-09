/*
* Node.hpp - Node class definition
* Author: Zeno Marquis
* HW2: Informed Search
* 2/26/2024
*/

#include <vector>
#include <cmath>

#pragma once

//Node class definition
class Node {

public:

    //instantiates node using given information
    Node(std::vector<int> ordering, int backcost, bool using_astar){

        this->pancakes = ordering;
        this->backwards_cost = backcost;
        this->astar = using_astar;

        //program runs ucs identically to a* but with uniform forwards cost
        //note: order is checked excplicitly so a cost of 0 does not terminate
        if (astar){
            set_forwards_cost();
        } else {
            forwards_cost = 0;
        }

        this->total_cost = this->backwards_cost + this->forwards_cost;
        
    }

    //comparison operator (reversed to make the priority queue a min heap)
    bool operator<(const Node& n) const {

        return (this->total_cost >= n.total_cost);

    }

    //uses heuristic function outlined in Helmert 2010
    void set_forwards_cost(){
        int heuristic_value = 0;
        for (int i = 0; i < 10; i++){
            if (i != 9){
                if (abs(pancakes.at(i) - pancakes.at(i + 1)) > 1){
                    heuristic_value++;
                }
            } else {
                if (abs(pancakes.at(i) - 10) > 1){
                    heuristic_value++;
                }
            }
        }

        this->forwards_cost = heuristic_value;

    }

    //returns order of pancakes
    std::vector<int> get_order(){
        return pancakes;
    }

    //returns cost to get to the node
    int get_back_cost(){
        return backwards_cost;
    }

    //returns heuristic evaluation
    int get_forwards_cost(){
        return forwards_cost;
    }

    //returns total cost
    int get_total_cost(){
        return total_cost;
    }

    //return whether the program is using astar search
    bool using_astar(){
        return astar;
    }

    //sets the index of the parent node
    void set_parent(int parent){
        this->parent_index = parent;
    }

    //gets the index of the parent node
    int get_parent(){
        return parent_index;
    }

    //sets the index of the flip made to get to this node
    void set_flip_number(int number){
        flip_number = number;
    }

    //gets the index of the flip made to get to this node
    int get_flip_number(){
        return flip_number;
    }

    

private:
    bool astar;
    int backwards_cost;
    int forwards_cost;
    int total_cost;

    //array to store order of pancakes
    std::vector<int> pancakes;

    //parent index in the explored nodes vector (see main.cpp)
    int parent_index;
    int flip_number;
};
