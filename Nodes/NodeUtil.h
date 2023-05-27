#pragma once
#include <iostream>

enum NodeType {
	plain_hash = 32, // 	Plain hash. Node is a mapping from 32-bit hashes to other nodes.
	value_hash = 33, // Value hash. Node is a mapping from 32-bit hashes to other nodes and a second 4 byte value.
	string = -96, // 	String. Value is an index into the string table.
	binary_data = -95, // Binary data.
	file_data = -94, // 	File data. Node contains the binary data of another file.
	array = -64, // Array. Node is an array of nodes, typically, though not necessarily, all of the same format.
	string_hash = -63, // String hash. Node is a mapping from strings in the hash key table to other nodes.
	string_table = -62, // String table. Special purpose node type only seen in the hash key table and the string table.
	boolean = -48, // Bool. Node is 1 or 0 representing true or false respectively.
	int_num = -47, // Int. Node is a signed 32 bit integer value.
	float_num = -46, // Float. Node is a binary32 floating-point number.
	uint_num = -45, // UInt. Node is an unsigned 32 bit integer value. The game uses this for some CRC32 hashes and for masks.
	int64_num = -44, // Int64. Node is a 64 bit integer value.
	uint64_num = -43, // UInt64. Node is an unsigned 64 bit integer value.
	double_num = -42, // Double. Node is a binary64 floating-point number.
	null_value = -1, // Null. Value is always 0.
};

NodeType detectNodeType(std::fstream& file);

//void processNode(std::fstream& file);

class Node {
public:
	Node(std::fstream& file, bool isBigEndian);
	~Node();
	NodeType getType();
private:
	bool isBigEndian;
	NodeType nodeType;
	void processNode(std::fstream& file);

	void processValueHashNode(std::fstream& file);
	void processStringTableNode(std::fstream& file);
};
