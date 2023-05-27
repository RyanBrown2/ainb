#include "NodeUtil.h"
#include <iostream>
#include <fstream>

using namespace std;



/*
* Detects the type of node that is being read.
* @param file The file stream to read from.
* The file stream must be at the start of the node.
*/
NodeType detectNodeType(fstream& file) {
	char type[2];
	file.read(type, 1);
	type[1] = '\0';

	int typeInt = static_cast<int>(type[0]);

	NodeType nodeType = static_cast<NodeType>(typeInt);

	std::cout << std::hex << type << std::endl;

	return nodeType;

}