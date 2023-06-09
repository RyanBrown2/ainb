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

void AINB::load(fstream& file)
{
	// Header Data

	file.seekg(0, ios::beg);

	// Reading Header Data
	file.read(m_header_data.type, 3);
	m_header_data.type[3] = '\0';

	// Check if file is AINB
	if (strcmp(m_header_data.type, "AIB") != 0) {
		cout << "File is not AINB" << endl;
		return;
	}

	int headerDataStart;
	readIntFromStream(file, is_big_endian, 1, headerDataStart);

	// find number of type A commands
	file.seekg(0xC, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_header_data.a_commands);

	// find number of type B commands
	readIntFromStream(file, is_big_endian, 4, m_header_data.b_commands);

	cout << endl;

	// find end address of command section
	file.seekg(0x20, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_header_data.data_body_start);

	// find start of string list
	readIntFromStream(file, is_big_endian, 4, m_header_data.string_section_start);

	// find start of parameter section
	file.seekg(0x2c, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_header_data.footer_start);

	file.seekg(0x60, ios::beg);

	// find file root name offset
	int root_name_pointer;
	readIntFromStream(file, is_big_endian, 4, root_name_pointer);

	// find file type (sequence, logic, etc)
	int fileType;
	readIntFromStream(file, is_big_endian, 4, fileType);

	//if (fileType == 2) {
	//	cout << "File Type: Sequence" << endl;
	//}
	//else if (fileType == 1) {
	//	cout << "File Type: Logic" << endl;
	//}
	//else {
	//	cout << "File Type: Unknown" << endl;
	//}

	file.seekg(m_header_data.string_section_start, ios::beg);
	StringList string_list(file);

	// get file name
	m_name = string_list.getString(0);

	//cout << "Root Name: " << string_list.getString(root_name_pointer) << endl << endl;

	// Start Parsing Commands

	file.seekg(0x74, ios::beg);

	for (int i = 0; i < m_header_data.a_commands; i++) {
		A_Command* aBlock = new A_Command();
		aBlock->load(file);
		aBlock->setString(string_list.getString(aBlock->getStringPointer()));
		m_a_commands.push_back(*aBlock);
	}

	for (int i = 0; i < m_header_data.b_commands; i++) {
		B_Command* bBlock = new B_Command();
		bBlock->load(file);
		bBlock->setString(string_list.getString(bBlock->getStringPointer()));
		bBlock->loadBody(file, string_list);
		m_b_commands.push_back(*bBlock);
	}

	Parameters dataFoot(file, &string_list);

	return;
}

string AINB::getName() {
	return m_name;
}

AINB::FileHeaderData AINB::getHeaderData()
{
	return m_header_data;
}

vector<A_Command> AINB::getACommands()
{
	return m_a_commands;
}

vector<B_Command> AINB::getBCommands()
{
	return m_b_commands;
}

