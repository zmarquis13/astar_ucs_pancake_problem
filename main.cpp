/*
* main.cpp - Search algorithm main file
* Author: Zeno Marquis
* HW2: Informed Search
* 2/26/2024
*/

#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <utility>
#include <ctime>
#include <forward_list>
#include <unordered_map>

#include "Node.hpp"
#include "VectorHash.hpp"

using namespace std;

int explore(Node *curr_node, priority_queue<Node, vector<Node> >* queue,
unordered_map<vector<int>, int, VectorHash>* nodes, 
vector<Node> *explored_nodes);

std::vector<int> flip(std::vector<int> pancakes, int flip_pos);
bool get_input();

void search(priority_queue<Node, vector<Node> >* queue,
unordered_map<vector<int>, int, VectorHash>* nodes,
vector<Node> *explored_nodes);

vector<int> get_starting_order();
bool check_valid_order(vector<int> ordering);


/**
     * Main function for search algorithm
     */
int main(){

    //user can choose between random ordering or setting their own ordering
    std::vector<int> order = get_starting_order();
    cout << "Starting order of pancakes: " << endl;
    
    for (int i = 0; i < 10; i++){
        cout << order.at(i) << ' ';
    }

    //get whether the user would like to use a* or ucs
    bool astar = get_input();

    //data structures for storing nodes avoiding duplicates, and exploring them
    unordered_map<vector<int>, int, VectorHash> nodes;

    //queue for exploring nodes
    priority_queue<Node, vector<Node> > node_queue;

    //tracks explored nodes
    vector<Node> explored_nodes;
    
    //starting state node
    Node *np1 = new Node(order, 0, astar);
    node_queue.push(*np1);
    nodes.emplace(np1->get_order(), -1);

    //find the solution
    search(&node_queue, &nodes, &explored_nodes);
    
    return 0;
}

/**
     * Explores the top node in the queue, checking if it's the goal and putting
     * its children in the queue if they are new or cheaper
     * 
     * @param curr_node Pointer to the node taken from the top of the queue
     * @param queue Pointer to priority queue of frontier nodes
     * @param nodes Pointer to hashmap that checks for duplicates
     * @param explored_nodes Pointer to vector of all nodes already explored
     * 
     * @return an integer index of the solution node if found, -1 otherwise
     */
