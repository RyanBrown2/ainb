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

		cout << endl;

	}

	//loadFooter(file);

	loadDataFootAddresses(file);

	//loadDataBody(file);

	return;


	//cout << endl << "Checks" << endl << endl;

	//for (int i = 0; i < bBlocks.size(); i++) {
	//	B_Block block = bBlocks[i];
	//	if (block.m_unknown1 == 0) {
	//		if (block.t_size != 0xa4) {
	//			cout << "Found Type B Block with Unknown 1 = 0 and Size != 0xa4" << endl;
	//			cout << "Block " << to_string(block.m_index) << endl;
	//			cout << "Address: " << hex << block.m_address << endl;
	//			cout << "Data Pointer: " << hex << block.m_dataPointer << endl;
	//			cout << "Unknown 1: " << hex << block.m_unknown1 << " (" << to_string(block.m_unknown1) << ")" << endl;
	//			cout << "Unknown 2: " << hex << block.m_unknown2 << " (" << to_string(block.m_unknown2) << ")" << endl;
	//			cout << "Unknown 3: " << hex << block.m_unknown3 << " (" << to_string(block.m_unknown3) << ")" << endl;
	//			cout << "Size: " << hex << block.t_size << " (" << to_string(block.t_size) << ")" << endl;
	//			cout << endl;
	//		}
	//	}
	//}

	//cout << "At Address: " << hex << maxBlock->m_address << endl;
	//if (b_BlockCount != bBlocks.size()) {
	//	cout << "Error: Type B Block Count Mismatch" << endl;
	//	cout << "Expected: " << to_string(b_BlockCount) << endl;
	//	cout << "Actual: " << to_string(bBlocks.size()) << endl << endl;
	//	return;
	//}



}

void AINB::loadDataBody(fstream& file) {
	// first entry in data body
	int first_entry_address = header_data.data_body_start + 0x30;
	file.seekg(first_entry_address, ios::beg);

	// todo - figure out what the beginning of the entry data is

	// check if entry has table
	file.seekg(0x94, ios::cur);
	int has_table;
	readIntFromStream(file, isBigEndian, 4, has_table);
	if (has_table != 0) {
		cout << "Entry has table" << endl;
	}

}

void AINB::loadDataFootAddresses(fstream& file)
{
	file.seekg(0x20, ios::beg);

	// load the table section start address
	file.seekg(0xc, ios::cur);
	readIntFromStream(file, isBigEndian, 4, data_foot_addresses.table_section_start);

	// load the structure section start address
	file.seekg(0x4, ios::cur);
	readIntFromStream(file, isBigEndian, 4, data_foot_addresses.structure_section_start);


}

void AINB::loadFooter(fstream& file)
{
	int initialPosition = file.tellg();

	//struct FootAddresses {
	//	int table_section_address;
	//	int structutre_section_address;
	//};

	file.seekg(0x28, ios::beg);

	cout << "Loading Footer Address Data" << endl;

	// find start of table section
	//file.seekg(0x8, ios::cur);

	// find end of footer
	int footerEndAddress;
	readIntFromStream(file, isBigEndian, 4, footerEndAddress);

	// find start of footer
	// also start of section one
	int footerStartAddress;
	readIntFromStream(file, isBigEndian, 4, footerStartAddress);

	// find start of section three
	int sectionThreeStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionThreeStartAddress);

	// find start of section two
	int sectionTwoStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionTwoStartAddress);
	// now at pos 0x38

	// find start of section four
	file.seekg(0x5c, ios::beg);
	int sectionFourStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionFourStartAddress);

	// find start of section five
	file.seekg(0x70, ios::beg);
	int sectionFiveStartAddress;
	readIntFromStream(file, isBigEndian, 4, sectionFiveStartAddress);

	file.seekg(initialPosition, ios::beg);

	cout << "Finished Loading Footer Address Data" << endl << endl;
	cout << "Section One: " << hex << footerStartAddress << " - " << sectionTwoStartAddress;
	cout << " Length: " << to_string(sectionTwoStartAddress - footerStartAddress) << endl;
	cout << "Section Two: " << hex << sectionTwoStartAddress << " - " << sectionThreeStartAddress;
	cout << " LengthL " << to_string(sectionThreeStartAddress - sectionTwoStartAddress) << endl;
	cout << "Section Three: " << hex << sectionThreeStartAddress << " - " << sectionFourStartAddress;
	cout << " Length: " << to_string(sectionFourStartAddress - sectionThreeStartAddress) << endl;
	cout << "Section Four: " << hex << sectionFourStartAddress << " - " << sectionFiveStartAddress;
	cout << " Length: " << to_string(sectionFiveStartAddress - sectionFourStartAddress) << endl;
	cout << "Section Five: " << hex << sectionFiveStartAddress << " - " << footerEndAddress;
	cout << " Length: " << to_string(footerEndAddress - sectionFiveStartAddress) << endl;

	cout << endl;

	cout << "Loading Section One" << endl;
	file.seekg(footerStartAddress, ios::beg);

	struct SectionOneAddresses {
		int addressOne;
		int addressTwo;
		int addressThree;
		int addressFour;
		int addressFive;
		int addressSix;
	};

	SectionOneAddresses sectionOneAddresses;

	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressOne);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressTwo);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressThree);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressFour);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressFive);
	readIntFromStream(file, isBigEndian, 4, sectionOneAddresses.addressSix);

	cout << "Finished Loading Section One" << endl << endl;

	cout << "Loading Section Two" << endl;


}


