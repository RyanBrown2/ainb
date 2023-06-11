#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "BaseCommand.h"
#include "StringList.h"
#include "nin-io/util/util.h"

class ExecutionCommand : public BaseCommand
{
public:
	ExecutionCommand();
	~ExecutionCommand();
	void load(std::fstream& file);


	// temp public for testing
	int m_unknown1; // 0x00
	int m_unknown2; // 0x06

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

	struct BodyDataStructure {
		std::vector<int> sections;

		std::map<int, int> section_to_key;
		std::map<int, int> section_to_value;

		void addSection(int section, int key, int value) {
			sections.push_back(section);
			section_to_key[section] = key;
			section_to_value[section] = value;
		}
	};

	struct BodyData {
		std::map<int, int> position_to_key;
		std::map<int, int> value_map;

		BodyDataStructure data_structure;
		
		
		Table table;
	};

	void loadBody(std::fstream& file, StringList string_list);

	int getCommandId() { return m_command_id; }

	BodyData getBodyData() { return m_body_data; }

	friend std::ostream& operator<<(std::ostream& os, const ExecutionCommand block);
private:

	int m_index; // 0x02
	int m_dataPointer; // 0x14
	int m_command_id; // 0x0c
	// unknown values

	BodyData m_body_data;	
	//Table m_table;


	int m_array_length;

	void loadBodyTable(std::fstream& file, int length, StringList string_list);

};