int explore(Node *curr_node, priority_queue<Node, vector<Node> >* queue,
unordered_map<vector<int>, int, VectorHash>* nodes, 
vector<Node> *explored_nodes){

    //add the current node to the end of the explored nodes vector
    explored_nodes->push_back(*curr_node);
    int curr_index = explored_nodes->size() - 1;

    //check to see if the current node has the goal ordering
    int templist[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> goal(&templist[0], &templist[0] + 10);
    if (explored_nodes->at(curr_index).get_order() == goal){
        return curr_index;
    }

    //set cost to -1 so future copies with the same order will not go in queue
    nodes->at(curr_node->get_order()) = -1;

    //get values to assign to children
    int new_cost = curr_node->get_back_cost() + 1;
    bool using_astar = curr_node->using_astar();
    std::vector<int> curr_order = curr_node->get_order();
    int parent_index = explored_nodes->size() - 1;
   
    for (int i = 2; i < 11; i++){
        std::vector<int> flip_result = flip(curr_order, i);

        //checks hashmap for the new node, if it is not found
        //or the new node has a lower cost, put that node in the queue
        try{
            if (new_cost < nodes->at(flip_result)){
                nodes->at(flip_result) = new_cost;
                Node *np = new Node(flip_result, new_cost, using_astar);
                np->set_parent(parent_index);
                np->set_flip_number(i);
                queue->push(*np);
            }
        } catch (const std::out_of_range){
            nodes->emplace(flip_result, new_cost);
            Node *np = new Node(flip_result, new_cost, using_astar);
            np->set_parent(parent_index);
            np->set_flip_number(i);
            queue->push(*np);
        } 
    }
    return -1;
}


/**
     * Searches through the search space until a solution is found
     * 
     * @param queue Pointer to priority queue of frontier nodes
     * @param nodes Pointer to hashmap that checks for duplicates
     * @param explored_nodes Pointer to vector of all nodes already explored
     */
void search(priority_queue<Node, vector<Node> >* queue,
unordered_map<vector<int>, int, VectorHash>* nodes,
vector<Node>* explored_nodes){

    int goal_index = -1;
    Node start = queue->top();

    //keep exploring from queue until goal is reached 
    while (goal_index == -1){
        Node present_node = queue->top();
        queue->pop();
        goal_index = explore(&present_node, queue, nodes, explored_nodes);
    }

    cout << "Number of flips required: "; 
    cout << explored_nodes->at(goal_index).get_back_cost() << endl;

    cout << "Order of flips:" << endl;
    std::forward_list<string> sequence;

    while(goal_index != 0){
        cout << explored_nodes->at(goal_index).get_flip_number() << " ";
        goal_index = explored_nodes->at(goal_index).get_parent();
    }

    cout << endl;

}


/**
     * Flips the elements from 0 to a given index of the input vector
     * 
     * @param pancakes Vector containing an order of pancakes
     * @param flip_pos Integer position of the flip
     * 
     * @return the vector after the flip has been performed
     */
std::vector<int> flip(std::vector<int> pancakes, int flip_pos){

    std::vector<int> result = pancakes;

    for(int i = 0; i < flip_pos/2; i++){
        swap(result[i], result[flip_pos - 1 - i]);
    }

    return result;

}

/**
     * Prompts user to determine whether they want to use A*
     * 
     * @return a boolean representing their choice
     */
bool get_input(){

    int int_input;
    bool success = false;
    string instring;

    cout << "\nEnter the number of the algorithm you'd like to use:\n";
    cout << "1. UCS\n2. A*" << endl;

    while (!success){

        cin >> instring;

        try{
            int_input = stoi(instring);
        } catch(std::invalid_argument){
            int_input = 0;
        }  

        if (int_input == 1){
            return false;
        } else if (int_input == 2){
            return true;
        } else {
            cout << "Invalid input, please try again\n";
        }
    }
    
    return false;

}

/**
     * Prompts user to determine the starting order of pancakes
     * 
     * @return the starting pancake order
     */
vector<int> get_starting_order(){

    string instring;
    int int_input;
    vector<int> result;
    bool success = false;

    while (!success){
        result.clear();
        cout << "Enter 1 for a random starting order, or 2 to set your own: \n";
        cin >> instring;

        try{
            int_input = stoi(instring);
        } catch(std::invalid_argument){
            int_input = 0;
        }  

        if (int_input == 1){
            //create vector of 10 random ints
            vector<int> temp;
            for (int i = 0; i < 10; i++){
                temp.push_back(i);
            }
            for (int i = 0; i < 10; i++){
                srand (time(NULL));
                int random = rand() % temp.size();
                result.push_back(temp.at(random));
                temp.erase(temp.begin()+random);
            }

            success = true;
        } else if (int_input == 2){
            cout << "Enter the 10 integers from 0 to 9 in any order";
            cout << " with no repeats\n";

            for (int i = 0; i < 10; i++){
                int input;
                cout << "Item " << (i + 1) << ": ";
                cin >> input;
                result.push_back(input);
            }
            success = check_valid_order(result);
        } else {
            cout << "Invalid input, please try again\n";
        }
    }
    return result;
}

/**
     * Determine whether the given vector contains a valid pancake arrangement
     * 
     * @return a boolen representing if the order is valid
     */
bool check_valid_order(vector<int> ordering){

    for (int i = 0; i < 10; i++){

        //checks for out of bounds or repeat inputs
        if (ordering.at(i) > 9 || ordering.at(i) < 0){
            cout << "Invalid input, entered item out of range\n";
            return false;
        }

        for (int j = i + 1; j < 10; j++){
            if (ordering.at(j) == ordering.at(i)){
                cout << "Invalid input, number entered multiple times\n";
                return false;
            }
        }

    }

    return true;
}
