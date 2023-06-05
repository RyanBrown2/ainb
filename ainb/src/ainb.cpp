#include "nin-io/ainb/Ainb.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

AINB::AINB()
{
	isBigEndian = false;
}

AINB::~AINB()
{

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
	readIntFromStream(file, isBigEndian, 1, headerDataStart);

	// find number of type A blocks
	file.seekg(0xC, ios::beg);
	readIntFromStream(file, isBigEndian, 4, header_data.a_blocks);
	cout << "Type A Block Count: " << header_data.a_blocks << endl;

	// find number of type B blocks
	readIntFromStream(file, isBigEndian, 4, header_data.b_blocks);
	cout << "Type B Block Count: " << header_data.b_blocks << endl;

	cout << endl;

	// find end address of block section
	file.seekg(0x20, ios::beg);
	readIntFromStream(file, isBigEndian, 4, header_data.data_body_start);

	// find start of string list
	readIntFromStream(file, isBigEndian, 4, header_data.string_section_start);

	// find start of footer section
	file.seekg(0x2c, ios::beg);
	readIntFromStream(file, isBigEndian, 4, header_data.t_footer_start);


	// load footer
	//loadFooter(file);

	// find file type (sequence, logic, etc)
	file.seekg(0x64, ios::beg);
	int fileType;
	readIntFromStream(file, isBigEndian, 4, fileType);

	if (fileType == 2) {
		cout << "File Type: Sequence" << endl;
	}
	else if (fileType == 1) {
		cout << "File Type: Logic" << endl;
	}
	else {
		cout << "File Type: Unknown" << endl;
	}

	cout << endl;

	// Start Parsing DataBlocks
	cout << "Loading Type A Blocks" << endl;
	file.seekg(0x74, ios::beg);

	vector<A_Block> aBlocks;
	for (int i = 0; i < header_data.a_blocks; i++) {
		A_Block aBlock = A_Block::load(file);
		aBlocks.push_back(aBlock);
	}

	cout << "Finished Loading Type A Blocks" << endl << endl;

	cout << "Loading Type B Blocks" << endl;

	vector<B_Block> bBlocks;
	for (int i = 0; i < header_data.b_blocks; i++) {
		B_Block bBlock = B_Block::load(file);
		bBlocks.push_back(bBlock);
	}

	cout << "Finished Loading Type B Blocks" << endl << endl;


	// parse b blocks
	for (int i = 0; i < bBlocks.size(); i++) {
		B_Block block = bBlocks[i];

		cout << "Block " << to_string(block.m_index) << endl;
		cout << "Data Pointer: " << hex << block.m_dataPointer << endl;
		cout << "Unknown 1: " << hex << block.m_unknown1 << " (" << to_string(block.m_unknown1) << ")" << endl;
		cout << "Unknown 2: " << hex << block.m_unknown2 << " (" << to_string(block.m_unknown2) << ")" << endl;
		cout << "Unknown 3: " << hex << block.m_unknown3 << " (" << to_string(block.m_unknown3) << ")" << endl;

		if (i + 1 < bBlocks.size()) {
			int size = bBlocks[i + 1].m_dataPointer - block.m_dataPointer;
			cout << "Size: " << hex << size << " (" << to_string(size) << ")" << endl;
			bBlocks[i].t_size = size;
		}
		else {
			int size = header_data.t_footer_start - block.m_dataPointer;
			cout << "Size: " << hex << size << " (" << to_string(size) << ")" << endl;
			bBlocks[i].t_size = size;
		}

		file.seekg(block.m_dataPointer, ios::beg);

		loadDataBody(file);

		cout << endl;

	}

	//loadDataBody(file);

	// Loading data body
	//for (int i = 0; i < bBlocks.size(); i++) {
	//	B_Block block = bBlocks[i];
	//	int pointer_address = block.m_dataPointer;
	//	file.seekg(pointer_address, ios::beg);
	//	loadDataBody(file);
	//}

	//loadFooter(file);

	//loadDataFootAddresses(file);



	return;

}

void AINB::loadDataBody(fstream& file) {
	// first entry in data body
	//int first_entry_address = header_data.data_body_start + 0x30;
	//file.seekg(first_entry_address, ios::beg);

	// todo - figure out what the beginning of the entry data is

	// check if entry has table
	file.seekg(0x90, ios::cur);
	cout << hex << file.tellg() << endl;
	int has_table;
	readIntFromStream(file, isBigEndian, 2, has_table);
	if (has_table != 0) {

		cout << "Entry has table" << endl;
		cout << "Length: " << to_string(has_table) << endl;
	}
	else {
		cout << "Entry does not have table" << endl;
	}

	//cout << endl;

}

void AINB::loadDataFootAddresses(fstream& file)
{


}


