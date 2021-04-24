#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "include/Node.hpp"
#include <stack>
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <string>

int Node_Can_Be_Zero(char incoming_char,std::vector<char> which_chars_cannot_be_zero)					// If node`s character is first letter of the word number of the node can not be zero thanks to this function.
{
	if (std::count(which_chars_cannot_be_zero.begin(), which_chars_cannot_be_zero.end(), incoming_char)) 
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

std::vector<int> Which_Number_Can_Node_Use(Node* parent_node,char incoming_char, std::vector<char> which_chars_cannot_be_zero)		// We determine here that , current node can take which number accordingly it`s restrictions.
{
	int parents_number = parent_node->number;
	std::vector<int> possible_numbers;
	std::vector<int> parent_numbers;
	
	for (Node* numbers : parent_node->parents) 
	{
		parent_numbers.push_back(numbers->number);
	}
	parent_numbers.push_back(parent_node->number);

	for (int i = Node_Can_Be_Zero(incoming_char, which_chars_cannot_be_zero); i < 10; ++i)	// If node number can be zero , start from it 0 otherwise starting from 1 thanks to this function => Node_Can_Be_Zero(incoming_char)
	{
		if (std::count(parent_numbers.begin(), parent_numbers.end(), i))	// Checking parents of the node contains which number.
		{
			continue;
		}
		possible_numbers.push_back(i);
	}

	return possible_numbers;
}

void Create_Tree(Node* parent, std::vector<char> karakterler , std::vector<char> which_chars_cannot_be_zero)	// Add new nodes to tree recursively.
{
	if (karakterler.size() == 0) { 
		return; 
	}

	parent->addChildren(parent, karakterler.front(), Which_Number_Can_Node_Use(parent, karakterler.front(), which_chars_cannot_be_zero));  // Add children to the node recursively. Adding the parents tree in the node. When leaf node is come , check the solution by these parents.

	karakterler.erase(karakterler.begin());

	for (Node* child : parent->children)
	{
		Create_Tree(child, karakterler,which_chars_cannot_be_zero);
	}
}

bool Reached_Leaf_Check_Solution(Node* leaf_node, std::vector<char> first_char_vector, std::vector<char> second_char_vector, std::vector<char> result_char_vector, std::vector<char> characters) // If the node has no children that means we reached the leaf.
																																															// Check the solution in terms of parents of the nodes. We keep them in the node member.
{
	std::vector<int> char_to_number;
	for (size_t i = 0; i < leaf_node->parents.size(); i++)			// Take the number of the children by order. For instance {1.2,5} for TWO string where T = 1 , W = 2 , O = 5
	{
		if (leaf_node->parents[i]->number == -10) continue;	// If parent is root, skipped it.
		int b = leaf_node->parents[i]->number;
		char_to_number.push_back(b);
	}
	char_to_number.push_back(leaf_node->number);

	int first_number  = 0;
	int second_number = 0;
	int result_number = 0;

	while (first_char_vector.empty() == false)
	{
		char letter = first_char_vector.front();
		int index_of_character = find(characters.begin(), characters.end(), letter) - characters.begin();
		int power_of_value = char_to_number[index_of_character];

		first_number += power_of_value * std::pow( 10  ,(first_char_vector.size() - 1));
		first_char_vector.erase(first_char_vector.begin());
	}

	while (second_char_vector.empty() == false)
	{
		char letter = second_char_vector.front();
		int index_of_character = find(characters.begin(), characters.end(), letter) - characters.begin();
		int power_of_value = char_to_number[index_of_character];

		second_number += power_of_value * std::pow(10, (second_char_vector.size() - 1));
		second_char_vector.erase(second_char_vector.begin());
	}

	while (result_char_vector.empty() == false)
	{
		char letter = result_char_vector.front();
		int index_of_character = find(characters.begin(), characters.end(), letter) - characters.begin();
		int power_of_value = char_to_number[index_of_character];

		result_number += power_of_value * std::pow(10, (result_char_vector.size() - 1));
		result_char_vector.erase(result_char_vector.begin());
	}

	if (first_number + second_number == result_number) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

int getSystemTime()		// Get the current time using chrono
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Node* BFS(Node* first_node, std::vector<char> first_char_vector, std::vector<char> second_char_vector, std::vector<char> result_char_vector , std::vector<char> characters, double& running_time , double& number_of_visited_nodes, int& max_nodes_in_memory)
{
	std::list<Node*> queue;
	queue.push_back(first_node);
	int starting_time = getSystemTime();	// Taking the starting time.

	while (queue.size() != 0)
	{
		Node* node = queue.front();
		queue.pop_front();
		number_of_visited_nodes++;
		
		if (node->children.size() == 0)
		{
			bool durum = Reached_Leaf_Check_Solution(node, first_char_vector ,second_char_vector ,result_char_vector , characters);
			if (durum == true)
			{
				running_time = (getSystemTime() - starting_time) / 1000.0;
				return node;			// Return the node which is having all parents as a leaf node
			}
		}

		for (Node* child : node->children)
		{
			queue.push_back(child);
		}
		max_nodes_in_memory = std::max(max_nodes_in_memory, static_cast<int>(queue.size()));
	}
	running_time = (getSystemTime() - starting_time) / 1000.0;		// Take the difference and get the running time if we do not find any solution.
	Node* node = new Node('e', -404);								// If there is no any solution , make the number -404 then use this number checking whether solution is made or not.
	return node;
}


Node* DFS(Node* start_node, std::vector<char> first_char_vector, std::vector<char> second_char_vector, std::vector<char> result_char_vector, std::vector<char> characters , double& running_time , double& number_of_visited_nodes, int& max_nodes_in_memory)
{
    double time = 0.0;
    int starting_time = getSystemTime();  // Taking the starting time of DFS function.

    std::stack<Node*> stack;
    stack.push(start_node);

    while (!stack.empty())
    {
        Node* node = stack.top();
        stack.pop();
		number_of_visited_nodes++;

        if (node->children.size() == 0)
        {
            bool solution_is_correct = Reached_Leaf_Check_Solution(node, first_char_vector, second_char_vector, result_char_vector, characters);		// We can check the solution for desired numbers into this function.

            if (solution_is_correct)
            {
                running_time = (getSystemTime() - starting_time) / 1000.0;		// Take the difference and get the running time
				return node;		// Return the node which is having all parents as a leaf node
            }
        }

        for (Node* child : node->children)
        {
            stack.push(child);
        }

        max_nodes_in_memory = std::max(max_nodes_in_memory, static_cast<int>(stack.size()));
    }
	running_time = (getSystemTime() - starting_time) / 1000.0;		// Take the difference and get the running time if we do not find any solution.
	Node* node = new Node('e',-404);								// If there is no any solution , make the number -404 then use this number checking whether solution is made or not.
    return node;
}

std::vector<char> Take_String_Get_Char(std::string incoming_word , std::vector<char> &which_chars_cannot_be_zero, std::vector<char> &distinct_chars)
{
	std::vector<char> returning_vector;
	for (int i = 0; i < incoming_word.length(); ++i)
	{
		if (i == 0) 
		{
			if (false == std::count(which_chars_cannot_be_zero.begin(), which_chars_cannot_be_zero.end(), incoming_word[i])) 
			{
				which_chars_cannot_be_zero.push_back(incoming_word[i]);
			}
		}

		if (false == std::count(distinct_chars.begin(), distinct_chars.end(), incoming_word[i]))
		{
			distinct_chars.push_back(incoming_word[i]);
		}

		returning_vector.push_back(incoming_word[i]);
	}
	return returning_vector;
}

void Show_The_Solution(std::string algorithm_name ,Node* leaf, std::vector<char> distinct_chars,int number_of_visited_nodes,int maximum_number_of_nodes_kept_in_the_memory, double running_time)
{
	std::cout << "Algorithm :  " << algorithm_name  << std::endl;
	std::cout << "Number of visited nodes : " << number_of_visited_nodes << std::endl;
	std::cout << "Maximum number of nodes kept in the memory : " << maximum_number_of_nodes_kept_in_the_memory << std::endl;
	std::cout << "Running time : " << running_time << std::endl;

	if (leaf->number == -404)
	{
		std::cout << "Solution is not found. " << std::endl;
		return;
	}

	leaf->parents.erase(leaf->parents.begin());
	leaf->parents.push_back(leaf);				// Adding the parents of the root for itself to print out the solution.

	std::cout << "Solution : ";
	for (size_t i = 0; i < distinct_chars.size(); i++)
	{
		std::cout << distinct_chars[i] << ": " << leaf->parents[i]->number;
		if (i != distinct_chars.size() - 1)
		{
			std::cout << ", ";
		}
		else 
		{
			std::cout << "\n";
		}
	} 
}

void Write_Matrix_Into_File(Node* leaf, std::vector<char> distinct_chars , std::string file_name)
{
	if (leaf->number == -404)
	{
		return;
	}

	std::ofstream myfile;
	myfile.open(file_name + ".txt");			// Open the file. Name is given by user.

	leaf->parents.push_back(leaf);				// Adding the parents of the root for itself to print out the solution.
	myfile << "\t";

	for (size_t i = 0; i < 10; i++)
	{
		myfile << i << "\t" ;
	}

	myfile << "\n";

	for (size_t i = 0; i < distinct_chars.size(); i++)
	{
		myfile << distinct_chars[i] << "\t";

		for (size_t j = 0; j < 10; j++)
		{
			if (leaf->parents[i]->number == j)
			{
				myfile << "1" << "\t";
			}
			else 
			{
				myfile << "." << "\t";
			}
		}
		myfile << "\n";
	}
	
	myfile.close();
}

int main(int argc, char** argv)
{
	if (std::string(argv[1]) != "DFS" && std::string(argv[1]) != "dfs" && std::string(argv[1]) != "BFS" && std::string(argv[1]) != "bfs")	// Checking the algorithm name.
	{
		std::cout << "There is no any algorithm by using this input : " << std::string(argv[1]) << "\n";
		return -1;
	}
	else if (argv[5] == nullptr)					// Checking the argument count. If number of arguments are less than desired return the function. Otherwise showing warning.
	{
		std::cout << "Missed input arguments ";
		return -1;
	}
	else if (argv[6] != nullptr)
	{
		std::cout << "Unnecessary input arguments , but still working. Waiting..." << "\n"; 
	}

	Node root;		// It takes empty string and -10 as a value by default. This is root node.
	std::string first_word		 = "TWO";
	std::string second_word		 = "TWO";
	std::string solution_word    = "FOUR";

	double running_time = 0, number_of_visited_nodes = 0;		// Define global variables to show the result.
	int maximum_number_of_nodes_kept_in_the_memory	 = 0;

	std::vector<char> first_char_vector            { };
	std::vector<char> second_char_vector           { };
	std::vector<char> result_char_vector           { };
	std::vector<char> which_chars_cannot_be_zero   { };
	std::vector<char> distinct_chars			   { };

	// Taking the reference of the inputs and returning one vector which contains different letters for each word. Then We are going to use these vectors checking the solution

	first_char_vector  = Take_String_Get_Char(first_word   ,    which_chars_cannot_be_zero  ,  distinct_chars);		
	second_char_vector = Take_String_Get_Char(second_word  ,    which_chars_cannot_be_zero  ,  distinct_chars);
	result_char_vector = Take_String_Get_Char(solution_word,    which_chars_cannot_be_zero  ,  distinct_chars);

	Create_Tree(&root, distinct_chars, which_chars_cannot_be_zero);

	Node* leaf_node = nullptr; 
	
	// According to parameter algorithm is started.

	if (std::string(argv[1]) == "DFS" || std::string(argv[1]) == "dfs")
	{
		leaf_node = DFS(&root, first_char_vector, second_char_vector, result_char_vector, distinct_chars, running_time, number_of_visited_nodes, maximum_number_of_nodes_kept_in_the_memory);
	}
	else if (std::string(argv[1]) == "BFS" || std::string(argv[1]) == "bfs")
	{
		leaf_node = BFS(&root, first_char_vector, second_char_vector, result_char_vector, distinct_chars, running_time, number_of_visited_nodes, maximum_number_of_nodes_kept_in_the_memory);
	}

	Show_The_Solution(std::string(argv[1]),leaf_node, distinct_chars, number_of_visited_nodes, maximum_number_of_nodes_kept_in_the_memory, running_time);

	Write_Matrix_Into_File(leaf_node, distinct_chars, std::string(argv[5]));
	
	return 0;
}
