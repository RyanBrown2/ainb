#include "NodeUtil.h"
#include <iostream>
#include <fstream>
#include "../Util.h"

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
	file.seekg(-1, ios::cur);

	int typeInt = static_cast<int>(type[0]);

	NodeType nodeType = static_cast<NodeType>(typeInt);

	//std::cout << typeInt << std::endl;

	return nodeType;

}

void Node::processNode(fstream& file) {
	NodeType nodeType = detectNodeType(file);
	Node::nodeType = nodeType;

	switch (nodeType) {
	case NodeType::plain_hash:
		std::cout << "Plain hash" << std::endl;
		break;
	case NodeType::value_hash:
		std::cout << "Value hash" << std::endl;
		break;
	case NodeType::string:
		std::cout << "String" << std::endl;
		break;
	case NodeType::binary_data:
		std::cout << "Binary data" << std::endl;
		break;
	case NodeType::file_data:
		std::cout << "File data" << std::endl;
		break;
	case NodeType::array:
		std::cout << "Array" << std::endl;
		break;
	case NodeType::string_hash:
		std::cout << "String hash" << std::endl;
		processStringHashNode(file);
		break;
	case NodeType::string_table:
		std::cout << "String table" << std::endl;
		processStringTableNode(file);
		break;
	case NodeType::boolean:
		std::cout << "Boolean" << std::endl;
		break;
	case NodeType::int_num:
		std::cout << "Int" << std::endl;
		break;
	case NodeType::float_num:
		std::cout << "Float" << std::endl;
		break;
	case NodeType::uint_num:
		std::cout << "UInt" << std::endl;
		break;
	case NodeType::int64_num:
		std::cout << "Int64" << std::endl;
		break;
	case NodeType::uint64_num:
		std::cout << "UInt64" << std::endl;
		break;
	case NodeType::double_num:
		std::cout << "Double" << std::endl;
		break;
	case NodeType::null_value:
		std::cout << "Null" << std::endl;
		break;
	default:
		std::cout << "Unknown node type" << std::endl;
		break;
	}
}

Node::Node(fstream& file, bool isBigEndian) {
	//Node::nodeType = detectNodeType(file);
	
	this->isBigEndian = isBigEndian;
	
	Node::processNode(file);

	
}

Node::~Node() {

}

NodeType Node::getType() {
	return NodeType::plain_hash;
}

void Node::processValueHashNode(fstream& file) {
	char value[4];
	file.read(value, 4);
}

void Node::processStringHashNode(fstream& file) {
	// Get number of entries
	file.seekg(1, ios::cur);
	char entryNum[4];
	file.read(entryNum, 3);
	entryNum[3] = '\0';
	int entryNumInt = convertHexCharArrayToInt(entryNum, this->isBigEndian, 3);

	cout << "Entry num: " << entryNumInt << endl;

}

void Node::processStringTableNode(std::fstream& file) {
	// Get the number of entries
	file.seekg(1, ios::cur);
	char entryNum[4];
	file.read(entryNum, 3);
	entryNum[3] = '\0';

	int entryNumInt = convertHexCharArrayToInt(entryNum, isBigEndian, 3);

	cout << "Entry num: " << entryNumInt << endl;

	file.seekg(-1, ios::cur);
}
