#include "nin-io/ainb/Ainb.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

AINB::AINB()
{
}

AINB::~AINB()
{

}

AINB::FileVariable AINB::readVariable(fstream& file, StringList* string_list)
{
	FileVariable var;
	file.read(var.raw_data, 4);
	int string_offset;
	readIntFromStream(file, is_big_endian, 4, string_offset);
	var.name = string_list->getString(string_offset);
	return var;
}

void AINB::load(fstream& file)
{
	cout << "Loading AINB" << endl << endl;

	//FileHeaderData header_data;

	file.seekg(0, ios::beg);

	// Reading Header Data
	file.read(header_data.type, 3);
	header_data.type[3] = '\0';

	// Check if file is AINB
	if (strcmp(header_data.type, "AIB") != 0) {
		cout << "File is not AINB" << endl;
		return;
	}

	int headerDataStart;
	readIntFromStream(file, is_big_endian, 1, headerDataStart);

	// find number of type A blocks
	file.seekg(0xC, ios::beg);
	readIntFromStream(file, is_big_endian, 4, header_data.a_blocks);
	cout << "Type A Block Count: " << header_data.a_blocks << endl;

	// find number of type B blocks
	readIntFromStream(file, is_big_endian, 4, header_data.b_blocks);
	cout << "Type B Block Count: " << header_data.b_blocks << endl;

	cout << endl;

	// find end address of block section
	file.seekg(0x20, ios::beg);
	readIntFromStream(file, is_big_endian, 4, header_data.data_body_start);

	// find start of string list
	readIntFromStream(file, is_big_endian, 4, header_data.string_section_start);

	// find start of footer section
	file.seekg(0x2c, ios::beg);
	readIntFromStream(file, is_big_endian, 4, header_data.t_footer_start);

	file.seekg(0x60, ios::beg);

	// find file root name offset
	int root_name_pointer;
	readIntFromStream(file, is_big_endian, 4, root_name_pointer);

	// find file type (sequence, logic, etc)
	int fileType;
	readIntFromStream(file, is_big_endian, 4, fileType);

	if (fileType == 2) {
		cout << "File Type: Sequence" << endl;
	}
	else if (fileType == 1) {
		cout << "File Type: Logic" << endl;
	}
	else {
		cout << "File Type: Unknown" << endl;
	}

	file.seekg(header_data.string_section_start, ios::beg);
	StringList string_list(file);

	cout << "Root Name: " << string_list.getString(root_name_pointer) << endl << endl;

	// Start Parsing DataBlocks
	cout << "Loading Type A Blocks" << endl << endl;
	file.seekg(0x74, ios::beg);

	A_Block* a_blocks = new A_Block[header_data.a_blocks];

	for (int i = 0; i < header_data.a_blocks; i++) {
		A_Block* aBlock = new A_Block();
		aBlock->load(file);
		aBlock->setString(string_list.getString(aBlock->getStringPointer()));
		a_blocks[i] = *aBlock;
		//cout << *aBlock << endl;
	}

	cout << "Finished Loading Type A Blocks" << endl << endl;

	cout << "Loading Type B Blocks" << endl << endl;

	B_Block* b_blocks = new B_Block[header_data.b_blocks];

	for (int i = 0; i < header_data.b_blocks; i++) {
		B_Block* bBlock = new B_Block();
		bBlock->load(file);
		bBlock->setString(string_list.getString(bBlock->getStringPointer()));
		bBlock->loadBody(file, string_list);
		b_blocks[i] = *bBlock;
		//cout << *bBlock << endl;
	}

	cout << "Finished Loading Type B Blocks" << endl << endl;

	cout << "Loading Data Body" << endl;

	// todo
	cout << endl;

	cout << "Loading Data Foot" << endl;

	DataFoot dataFoot(file, &string_list);

	
	DataFoot::StructureSectionData structure_data = dataFoot.getStructureSectionData();

	for (int i = 0; i < structure_data.parameter_nodes.size(); i++) {
		ParameterNode node = structure_data.parameter_nodes[i];
		cout << node << endl;
		if (node.getBlockRef() < header_data.b_blocks && node.getBlockRef() > 0) {
			cout << "Referenced Block: " << endl;
			cout << b_blocks[node.getBlockRef()] << endl;
		}
	}

	return;
}

void AINB::loadDataBody(fstream& file) {
	// first entry in data body

	// todo - figure out what the beginning of the entry data is

	// check if entry has table
	file.seekg(0x90, ios::cur);
	cout << hex << file.tellg() << endl;
	int has_table;
	readIntFromStream(file, is_big_endian, 2, has_table);
	if (has_table != 0) {
		cout << "Entry has table" << endl;
		cout << "Length: " << to_string(has_table) << endl;
	}
	else {
		cout << "Entry does not have table" << endl;
	}

	//cout << endl;

}

void AINB::loadDataFoot(fstream& file)
{
	//DataFoot dataFoot(file);

}


