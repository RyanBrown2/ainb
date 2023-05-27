#pragma once
#include <iostream>

enum NodeType {
	string_table = -62,
};

NodeType detectNodeType(std::fstream& file);
