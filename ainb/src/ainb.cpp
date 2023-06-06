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

	A_Block* a_blocks = new A_Block[header_data.a_blocks];

	for (int i = 0; i < header_data.a_blocks; i++) {
		A_Block* aBlock = new A_Block();
		aBlock->load(file);
		a_blocks[i] = *aBlock;
		cout << *aBlock << endl;
	}

	cout << "Finished Loading Type A Blocks" << endl << endl;

	cout << "Loading Type B Blocks" << endl;

	B_Block* b_blocks = new B_Block[header_data.b_blocks];

	for (int i = 0; i < header_data.b_blocks; i++) {
		B_Block bBlock;
		bBlock.load(file);
		b_blocks[i] = bBlock;
		cout << bBlock << endl;
	}

	cout << "Finished Loading Type B Blocks" << endl << endl;

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


