#pragma once
#include <list>
#include <vector>

class Node
{
public:
	Node(char c = ' ', int n = -10);		// If the node is root number is -10 and char is empty string. Furthermore if there is no solutin number of the node is -404.
	void addChildren(Node* parent,char child_karakteri, std::vector<int> rakamlar);

	char character = ' ';
	int  number    = -1;
	Node* parent   = nullptr;

	std::list<Node*> children;
	std::vector<Node*> parents;
};

Node::Node(char c, int n)
{
	this->character = c;
	this->number = n;
};

void Node::addChildren(Node* parent,char child_karakteri, std::vector<int> numbers)
{
	for (int number : numbers)
	{
		Node* child = new Node(child_karakteri, number);
		for (size_t i = 0; i < parent->parents.size(); i++)
		{
			child->parents.push_back(parents[i]);
		}
		child->parents.push_back(this);
		this->children.push_back(child);
	}
}
