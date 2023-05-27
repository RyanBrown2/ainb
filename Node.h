#pragma once
#include <iostream>
#include <fstream>

class Node
{
public:
	Node(std::fstream& file);
	~Node();
};

