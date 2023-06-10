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
		EntryPointCommand* aBlock = new EntryPointCommand();
		aBlock->load(file);
		aBlock->setName(string_list.getString(aBlock->getStringPointer()));
		m_a_commands.push_back(*aBlock);
		//cout << *aBlock << endl;
		//cout << endl;
	}

	for (int i = 0; i < m_header_data.b_commands; i++) {
		ExecutionCommand* bBlock = new ExecutionCommand();
		bBlock->load(file);
		bBlock->setName(string_list.getString(bBlock->getStringPointer()));
		bBlock->loadBody(file, string_list);
		m_b_commands.push_back(*bBlock);
		cout << *bBlock << endl;
		cout << endl;
	}

	map<int, int> pos_cout; // testing to see how many times a position has a value
	vector<int> unknown1s;
	vector<int> unknown2s;

	for (ExecutionCommand command : m_b_commands) {
		ExecutionCommand::BodyData body_data = command.getBodyData();
		for (const auto& pair : body_data.position_to_key) {
			int pos = pair.first;
			if (pos_cout.count(pos)) {
				pos_cout[pos] += 1;
			} else {
				pos_cout[pos] = 1;
			}
		}

		int unknown1 = command.m_unknown1;
		int unknown2 = command.m_unknown2;

		if (find(unknown1s.begin(), unknown1s.end(), unknown1) == unknown1s.end()) {
			unknown1s.push_back(unknown1);
		}

		if (find(unknown2s.begin(), unknown2s.end(), unknown2) == unknown2s.end()) {
			unknown2s.push_back(unknown2);
		}
		
	}


	for (const auto& pair : pos_cout) {
		int pos = pair.first;
		int count = pair.second;
		cout << "Pos: " << hex << pos << " Count: " << dec << count << endl;
	}

	cout << "Unknown 1's: ";
	for (int i = 0; i < unknown1s.size(); i++) {
		cout << hex << unknown1s[i] << " ";
	}
	cout << endl;

	cout << "Unknown 2's: ";
	for (int i = 0; i < unknown2s.size(); i++) {
		cout << hex << unknown2s[i] << " ";
	}
	cout << endl;

	cout << endl;
	
	return;

	Parameters parameters(file, &string_list);
	
	m_parameter_data = parameters.getData();

	for (int i = 0; i < m_parameter_data.parameters.size(); i++) {
		ParameterNode node = m_parameter_data.parameters[i];
		cout << node << endl;

		cout << endl;
	}

	return;
}

string AINB::getName() {
	return m_name;
}

AINB::AinbHeaderData AINB::getHeaderData()
{
	return m_header_data;
}

vector<EntryPointCommand> AINB::getACommands()
{
	return m_a_commands;
}

vector<ExecutionCommand> AINB::getBCommands()
{
	return m_b_commands;
}

