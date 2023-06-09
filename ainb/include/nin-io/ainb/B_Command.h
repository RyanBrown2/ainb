#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "BaseCommand.h"
#include "StringList.h"
#include "nin-io/util/util.h"

class B_Command : public BaseCommand
{
public:
	B_Command();
	~B_Command();
	void load(std::fstream& file);


	// temp public for testing
	int m_unknown1; // 0x00
	int getIndex() { return m_index; }

	struct TableValuePair {
		int value1;
		std::string value2;
	};

	struct Table {
		int address;
		int length;
		std::vector<TableValuePair> entries;
	};

	struct BodyData {
		int value2; // 0x1c
		int value3; // 0x44
	};

	void loadBody(std::fstream& file, StringList string_list);

	int getCommandId() { return m_command_id; }

	friend std::ostream& operator<<(std::ostream& os, const B_Command block);
private:

	int m_index; // 0x02
	int m_dataPointer; // 0x14
	int m_command_id; // 0x0c
	// unknown values

	BodyData m_body_data;	
	Table m_table;

	int m_unknown2; // 0x06
	int m_array_length;

	Table loadBodyTable(std::fstream& file, int length, StringList string_list);

};
